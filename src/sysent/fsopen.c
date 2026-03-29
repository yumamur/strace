#include "../ft_print.h"
#include "fsopen.xlat.h"

SYS_FUNC(fsopen)
{
	FIRST_ARG("fsname");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(fsopen_flags, td->sc_args[1], );

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fsconfig)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("cmd");
	printflags(fsconfig_flags, td->sc_args[1], "FSCONFIG_CMD_???");

	NEXT_ARG("key");
	printaddr(td->sc_args[2]);

	NEXT_ARG("value");
	printaddr(td->sc_args[3]);

	NEXT_ARG("aux");
	PRINT_U(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fsmount)
{
	FIRST_ARG("fs_fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(fsmount_flags, td->sc_args[1], "FSMOUNT_???");

	NEXT_ARG("attr_flags");
	printflags(fsmount_attr_flags, td->sc_args[2], "FSMOUNT_ATTR_???")

		return SF_DECODE_COMPLETE;
}

SYS_FUNC(fspick)
{
	FIRST_ARG("dfd");
	printdfd(td->sc_args[0]);

	NEXT_ARG("path");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(fspick_flags, td->sc_args[2], "FSPICK_???");

	return SF_DECODE_COMPLETE;
}