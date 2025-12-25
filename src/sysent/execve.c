#include "../ft_common.h"
#include "../ft_print.h"
#include "xlat.h"

SYS_FUNC(execve)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);
}

SYS_FUNC(execveat)
{
	FIRST_ARG("dirfd");
	print_dirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);
}