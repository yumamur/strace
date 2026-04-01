#include "../ft_print.h"
#include "../ft_utils.h"

#include <asm/ldt.h>

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

SYS_FUNC(modify_ldt)
{
	if (entering(*td))
	{
		FIRST_ARG("func");
		PRINT_D(td->sc_args[0]);

		NEXT_ARG("ptr");

		// modify_ldt will read table with 'ptr->entry_number' into '*ptr'
		// so, wait for return
		if (td->sc_args[0] == 0)
			return 0;
		if (td->sc_args[2] == sizeof(struct user_desc))
			printuser_desc(td, td->sc_args[1]);
		else
			printaddr(td->sc_args[1]);

		NEXT_ARG("bytecount");
		PRINT_L(td->sc_args[2]);

		return SF_DECODE_COMPLETE;
	}
	else
	{
		printuser_desc(td, td->sc_args[1]);

		NEXT_ARG("bytecount");
		PRINT_L(td->sc_args[2]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(get_thread_area)
{
	if (exiting(*td))
	{
		FIRST_ARG("u_info");
		printuser_desc(td, td->sc_args[0]);
	}
	return 0;
}

SYS_FUNC(set_thread_area)
{
	FIRST_ARG("u_info");
	printuser_desc(td, td->sc_args[0]);
	return SF_DECODE_COMPLETE;
}