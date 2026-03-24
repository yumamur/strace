#ifndef MODULE_XLAT_H
#define MODULE_XLAT_H

#include "xlat.h"

#include <fcntl.h>
#include <linux/module.h>

const t_xlat_data module_init_flags_data[] = {
	XLAT(MODULE_INIT_IGNORE_MODVERSIONS),
	XLAT(MODULE_INIT_IGNORE_VERMAGIC),
	XLAT(MODULE_INIT_COMPRESSED_FILE),
};
WXLAT(module_init_flags);

const t_xlat_data module_delete_flags_data[] = {
	XLAT(O_TRUNC),
	XLAT(O_NONBLOCK),
};
WXLAT(module_delete_flags);

#endif /* MODULE_XLAT_H */