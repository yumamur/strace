#include "../ft_print.h"
#include "../ft_utils.h"
#include "ptrace.xlat.h"

void print_syscall_info_entry_struct(
	typeof(((struct ptrace_syscall_info *) 0)->entry) *pt,
	__kernel_ulong_t                                   len)
{
	print_struct_start();
	PRINT_MEMBER_LLU(*pt, nr);
	__kernel_ulong_t trunc_len = (len - offsetof(struct ptrace_syscall_info, entry.args))
								 / sizeof(pt->args[0]);

	if (trunc_len)
	{
		print_next_struct_member("nr");
		print_arr_start();
		for (__kernel_ulong_t i = 0; i < trunc_len; ++i)
		{
			printaddr(pt->args[i]);
			if (i)
				print_arg_sep();
		}
		print_arr_end();
	}
	print_struct_end();
}

void print_syscall_info_exit_struct(
	typeof(((struct ptrace_syscall_info *) 0)->exit) *pt,
	__kernel_ulong_t                                  len)
{
	print_struct_start();
	PRINT_MEMBER_L(*pt, rval);
	print_struct_member_sep();
	if (len < offsetof(struct ptrace_syscall_info, exit.is_error))
		PRINT_MEMBER_U(*pt, is_error);
	print_struct_end();
}

void print_syscall_info_seccomp_struct(
	typeof(((struct ptrace_syscall_info *) 0)->seccomp) *pt,
	__kernel_ulong_t                                     len)
{
	print_struct_start();
	PRINT_MEMBER_LLU(*pt, nr);
	__kernel_ulong_t trunc_len = (len - offsetof(struct ptrace_syscall_info, entry.args))
								 / sizeof(pt->args[0]);

	if (!trunc_len)
		return print_struct_end();

	print_next_struct_member("nr");
	print_arr_start();
	for (__kernel_ulong_t i = 0; i < trunc_len; ++i)
	{
		printaddr(pt->args[i]);
		if (i)
			print_arg_sep();
	}
	print_arr_end();

	if (len < offsetof(struct ptrace_syscall_info, seccomp.ret_data))
		return print_struct_end();
	PRINT_MEMBER_U(*pt, ret_data);

//	if (len < offsetof(struct ptrace_syscall_info, seccomp.reserved2))
//		return print_struct_end();
//	PRINT_MEMBER_U(*pt, reserved2);

	print_struct_end();
}

void printptrace_syscall_info(
	struct s_td     *td,
	__kernel_ulong_t data,
	__kernel_ulong_t user_size,
	unsigned int     kernel_response_size)
{
	struct ptrace_syscall_info info = {};
	__kernel_ulong_t           len = MIN(MIN(user_size, kernel_response_size), sizeof(info));

	if (!len || umovemem(td, &info, data, len) < 0)
	{
		print_debug("Invalid td or data");
		printaddr(data);
		return;
	}

	print_struct_start();
	PRINT_MEMBER_FLAG(info, op, ptrace_syscall_info_ops, "PTRACE_SYSCALL_INFO_");

/*	if (len < offsetof(struct ptrace_syscall_info, reserved))
		return print_struct_end();

	if (info.reserved)
	{
		print_struct_member_sep();
		PRINT_MEMBER_LLX(info, reserved);
	}

	if (len < offsetof(struct ptrace_syscall_info, flags))
		return print_struct_end();

	if (info.flags)
	{
		print_struct_member_sep();
		PRINT_MEMBER_LLX(info, flags);
	}
*/
	if (len < offsetof(struct ptrace_syscall_info, arch))
		return print_struct_end();
	print_struct_member_sep();
	PRINT_MEMBER_FLAG(info, arch, audit_archs, "AUDIT_ARCH_???");

	if (len < offsetof(struct ptrace_syscall_info, instruction_pointer))
		return print_struct_end();
	print_struct_member_sep();
	PRINT_MEMBER(info, instruction_pointer, printaddr);

	if (len < offsetof(struct ptrace_syscall_info, stack_pointer))
		return print_struct_end();
	print_struct_member_sep();
	PRINT_MEMBER(info, stack_pointer, printaddr);

	if (len < offsetof(struct ptrace_syscall_info, entry.nr))
		return print_struct_end();

	switch (info.op)
	{
	case PTRACE_SYSCALL_INFO_ENTRY:
		print_next_struct_member("entry");
		print_syscall_info_entry_struct(&info.entry, len);
		break;
	case PTRACE_SYSCALL_INFO_EXIT:
		print_next_struct_member("exit");
		print_syscall_info_exit_struct(&info.exit, len);
		break;
	case PTRACE_SYSCALL_INFO_SECCOMP:
		print_next_struct_member("seccomp");
		print_syscall_info_seccomp_struct(&info.seccomp, len);
		break;
	}
	return print_struct_end();
}

