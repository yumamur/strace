#ifndef OPEN_XLAT_H
#define OPEN_XLAT_H

#include "xlat.h"
#include <linux/fcntl.h>

const t_xlat_data open_access_flags_data[] = {
	XLAT(O_RDWR),
	XLAT(O_RDONLY),
	XLAT(O_WRONLY),
};
WXLAT(open_access_flags);

const t_xlat_data open_flags_data[] = {
	XLAT(O_CREAT),
	XLAT(O_EXCL),
	XLAT(O_NOCTTY),
	XLAT(O_TRUNC),
	XLAT(O_APPEND),
	XLAT(O_NONBLOCK),
	XLAT(O_DSYNC),
	XLAT(FASYNC),
	XLAT(O_DIRECT),
	XLAT(O_LARGEFILE),
	XLAT(O_DIRECTORY),
	XLAT(O_NOFOLLOW),
	XLAT(O_NOATIME),
	XLAT(O_CLOEXEC),
	XLAT_NAMED(__O_SYNC, "O_SYNC"),
	XLAT(O_PATH),
	XLAT_NAMED(__O_TMPFILE, "O_TMPFILE"),
};
WXLAT(open_flags);

#endif