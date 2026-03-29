#include "../ft_print.h"
#include "eventfd.xlat.h"

SYS_FUNC(eventfd)
{
	FIRST_ARG("count");
	PRINT_U(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(eventfd2)
{
	FIRST_ARG("count");
	PRINT_U(td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(eventfd_flags, td->sc_args[1], "EFD_???");

	return SF_DECODE_COMPLETE;
}