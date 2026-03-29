#ifndef NAMEI_XLAT_H
#define NAMEI_XLAT_H

#include "xlat.h"

#include <linux/fs.h>
#include <linux/fcntl.h>

const t_xlat_data rename_flags_data[] = {
	XLAT(RENAME_EXCHANGE),
	XLAT(RENAME_NOREPLACE),
	XLAT(RENAME_WHITEOUT),
};
WXLAT(rename_flags);

const t_xlat_data at_flags_data[] = {
	XLAT(AT_SYMLINK_NOFOLLOW),
	XLAT(AT_REMOVEDIR),
	XLAT(AT_SYMLINK_FOLLOW),
	XLAT(AT_NO_AUTOMOUNT),
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_RECURSIVE),
};
WXLAT(at_flags);

#endif