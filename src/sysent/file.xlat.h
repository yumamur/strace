#ifndef FILE_XLAT_H
#define FILE_XLAT_H

#include "xlat.h"

#include <linux/close_range.h>

const t_xlat_data close_range_flags_data[] = {
	XLAT(CLOSE_RANGE_UNSHARE),
	XLAT(CLOSE_RANGE_CLOEXEC),
};
WXLAT(close_range_flags);

#endif