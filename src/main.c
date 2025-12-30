#include "ft_common.h"
#include "ft_print.h"
#include "ft_utils.h"
#include "sysent/ft_sysent.h"
#include <bits/types/siginfo_t.h>
#include <errno.h>
#include <linux/elf.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>

#ifndef NT_PRSTATUS
#  define NT_PRSTATUS 1
#endif

#define MAX_SYSCALL 512

#define IS_SET(flag_) ((flag_) != 0)

static struct user_regs_struct g_regs;

static t_entry                 sysent[] = {
#define X(name) [__NR_##name] = {.call_name = #name, .logger = sys_##name, .traced = 0},
#include "syscalls.def"
#undef X
};

// flags
bool g_flag_trace = false;

//
static inline const char *get_syscall_name(t_td *td)
{
	unsigned long scno = td->sc_no;
	if (scno >= (long) (ARRAY_SIZE(sysent)))
		return "unknown";

	const char *s = sysent[scno].call_name;
	return s ? s : "unknown";
}

void fill_trace_data_entering(t_td *td)
{
	td->abi = detect_abi(&g_regs);
	switch (td->abi)
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

void fill_trace_data_exiting(t_td *td)
{
	td->abi = detect_abi(&g_regs);
	switch (td->abi)
	{
	case ABI_32BIT:
#ifdef __x86_64__
		td->sc_ret = g_regs.rax;
#else
		td->sc_ret = () & g_regs.eax;
#endif
		break;

	case ABI_X32:
#ifdef __x86_64__
		td->sc_ret = g_regs.rax;
#endif
		break;

	case ABI_64BIT:
	default:
#ifdef __x86_64__
		td->sc_ret = g_regs.rax;
#endif
	}
}

void syscallstart(t_td *td)
{
	fill_trace_data_entering(td);
	if (g_flag_trace == sysent[td->sc_no].traced)
	{
		print_syscall_enter(get_syscall_name(td));
		td->flags |= sysent[td->sc_no].logger(td);
	}
	td->flags |= TD_INSYSCALL;
}

void syscallend(t_td *td)
{
	fill_trace_data_exiting(td);
	if (g_flag_trace == sysent[td->sc_no].traced)
	{
		if (td->flags & SC_AFTER_RETURN)
		{
			print_syscall_return(td);
			print_space();
			sysent[td->sc_no].logger(td);
		}
		else
		{
			if (!(td->flags & SC_DECODE_COMPLETE))
				td->flags |= sysent[td->sc_no].logger(td);
			print_syscall_return(td);
		}
		td->flags &= ~SC_MASK;
		print_syscall_end();
	}
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
					sig = si.si_signo;
				else
					sig = stopsig;

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
	char  *inf_save;
	char  *tok;
	size_t cmdlen;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '.' || cmd[0] == '/')
	{
		strcpy(buf, cmd);
		if (access(buf, F_OK))
			perror_and_die(errno, "Cannot stat '%s'", cmd);
		return buf;
	}
	cmdlen = strlen(cmd);
	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	env_path = strdup(env_path);
	tok = __strtok_r(env_path, ":", &inf_save);
	size_t toklen = strlen(tok);
	while (tok && *tok && toklen + 1 + cmdlen <= buflen)
	{
		strcpy(buf, tok);
		buf[toklen] = '/';
		strcpy(buf + toklen + 1, cmd);
		if (access(buf, F_OK) == 0)
		{
			free(env_path);
			return buf;
		}
		tok = __strtok_r(NULL, ":", &inf_save);
		if (tok)
			toklen = strlen(tok);
	}
	free(env_path);
	return (NULL);
}

int check_and_set_syscall(const char *scname)
{
	static const size_t sysent_size = ARRAY_SIZE(sysent);
	for (size_t i = 0; i < sysent_size; i++)
		if (sysent[i].call_name && strcmp(scname, sysent[i].call_name) == 0)
			return !(sysent[i].traced = true);
	return -1;
}

int parse_trace_options(const char *arg)
{
	if (!*arg)
		return -1;

	char *copied = strdup(arg);
	char *inf_save = NULL;

	for (const char *tok = __strtok_r(copied, ",", &inf_save);
		 tok;
		 tok = __strtok_r(NULL, ",", &inf_save))
	{
		if (check_and_set_syscall(tok))
			die("Invalid syscall name: %s", tok);
	}

	g_flag_trace = true;
	free(copied);
	return (0);
}

int main(int argc, char *const *argv, char *const *envp)
{
	if (argc < 2)
		die("Usage: %s [--trace=<syscall_name[,syscall_name...]>] <program> [args...]",
			argv[0]);

	char         path[4096];
	unsigned int args_start = 1;

	if (argv[1][0] == '-' && strncmp(argv[1], "--trace=", 8) == 0)
	{
		parse_trace_options(argv[1] + 8);
		args_start = 2;
	}

	if (!get_executable(argv[args_start], path, sizeof(path)))
		perror_and_die(errno, "Cannot find executable '%s'", argv[args_start]);
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		return 1;
	}
	if (pid == 0)
	{
		raise(SIGSTOP);
		execve(path, &argv[args_start], envp);
		perror("execvp");
		exit(1);
	}
	else
		trace_syscalls(pid);

	return 0;
}