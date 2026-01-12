#include "../ft_print.h"
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