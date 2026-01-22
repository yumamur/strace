#include "../ft_print.h"
#include "file.xlat.h"
#include <asm/fcntl.h>

SYS_FUNC(dup)
{
	FIRST_ARG("oldfd");
	PRINT_D(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(dup2)
{
	FIRST_ARG("oldfd");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("newfd");
	PRINT_D(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(dup3)
{
	FIRST_ARG("oldfd");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("newfd");
	PRINT_D(td->sc_args[1]);

	if (td->sc_args[2] & O_CLOEXEC)
	{
		NEXT_ARG("flags");
		prints("O_CLOEXEC");
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(close_range)
{
	FIRST_ARG("first");
	printfd(td->sc_args[0]);

	NEXT_ARG("last");
	printfd(td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(close_range_flags, td->sc_args[2], "CLOSE_RANGE_???");

	return SF_DECODE_COMPLETE;
}
