#include "../ft_print.h"
#include "pid.xlat.h"

SYS_FUNC(pidfd_open)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(pidfd_open_flags, td->sc_args[1], "PIDFD_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(pidfd_getfd)
{
	FIRST_ARG("pidfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("fd");
	printfd(td->sc_args[1]);

	NEXT_ARG("flags");
	// not implemented yet
	PRINT_X(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}