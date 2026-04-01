#ifndef READ_WRITE_XLAT_H
#define READ_WRITE_XLAT_H

#include "xlat.h"
#include <linux/fs.h>
#include <unistd.h>

const char *lseek_whence_values[] = {
	XLAT_INDEXED(SEEK_SET),
	XLAT_INDEXED(SEEK_CUR),
	XLAT_INDEXED(SEEK_END),
#ifdef SEEK_DATA
	XLAT_INDEXED(SEEK_DATA),
#endif
#ifdef SEEK_HOLE
	XLAT_INDEXED(SEEK_HOLE),
#endif
};

#ifndef RWF_NOAPPEND
#  define RWF_NOAPPEND ((__kernel_rwf_t) 0x00000020)
#endif
#ifndef RWF_ATOMIC
#  define RWF_ATOMIC ((__kernel_rwf_t) 0x00000040)
#endif
#ifndef RWF_DONTCACHE
#  define RWF_DONTCACHE ((__kernel_rwf_t) 0x00000080)
#endif

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
