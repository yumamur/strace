#include "../ft_print.h"
#include "../ft_utils.h"
#include "seccomp.xlat.h"

SYS_FUNC(seccomp)
{
	FIRST_ARG("operation");
	printflag_indexed(seccomp_operations, td->sc_args[0], "SECCOMP_???");

	NEXT_ARG("flags");
	if (td->sc_args[0] == SECCOMP_SET_MODE_FILTER)
		printflags(seccomp_filter_flags, td->sc_args[1], "SECCOMP_FILTER_FLAG_???");
	else
		PRINT_X(td->sc_args[1]);

	NEXT_ARG("args");
	printaddr(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}
