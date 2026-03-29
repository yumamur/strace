#include "../ft_print.h"
#include "../ft_utils.h"
#include "fanotify.xlat.h"

SYS_FUNC(fanotify_init)
{
	FIRST_ARG("flags");
	unsigned int flags = td->sc_args[0];

	printflags(fan_class_flags, flags & FAN_ALL_CLASS_BITS, "FAN_CLASS_???");
	flags &= ~FAN_ALL_CLASS_BITS;
	if (flags)
	{
		print_or();
		printflags(fan_init_flags, flags, "FAN_???");
	}

	NEXT_ARG("event_f_flags");
	print_open_flags((unsigned int) td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fanotify_mark)
{
	FIRST_ARG("fanotify_fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(fan_mark_flags, td->sc_args[1], "FAN_MARK_???");

	NEXT_ARG("mask");
	unsigned long long mask = 0;
	unsigned           iarg = get_ll_arg(td, &mask, 2);
	printflags(fan_events, mask, "FAN_???");

	NEXT_ARG("dfd");
	printdirfd(td, td->sc_args[iarg]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[iarg + 1]);

	return SF_DECODE_COMPLETE;
}