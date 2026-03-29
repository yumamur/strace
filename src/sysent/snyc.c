#include "../ft_print.h"
#include "../ft_utils.h"
#include "sync.xlat.h"

SYS_FUNC(sync_file_range)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("offset");
	unsigned idx = print_ll_arg(td, 1);

	NEXT_ARG("nbytes");
	idx = print_ll_arg(td, idx);

	NEXT_ARG("flags");
	printflags(sync_file_range_flags, td->sc_args[idx], "SYNC_FILE_RANGE_???");

	return SF_DECODE_COMPLETE;
}