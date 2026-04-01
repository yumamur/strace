#include "../ft_print.h"

SYS_FUNC(getdents)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("dirp");
	// this does not even have a standard
	printaddr(td->sc_args[1]);

	NEXT_ARG("count");
	PRINT_U(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}