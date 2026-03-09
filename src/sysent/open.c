#include "../ft_common.h"
#include "../ft_print.h"
#include "open.xlat.h"
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
		printumode(td->sc_args[2]);
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
		printumode(td->sc_args[3]);
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(access)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[1], "?_OK");

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(faccessat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[2], "?_OK");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(faccessat2)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printflags(access_modes, td->sc_args[2], "?_OK");

	NEXT_ARG("flags");
	printflags(faccessat2_flags, td->sc_args[3], "AT_?");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(close)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(truncate)
{
	FIRST_ARG("path");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("length");
	PRINT_LL(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(ftruncate)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("length");
	PRINT_LL(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(chdir)
{
	FIRST_ARG("path");
	printpath(td, td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fchmod)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("mode");
	printumode(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(chown)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("owner");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("group");
	PRINT_U(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fchown)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("owner");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("group");
	PRINT_U(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}
