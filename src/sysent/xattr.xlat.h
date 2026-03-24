#ifndef XATTR_XLAT_H
#define XATTR_XLAT_H

#include "xlat.h"

#include <linux/fcntl.h>
#include <linux/xattr.h>
#include <sys/xattr.h>

const t_xlat_data xattr_flags_data[] = {
	XLAT(XATTR_CREATE),
	XLAT(XATTR_REPLACE),
};
WXLAT(xattr_flags);

const t_xlat_data xattr_at_flags_data[] = {
	XLAT(AT_SYMLINK_NOFOLLOW),
	XLAT(AT_EMPTY_PATH),
};
WXLAT(xattr_at_flags);

#endif /* XATTR_XLAT_H */