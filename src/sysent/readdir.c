#include "../ft_common.h"
#include "../ft_print.h"

#define __USE_GNU
#include <dirent.h>

int decode_getdents(struct s_td *td)
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

SYS_FUNC(getdents)
{
	return decode_getdents(td);
}

SYS_FUNC(getdents64)
{
	return decode_getdents(td);
}