#ifndef LSEEK_XLAT_H
#define LSEEK_XLAT_H

#include "xlat.h"
#include <unistd.h>

const t_xlat_data lseek_whence_values_data[] = {
	XLAT(SEEK_SET),
	XLAT(SEEK_CUR),
	XLAT(SEEK_END),
#ifdef SEEK_DATA
	XLAT(SEEK_DATA),
#endif
#ifdef SEEK_HOLE
	XLAT(SEEK_HOLE),
#endif
};
WXLAT(lseek_whence_values);

#endif