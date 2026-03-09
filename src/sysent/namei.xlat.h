#ifndef NAMEI_XLAT_H
#define NAMEI_XLAT_H

#include "xlat.h"

#include <linux/fs.h>

const t_xlat_data rename_flags_data[] = {
	XLAT(RENAME_EXCHANGE),
	XLAT(RENAME_NOREPLACE),
	XLAT(RENAME_WHITEOUT),
};
WXLAT(rename_flags);

#endif