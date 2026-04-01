#include "../ft_print.h"
#include "fork.xlat.h"

#define IDX_TLS (current_abi == ABI_64BIT ? 4 : 3)
#define IDX_CTD (current_abi == ABI_64BIT ? 3 : 4)

void printtls(struct s_td *td, __kernel_ulong_t addr)
{
	if (current_abi == ABI_32BIT)
		printuser_desc(td, addr);
	else
		printaddr(addr);
}

SYS_FUNC(clone)
{
	const __kernel_ulong_t flags = td->sc_args[0] & ~CSIGNAL;

	if (entering(*td))
	{
		const uint32_t sig = td->sc_args[0] & CSIGNAL;
		FIRST_ARG("flags");
		if (flags)
		{
			printflags(clone_flags, flags, "CLONE_???");
			if (sig)
			{
				print_or();
				printsignal(sig);
			}
		}
		else
			printsignal(sig);

		NEXT_ARG("child_stack");
		printaddr(td->sc_args[1]);
	}
	else
	{
		if (flags & (CLONE_PARENT_SETTID | CLONE_PIDFD))
		{
			NEXT_ARG("parent_tid");
			printnum_addr_int32(td, td->sc_args[2]);
		}

		if (flags & CLONE_SETTLS)
		{
			NEXT_ARG("tls");
			printtls(td, td->sc_args[IDX_TLS]);
		}
		if (flags & (CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID))
		{
			NEXT_ARG("child_tid");
			printaddr(td->sc_args[IDX_CTD]);
		}
	}
	return 0;
}

SYS_FUNC(set_tid_address)
{
	FIRST_ARG("tidptr");
	printaddr(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(unshare)
{
	FIRST_ARG("flags");
	printflags(clone_flags, td->sc_args[0], "CLONE_???");

	return 0;
}

SYS_FUNC(setns)
{
	FIRST_ARG("fd");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("nstype");
	printflag(clone_flags, td->sc_args[1], "CLONE_NEW???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(clone3)
{
	FIRST_ARG("uargs");
	// clone3 has 7 args, and since the syscalls can have 6 at most,
	// the first arg is an array of the args.
	printaddr(td->sc_args[0]);

	NEXT_ARG("size");
	PRINT_LU(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}