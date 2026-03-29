#include "../ft_print.h"
#include "inotify.xlat.h"

SYS_FUNC(inotify_add_watch)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("mask");
	printflags(inotify_flags, td->sc_args[2], "IN_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(inotify_rm_watch)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("wd");
	PRINT_D(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(inotify_init1)
{
	FIRST_ARG("flags");
	printflags(inotify_init_flags, td->sc_args[0], "IN_???");

	return SF_DECODE_COMPLETE;
}