#include "../ft_common.h"
#include "../ft_print.h"
#include "namespace.xlat.h"

SYS_FUNC(mount)
{
	FIRST_ARG("source");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("target");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("filesystemtype");
	if (td->sc_args[3]
		& (MS_REMOUNT | MS_BIND | MS_MOVE | MS_UNBINDABLE | MS_PRIVATE | MS_SLAVE | MS_SHARED))
		printstr(td, td->sc_args[2]);
	else
		printaddr(td->sc_args[2]);

	NEXT_ARG("mountflags");
	printflags(mount_flags, td->sc_args[3], "MS_???");

	NEXT_ARG("data");
	printaddr(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(umount)
{
	FIRST_ARG("target");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(umount_flags, td->sc_args[1], "MNT_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(open_tree)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("filename");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(open_tree_flags, td->sc_args[2], "OPEN_TREE_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(move_mount)
{
	FIRST_ARG("from_dfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("from_pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("to_dfd");
	printdirfd(td, td->sc_args[2]);

	NEXT_ARG("to_pathname");
	printpath(td, td->sc_args[3]);

	NEXT_ARG("flags");
	printflags(move_mount_flags, td->sc_args[2], "MOVE_MOUNT_???");

	return SF_DECODE_COMPLETE;
}