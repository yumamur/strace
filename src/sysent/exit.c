#include "../ft_print.h"
#include "../ft_utils.h"
#include <sys/resource.h>
#include <sys/signal.h>

#include "exit.xlat.h"

void printrusage(struct s_td *td, __kernel_ulong_t addr)
{
	struct rusage ru;

	if (umovemem_or_printaddr(td, &ru, addr, sizeof(struct rusage)))
		return;

	print_struct_start();

	PRINT_MEMBER_ADDR(ru, ru_utime, printtimeval_struct);
	print_struct_member_sep();
	PRINT_MEMBER_ADDR(ru, ru_stime, printtimeval_struct);
	print_struct_member_sep();
	if (is_verbose(*td))
	{
		PRINT_MEMBER(ru, ru_maxrss, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_minflt, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_majflt, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_nswap, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_inblock, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_oublock, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_msgsnd, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_msgrcv, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_nsignals, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_nvcsw, PRINT_LL);
		print_struct_member_sep();
		PRINT_MEMBER(ru, ru_nivcsw, PRINT_LL);
	}
	else
		print_has_more();

	print_struct_end();
}

SYS_FUNC(exit)
{
	FIRST_ARG("error_code");
	PRINT_D(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(wait4)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_D(td->sc_args[0]);

		return (0);
	}
	else
	{
		NEXT_ARG("status");
		if (!td->sc_ret)
			printaddr(td->sc_args[1]);
		else
			printnum_addr_int32(td, td->sc_args[1]);

		NEXT_ARG("options");
		printflags(wait4_options, td->sc_args[2], "W??");

		NEXT_ARG("usage");
		if (td->sc_ret > 0)
			printrusage(td, td->sc_args[3]);
		else
			printaddr(td->sc_args[3]);
	}

	return (SF_DECODE_COMPLETE);
}

SYS_FUNC(waitid)
{
	if (entering(*td))
	{
		FIRST_ARG("idtype");
		printflag(waitid_idtypes, td->sc_args[0], "P_???");

		NEXT_ARG("id");
		PRINT_LL(td->sc_args[1]);

		return 0;
	}
	else
	{
		NEXT_ARG("infop");
		printsiginfo(td, td->sc_args[2]);

		NEXT_ARG("options");
		printflags(wait4_options, td->sc_args[3], "W??");

		NEXT_ARG("usage");
		printrusage(td, td->sc_args[3]);
	}

	return SF_DECODE_COMPLETE;
}