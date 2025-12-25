#include "ft_common.h"
#include "ft_print.h"
#include "ft_utils.h"
#include "sysent/ft_sysent.h"
#include <bits/types/siginfo_t.h>
#include <errno.h>
#include <linux/elf.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>

#ifndef NT_PRSTATUS
#  define NT_PRSTATUS 1
#endif

#define MAX_SYSCALL 512

static struct user_regs_struct g_regs;

static t_entry                 sysent[] = {
#define X(name) [__NR_##name] = {.call_name = #name, .logger = sys_##name},
#include "syscalls.def"
#undef X
};

static inline const char *get_syscall_name(t_td *td)
{
	unsigned long scno = td->sc_no;
	if (scno >= (long) (sizeof(sysent) / sizeof(sysent[0])))
		return "unknown";

	const char *s = sysent[scno].call_name;
	return s ? s : "unknown";
}

void fill_trace_data(t_td *td)
{
	switch (detect_abi(&g_regs))
	{
	case ABI_32BIT:
#ifdef __x86_64__
		td->sc_no = g_regs.orig_rax;
		td->sc_args[0] = g_regs.rbx;
		td->sc_args[1] = g_regs.rcx;
		td->sc_args[2] = g_regs.rdx;
		td->sc_args[3] = g_regs.rsi;
		td->sc_args[4] = g_regs.rdi;
		td->sc_args[5] = g_regs.rbp;
#else
		td->sc_no = g_regs.orig_eax;
		td->sc_ret = () & g_regs.eax;
		td->sc_args[0] = g_regs.ebx;
		td->sc_args[1] = g_regs.ecx;
		td->sc_args[2] = g_regs.edx;
#endif
		break;

	case ABI_X32:
#ifdef __x86_64__
		td->sc_no = g_regs.orig_rax & ~__X32_SYSCALL_BIT;
		td->sc_args[0] = g_regs.rdi;
		td->sc_args[1] = g_regs.rsi;
		td->sc_args[2] = g_regs.rdx;
		td->sc_args[3] = g_regs.r10;
		td->sc_args[4] = g_regs.r8;
		td->sc_args[5] = g_regs.r9;
#endif
		break;

	case ABI_64BIT:
	default:
#ifdef __x86_64__
		td->sc_no = g_regs.orig_rax;
		td->sc_args[0] = g_regs.rdi;
		td->sc_args[1] = g_regs.rsi;
		td->sc_args[2] = g_regs.rdx;
		td->sc_args[3] = g_regs.r10;
		td->sc_args[4] = g_regs.r8;
		td->sc_args[5] = g_regs.r9;
#endif
	}
}

void syscallstart(t_td *td)
{
	fill_trace_data(td);
	printsyscallstart(get_syscall_name(td));
	sysent[td->sc_no].logger(td);
	td->flags |= TD_INSYSCALL;
}

void syscallend(t_td *td)
{
	printsyscallend(td);
	td->flags &= ~TD_INSYSCALL;
}

void trace_syscalls(pid_t child)
{
	int          status;
	struct iovec io;
	siginfo_t    si;
	t_td         td = {.pid = child};

	if (waitpid(child, &status, WUNTRACED) == -1)
	{
		perror("waitpid initial");
		return;
	}

	if (!WIFSTOPPED(status))
	{
		perror("Child not stopped\n");
		return;
	}

	if (ptrace(PTRACE_SEIZE, child, 0, PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC) == -1)
	{
		perror("ptrace seize");
		return;
	}

	if (ptrace(PTRACE_INTERRUPT, child, 0, 0) == -1)
	{
		perror("ptrace interrupt");
		return;
	}

	if (waitpid(child, &status, 0) == -1)
	{
		perror("waitpid after interrupt");
		return;
	}

	if (ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC) == -1)
	{
		perror("ptrace setoptions");
		return;
	}

	if (ptrace(PTRACE_SYSCALL, child, 0, 0) == -1)
	{
		perror("ptrace syscall");
		return;
	}

	while (1)
	{
		int sig = 0;

		if (waitpid(child, &status, 0) == -1)
		{
			if (errno == EINTR)
				continue;
			perror("waitpid");
			break;
		}

		if (WIFEXITED(status))
		{
			printexit(WEXITSTATUS(status));
			break;
		}

		if (WIFSIGNALED(status))
		{
			printkillsig(WTERMSIG(status));
			break;
		}

		if (WIFSTOPPED(status))
		{
			int stopsig = WSTOPSIG(status);

			if (stopsig == (SIGTRAP | 0x80))
			{
				io.iov_base = &g_regs;
				io.iov_len = sizeof(g_regs);

				if (ptrace(PTRACE_GETREGSET, child, NT_PRSTATUS, &io) == -1)
				{
					if (errno == ESRCH)
						break;
					perror("ptrace getregset");
					break;
				}

				if (entering(td))
					syscallstart(&td);
				else
					syscallend(&td);

				if (ptrace(PTRACE_SYSCALL, child, 0, 0) == -1)
				{
					if (errno == ESRCH)
						break;
					perror("ptrace syscall");
					break;
				}
			}
			else
			{
				if (ptrace(PTRACE_GETSIGINFO, child, 0, &si) == 0)
				{
					// putfmt("--- %s {si_signo=%d, si_code=%d} ---\n", strsignal(si.si_signo), si.si_signo, si.si_code);
					sig = si.si_signo;
				}
				else
				{
					sig = stopsig;
				}

				if (ptrace(PTRACE_SYSCALL, child, 0, sig) == -1)
				{
					if (errno == ESRCH)
						break;
					perror("ptrace syscall with signal");
					break;
				}
			}
		}
	}
}

char *get_executable(const char *cmd, char *buf, size_t buflen)
{
	char  *env_path;
	char  *tok;
	size_t cmdlen;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '.' || cmd[0] == '/')
	{
		strcpy(buf, cmd);
		if (access(buf, F_OK))
			perror_and_die("Cannot stat '%s': %s", cmd, strerror(errno));
		return buf;
	}
	cmdlen = strlen(cmd);
	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	tok = strtok(env_path, ":");
	while (tok && *tok && strlen(tok) + 1 + cmdlen <= buflen)
	{
		strcpy(buf, tok);
		buf[strlen(tok)] = '/';
		strcpy(buf + strlen(tok) + 1, cmd);
		if (access(buf, F_OK) == 0)
			return buf;
		tok = strtok(NULL, ":");
	}
	return (NULL);
}

int main(int argc, char *const *argv, char *const *envp)
{
	if (argc < 2)
		perror_and_die("Usage: %s <program> [args...]\n", argv[0]);
	char  another_buffer[4096];
	char *path = get_executable(argv[1], another_buffer, sizeof(another_buffer));
	if (!path)
		perror_and_die("Cannot find executable '%s'\n", argv[1]);

	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		return 1;
	}
	if (pid == 0)
	{
		raise(SIGSTOP);
		execve(another_buffer, &argv[1], envp);
		perror("execvp");
		exit(1);
	}
	else
		trace_syscalls(pid);

	return 0;
}