void printptrace_peeksiginfo_args(struct s_td *td, const __kernel_ulong_t addr)
{
	struct ptrace_peeksiginfo_args buf = {};

	if (umovemem(td, &buf, addr, sizeof(buf)) == -1)
		return printaddr(addr);

	print_struct_start();

	PRINT_MEMBER_LLU(buf, off);

	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(buf, flags, ptrace_peeksiginfo_flags, "PTRACE_PEEKSIGINFO_???");

	print_struct_end();
}

int decode_ptrace_entering(struct s_td *const td)
{
	__kernel_ulong_t op = td->sc_args[0];
	int              pid = td->sc_args[1];
	__kernel_ulong_t addr = td->sc_args[2];
	__kernel_ulong_t data = td->sc_args[3];

	FIRST_ARG("op");
	printflag(ptrace_requests, op, "PTRACE_???");

	if (op == PTRACE_SEIZE)
		return SF_DECODE_COMPLETE;

	NEXT_ARG("pid");
	PRINT_D(pid);

	if (op == PTRACE_ATTACH || op == PTRACE_INTERRUPT
		|| op == PTRACE_KILL || op == PTRACE_LISTEN)
		return SF_DECODE_COMPLETE;

	NEXT_ARG("addr");
	switch (op)
	{
		break;
	// case PTRACE_GETREGS:
	// case PTRACE_GETFPREGS:
	// 	return 0;
	// case PTRACE_SETREGS:
	// 	decode_pt_regs(td, data);
	// 	return SF_DECODE_COMPLETE;
	// case PTRACE_SETFPREGS:
	// 	decode_pt_fpregs(td, data);
	// 	return SF_DECODE_COMPLETE;
	case PTRACE_GETREGSET:
	case PTRACE_SETREGSET:
		printflag(nt_types, addr, "NT_???");
		break;
	case PTRACE_GETSIGMASK:
	case PTRACE_SETSIGMASK:
	case PTRACE_SECCOMP_GET_FILTER:
	case PTRACE_SECCOMP_GET_METADATA:
	case PTRACE_GET_SYSCALL_INFO:
		PRINT_U(addr);
		break;
	case PTRACE_PEEKSIGINFO:
		printptrace_peeksiginfo_args(td, addr);
		break;
	default:
		printaddr(addr);
	}

	switch (op)
	{
	case PTRACE_CONT:
	case PTRACE_DETACH:
	case PTRACE_SYSCALL:
	case PTRACE_SINGLESTEP:
	case PTRACE_SINGLEBLOCK:
	case PTRACE_SYSEMU:
	case PTRACE_SYSEMU_SINGLESTEP:
		printsignal(data);
		break;
	case PTRACE_SEIZE:
	case PTRACE_SETOPTIONS:
	case PTRACE_OLDSETOPTIONS:
		printflags(ptrace_option_flags, data, "PTRACE_O_???");
		break;
	case PTRACE_SETSIGINFO:
		printsiginfo(td, data);
		break;
	case PTRACE_SETSIGMASK:
		printsigmask_sized(td, data, addr);
		break;
	default:
		printaddr(data);
		break;
	}

	return SF_DECODE_COMPLETE;
}

int decode_ptrace_exiting(struct s_td *const td)
{
	// this would take another couple of hours, I'll ignore it at this point
	(void) td;
	return 0;
}

SYS_FUNC(ptrace)
{
	if (entering(*td))
		return decode_ptrace_entering(td);
	else
		return decode_ptrace_exiting(td);
}
