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
	printtimeval_utimes64(td, td->sc_args[off]);
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

void do_utimensat(struct s_td *td, typeof(printtimespec_utimes64) printtimespec_utimes_fn)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("filename");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("times");
	printtimespec_utimes_fn(td, td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(utimensat_flags, td->sc_args[3], "UTIME_???");
}

SYS_FUNC(utimensat_time32)
{
	do_utimensat(td, printtimespec_utimes32);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(utimensat_time64)
{
	do_utimensat(td, printtimespec_utimes64);
	return SF_DECODE_COMPLETE;
}