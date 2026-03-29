#ifndef FADVISE_XLAT_H
#define FADVISE_XLAT_H

#include "xlat.h"

#include <linux/fadvise.h>

const t_xlat_data fadvise_flags_data[] = {
	XLAT(POSIX_FADV_NORMAL),
	XLAT(POSIX_FADV_RANDOM),
	XLAT(POSIX_FADV_SEQUENTIAL),
	XLAT(POSIX_FADV_WILLNEED),
	XLAT(POSIX_FADV_DONTNEED),
	XLAT(POSIX_FADV_NOREUSE),
};
WXLAT(fadvise_flags);

#endif /* FADVISE_XLAT_H */