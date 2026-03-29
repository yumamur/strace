#ifndef UTIMES_XLAT_H
#define UTIMES_XLAT_H

#include "xlat.h"

#include <linux/fcntl.h>

const t_xlat_data utimensat_flags_data[] = {
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW),
};
WXLAT(utimensat_flags);

#endif /* UTIMES_XLAT_H */