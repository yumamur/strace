#include "../ft_print.h"
#include "../ft_utils.h"
#include "fcntl.xlat.h"

SYS_FUNC(fcntl)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("op");
	printflags(fctnl_flags, td->sc_args[1], "F_???");

	// again, ton of work
	NEXT_ARG("arg");
	putnum(td->sc_args[2], HEX);

	return SF_DECODE_COMPLETE;
}