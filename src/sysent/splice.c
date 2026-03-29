#include "../ft_print.h"
#include "../ft_utils.h"
#include "splice.xlat.h"

SYS_FUNC(splice)
{
	FIRST_ARG("fd_in");
	printfd(td->sc_args[0]);

	NEXT_ARG("off_in");
	printnum_addr_int64(td, td->sc_args[1]);

	NEXT_ARG("fd_out");
	printfd(td->sc_args[2]);

	NEXT_ARG("off_out");
	printnum_addr_int64(td, td->sc_args[3]);

	NEXT_ARG("len");
	PRINT_U(td->sc_args[4]);

	NEXT_ARG("flags");
	printflags(splice_flags, td->sc_args[5], "SPLICE_F_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(tee)
{
	FIRST_ARG("fd_in");
	printfd(td->sc_args[0]);

	NEXT_ARG("fd_out");
	printfd(td->sc_args[1]);

	NEXT_ARG("len");
	PRINT_U(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(splice_flags, td->sc_args[3], "SPLICE_F_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(vmsplice)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("uiov");
	printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

	NEXT_ARG("nr_segs");
	PRINT_LU(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(splice_flags, td->sc_args[3], "SPLICE_F_???");

	return SF_DECODE_COMPLETE;
}