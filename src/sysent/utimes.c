#include "../ft_print.h"
#include "../ft_utils.h"

#include <utime.h>

SYS_FUNC(utime)
{
	FIRST_ARG("filename");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("times");
	printutimbuf(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(utimes)
{
	FIRST_ARG("filename");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("times");
	printutimbuf_utimes(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}
