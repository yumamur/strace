#include "../ft_common.h"
#include "../ft_print.h"
#include "xlat.h"
#include <string.h>

void print_open_flags(unsigned int flags)
{
	char               buffer[1024];
	const unsigned int acf = flags & O_ACCMODE;
	const char        *access_mode_name = search_xlat(open_access_flags, acf);

	if (!access_mode_name)
		access_mode_name = "O_???";

	int          written = snprintf(buffer, sizeof(buffer), "%s", access_mode_name);
	unsigned int remaining_flags = flags & ~O_ACCMODE;

	if (remaining_flags && (size_t) written < sizeof(buffer) - 1)
	{
		buffer[written++] = '|';
		snprintflags(buffer + written,
					 sizeof(buffer) - written,
					 open_flags,
					 remaining_flags,
					 "O_???");
	}

	TPUTS(buffer);
}

SYS_FUNC(open)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("flags");
	print_open_flags(td->sc_args[2]);

	if (td->sc_args[1] & (O_CREAT | O_TMPFILE))
	{
		NEXT_ARG("mode");
		printmode(td->sc_args[2]);
	}
}

SYS_FUNC(openat)
{
	FIRST_ARG("dirfd");
	print_dirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	print_open_flags(td->sc_args[2]);

	if (td->sc_args[2] & (O_CREAT | O_TMPFILE))
	{
		NEXT_ARG("mode");
		printmode(td->sc_args[3]);
	}
}