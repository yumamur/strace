#ifndef SWAPFILE_XLAT_H
#define SWAPFILE_XLAT_H

#include "xlat.h"

#include <sys/swap.h>

static const t_xlat_data swap_flags_data[] = {
	XLAT(SWAP_FLAG_PREFER),
	XLAT(SWAP_FLAG_DISCARD),
};
WXLAT(swap_flags);

#endif /* SWAPFILE_XLAT_H */