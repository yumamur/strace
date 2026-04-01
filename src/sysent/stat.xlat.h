#ifndef STAT_XLAT_H
#define STAT_XLAT_H

#include "xlat.h"

#include <sys/time.h>
#include <linux/fcntl.h>
#ifdef __USE_XOPEN2K8
#  undef __USE_XOPEN2K8
#  define __DISABLED_USE_XOPEN2K8
#else
#  undef __DISABLED_USE_XOPEN2K8
#endif
#include <linux/stat.h>
#include <sys/stat.h>
#ifdef __DISABLED_USE_XOPEN2K8
#  define __USE_XOPEN2K8
#  undef __DISABLED_USE_XOPEN2K8
#endif
#include <sys/sysmacros.h>

const t_xlat_data mode_file_types_data[] = {
	XLAT_NAMED(__S_IFDIR, "S_IFDIR"),
	XLAT_NAMED(__S_IFCHR, "S_IFCHR"),
	XLAT_NAMED(__S_IFBLK, "S_IFBLK"),
	XLAT_NAMED(__S_IFREG, "S_IFREG"),
	XLAT_NAMED(__S_IFIFO, "S_IFIFO"),
	XLAT_NAMED(__S_IFLNK, "S_IFLNK"),
	XLAT_NAMED(__S_IFSOCK, "S_IFSOCK"),
};
WXLAT(mode_file_types);

const t_xlat_data mode_protection_bits_data[] = {
	XLAT_NAMED(__S_ISUID, "S_ISUID"),
	XLAT_NAMED(__S_ISGID, "S_ISGID"),
	XLAT_NAMED(__S_ISVTX, "S_ISVTX"),
	// not sure why these are even defined in 'stat.h' in the first place
	// XLAT_NAMED(__S_IREAD, "S_IREAD"),
	// XLAT_NAMED(__S_IWRITE, "S_IWRITE"),
	// XLAT_NAMED(__S_IEXEC, "S_IEXEC"),
};
WXLAT(mode_protection_bits);

const t_xlat_data fstatat_flags_data[] = {
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_NO_AUTOMOUNT),
	XLAT(AT_SYMLINK_NOFOLLOW),
};
WXLAT(fstatat_flags);

const t_xlat_data at_statx_sync_types_data[] = {
	XLAT(AT_STATX_SYNC_AS_STAT),
	XLAT(AT_STATX_FORCE_SYNC),
	XLAT(AT_STATX_DONT_SYNC),
};
WXLAT(at_statx_sync_types);

const t_xlat_data statx_flags_data[] = {
	XLAT(STATX_TYPE),
	XLAT(STATX_MODE),
	XLAT(STATX_NLINK),
	XLAT(STATX_UID),
	XLAT(STATX_GID),
	XLAT(STATX_ATIME),
	XLAT(STATX_MTIME),
	XLAT(STATX_CTIME),
	XLAT(STATX_INO),
	XLAT(STATX_SIZE),
	XLAT(STATX_BLOCKS),
	XLAT(STATX_BASIC_STATS),
	XLAT(STATX_BTIME),
	XLAT(STATX_MNT_ID),
	XLAT(STATX_DIOALIGN),
	XLAT(STATX_MNT_ID_UNIQUE),
};
WXLAT(statx_flags);

#endif
