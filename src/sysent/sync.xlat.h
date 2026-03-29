#ifndef SYNC_XLAT_H
#define SYNC_XLAT_H

#include "xlat.h"

#ifndef __USE_GNU
#  define __USE_GNU
#endif
#include <fcntl.h>

const t_xlat_data sync_file_range_flags_data[] = {
	XLAT(SYNC_FILE_RANGE_WAIT_BEFORE),
	XLAT(SYNC_FILE_RANGE_WRITE),
	XLAT(SYNC_FILE_RANGE_WAIT_AFTER),
};
WXLAT(sync_file_range_flags);

#endif /* SYNC_XLAT_H */