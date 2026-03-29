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

SYS_FUNC(fchmodat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mode");
	printumode(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

void decode_chown(struct s_td *td, unsigned off)
{
	if (off)
		NEXT_ARG("pathname");
	else
		FIRST_ARG("pathname");

	printpath(td, td->sc_args[off]);

	NEXT_ARG("owner");
	PRINT_U(td->sc_args[off + 1]);

	NEXT_ARG("group");
	PRINT_U(td->sc_args[off + 2]);
}

SYS_FUNC(chown)
{
	decode_chown(td, 0);
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

SYS_FUNC(fchownat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	decode_chown(td, 1);

	NEXT_ARG("flags");
	printflags(fchownat_flags, td->sc_args[4], "AT_?");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fallocate)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("mode");
	printumode(td->sc_args[1]);

	NEXT_ARG("offset");
	unsigned int idx = print_ll_arg(td, 2);

	NEXT_ARG("len");
	print_ll_arg(td, idx);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(name_to_handle_at)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("handle");
	printaddr(td->sc_args[2]);

	NEXT_ARG("mnt_id");
	printnum_addr_int32(td, td->sc_args[3]);

	NEXT_ARG("flags");
	printflags(name_to_handle_at_flags, td->sc_args[4], "AT_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(open_by_handle_at)
{
	FIRST_ARG("mountdirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("handle");
	printaddr(td->sc_args[1]);

	NEXT_ARG("flags");
	print_open_flags(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}