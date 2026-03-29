#ifndef FSOPEN_XLAT_H
#define FSOPEN_XLAT_H

#include "xlat.h"

#include <linux/mount.h>

const t_xlat_data fsopen_flags_data[] = {
	XLAT(FSOPEN_CLOEXEC),
};
WXLAT(fsopen_flags);

const t_xlat_data fsconfig_flags_data[] = {
	XLAT(FSCONFIG_SET_FLAG),
	XLAT(FSCONFIG_SET_STRING),
	XLAT(FSCONFIG_SET_BINARY),
	XLAT(FSCONFIG_SET_PATH),
	XLAT(FSCONFIG_SET_PATH_EMPTY),
	XLAT(FSCONFIG_SET_FD),
	XLAT(FSCONFIG_CMD_CREATE),
	XLAT(FSCONFIG_CMD_RECONFIGURE),
	XLAT(FSCONFIG_CMD_CREATE_EXCL),
};
WXLAT(fsconfig_flags);

const t_xlat_data fsmount_flags_data[] = {
	XLAT(FSMOUNT_CLOEXEC),
};
WXLAT(fsmount_flags);

const t_xlat_data fsmount_attr_flags_data[] = {
	XLAT(MOUNT_ATTR_RDONLY),
	XLAT(MOUNT_ATTR_NOSUID),
	XLAT(MOUNT_ATTR_NODEV),
	XLAT(MOUNT_ATTR_NOEXEC),
	XLAT(MOUNT_ATTR__ATIME),
	XLAT(MOUNT_ATTR_RELATIME),
	XLAT(MOUNT_ATTR_NOATIME),
	XLAT(MOUNT_ATTR_STRICTATIME),
	XLAT(MOUNT_ATTR_NODIRATIME),
	XLAT(MOUNT_ATTR_IDMAP),
	XLAT(MOUNT_ATTR_NOSYMFOLLOW),
};
WXLAT(fsmount_attr_flags);

const t_xlat_data fspick_flags_data[] = {
	XLAT(FSPICK_CLOEXEC),
	XLAT(FSPICK_SYMLINK_NOFOLLOW),
	XLAT(FSPICK_NO_AUTOMOUNT),
	XLAT(FSPICK_EMPTY_PATH),
};
WXLAT(fspick_flags);

#endif /* FSOPEN_XLAT_H */