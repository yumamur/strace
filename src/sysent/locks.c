#include "../ft_print.h"
#include "../ft_utils.h"
#include "locks.xlat.h"

SYS_FUNC(flock)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("op");
	printflags(lock_ops, td->sc_args[1], "LOCK_???");

	return SF_DECODE_COMPLETE;
}