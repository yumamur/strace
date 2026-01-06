#ifndef ACCESS_XLAT_H
#define ACCESS_XLAT_H

#include "xlat.h"
#include <linux/fcntl.h>
#include <unistd.h>

const t_xlat_data access_modes_data[] = {
	XLAT(F_OK),
	XLAT(R_OK),
	XLAT(W_OK),
	XLAT(X_OK),
};
WXLAT(access_modes);

const t_xlat_data faccessat2_flags_data[] = {
	XLAT(AT_EACCESS),
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW),
};
WXLAT(faccessat2_flags);

#endif