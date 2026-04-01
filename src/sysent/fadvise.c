#include "../ft_print.h"
#include "fadvise.xlat.h"

SYS_FUNC(fadvise64)
{
	FIRST_ARG("fd");
	PRINT_LU(td->sc_args[0]);

	NEXT_ARG("offset");
	unsigned int argi = print_ll_arg(td, 1);

	NEXT_ARG("len");
	__kernel_ulong_t len = td->sc_args[argi++];
	PRINT_LU(len);

	NEXT_ARG("advice");
	printflags(fadvise_flags, td->sc_args[argi++], "FADV_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fadvise64_64)
{
	FIRST_ARG("fd");
	PRINT_LU(td->sc_args[0]);

	NEXT_ARG("offset");
	unsigned int argi = print_ll_arg(td, 1);

	NEXT_ARG("len");
	argi = print_llu_arg(td, argi);

	NEXT_ARG("advice");
	printflags(fadvise_flags, td->sc_args[argi], "FADV_???");

	return SF_DECODE_COMPLETE;
}