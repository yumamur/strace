#ifndef READ_WRITE_XLAT_H
#define READ_WRITE_XLAT_H

#include "xlat.h"
#include <linux/fs.h>
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

const t_xlat_data rwf_flags_data[] = {
	XLAT(RWF_HIPRI),
	XLAT(RWF_DSYNC),
	XLAT(RWF_SYNC),
	XLAT(RWF_NOWAIT),
	XLAT(RWF_APPEND),
	XLAT(RWF_NOAPPEND),
	XLAT(RWF_ATOMIC),
	XLAT(RWF_DONTCACHE),
};
WXLAT(rwf_flags);


#endif