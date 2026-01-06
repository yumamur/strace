#include "../ft_common.h"
#include "../ft_print.h"
#include "../syscall_ent.h"
#include "open.xlat.h"
#include <linux/fcntl.h>
#include <string.h>

void print_open_flags(unsigned int flags)
{
	char        buffer[1024];
	const char *access_mode_name = search_xlat(open_access_flags, flags & O_ACCMODE);

	if (!access_mode_name)
		access_mode_name = "O_???";

	int written = snprintf(buffer, sizeof(buffer), "%s", access_mode_name);
	flags &= ~O_ACCMODE;

	if (flags && (size_t) written < sizeof(buffer) - 1)
	{
		buffer[written++] = '|';
		snprintflags(buffer + written,
					 sizeof(buffer) - written,
					 open_flags,
					 flags,
					 "O_???");
	}

	prints(buffer);
}

SYS_FUNC(open)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("flags");
	print_open_flags(td->sc_args[1]);

	if (td->sc_args[1] & (O_CREAT | O_TMPFILE))
	{
		NEXT_ARG("mode");
		printmode(td->sc_args[2]);
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(openat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	print_open_flags(td->sc_args[2]);

	if (td->sc_args[2] & (O_CREAT | O_TMPFILE))
	{
		NEXT_ARG("mode");
		printmode(td->sc_args[3]);
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(close)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}