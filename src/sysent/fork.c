#include "../ft_print.h"
#include "../ft_utils.h"
#include "fork.xlat.h"

#include <asm/ldt.h>

#define IDX_TLS (current_abi == ABI_64BIT ? 4 : 3)
#define IDX_CTD (current_abi == ABI_64BIT ? 3 : 4)

void printuser_desc(struct s_td *td, __kernel_ulong_t addr)
{
	struct user_desc ud;

	if (umovemem(td, &ud, addr, sizeof(ud)) < 0)
		return printaddr(addr);

	print_struct_start();

	if ((long long) (signed int) (ud.entry_number) == -1LL)
		prints("entry_number=-1");
	else
		PRINT_MEMBER(ud, entry_number, PRINT_U);
	print_struct_member_sep();

	PRINT_MEMBER(ud, base_addr, PRINT_U);
	print_struct_member_sep();
	PRINT_MEMBER(ud, limit, PRINT_U);
	print_struct_member_sep();
	PRINT_MEMBER(ud, seg_32bit, PRINT_U);
	print_struct_member_sep();
	PRINT_MEMBER(ud, read_exec_only, PRINT_U);
	print_struct_member_sep();
	PRINT_MEMBER(ud, limit_in_pages, PRINT_U);
	print_struct_member_sep();
	PRINT_MEMBER(ud, useable, PRINT_U);
	if (current_abi == ABI_64BIT)
	{
		print_struct_member_sep();
		PRINT_MEMBER(ud, lm, PRINT_U);
	}

	print_struct_end();
}

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
		prints("flags=");
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

		prints(", child_stack=");
		printaddr(td->sc_args[1]);
	}
	else
	{
		if (flags & (CLONE_PARENT_SETTID | CLONE_PIDFD))
		{
			prints(", parent_tid=");
			printnum_addr_int32(td, td->sc_args[2]);
		}

		if (flags & CLONE_SETTLS)
		{
			prints(", tls=");
			printtls(td, td->sc_args[IDX_TLS]);
		}
		if (flags & (CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID))
		{
			prints(", child_tid=");
			printaddr(td->sc_args[IDX_CTD]);
		}
	}
	return 0;
}