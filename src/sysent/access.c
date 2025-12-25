#include "../ft_common.h"
#include "../ft_print.h"
#include "xlat.h"

SYS_FUNC(access)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[1], "?_OK");
}

SYS_FUNC(faccessat)
{
	FIRST_ARG("dirfd");
	print_dirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[2], "?_OK");
}

SYS_FUNC(faccessat2)
{
	FIRST_ARG("dirfd");
	print_dirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[2], "?_OK");

	NEXT_ARG("flags");
	printflags(faccessat2_flags, td->sc_args[3], "AT_?");
}
