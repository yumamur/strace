#include "../ft_print.h"
#include "../ft_utils.h"
#include "utimes.xlat.h"
#include <utime.h>

SYS_FUNC(utime)
{
	FIRST_ARG("filename");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("times");
	printutimbuf(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

void decode_utimes(struct s_td *td, unsigned off)
{
	if (off)
		NEXT_ARG("filename");
	else
		FIRST_ARG("filename");
	printpath(td, td->sc_args[off++]);

	NEXT_ARG("times");
	printutimbuf_utimes(td, td->sc_args[off]);
}

SYS_FUNC(utimes)
{
	decode_utimes(td, 0);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(futimesat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	decode_utimes(td, 1);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(utimensat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	decode_utimes(td, 1);

	NEXT_ARG("flags");
	printflags(utimensat_flags, td->sc_args[3], "UTIME_???");

	return SF_DECODE_COMPLETE;
}