#include "../ft_common.h"
#include "../ft_print.h"
#include "access.xlat.h"

SYS_FUNC(access)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[1], "?_OK");

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(faccessat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[2], "?_OK");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(faccessat2)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[2], "?_OK");

	NEXT_ARG("flags");
	printflags(faccessat2_flags, td->sc_args[3], "AT_?");

	return SF_DECODE_COMPLETE;
}
