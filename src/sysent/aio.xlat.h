#ifndef AIO_XLAT_H
#define AIO_XLAT_H

#include "xlat.h"

#include <linux/aio_abi.h>

const t_xlat_data aio_rwf_flags_data[] = {
	XLAT(RWF_HIPRI),
	XLAT(RWF_DSYNC),
	XLAT(RWF_SYNC),
	XLAT(RWF_NOWAIT),
	XLAT(RWF_APPEND),
	XLAT(RWF_NOAPPEND),
	XLAT(RWF_ATOMIC),
	XLAT(RWF_DONTCACHE),
	XLAT(RWF_NOSIGNAL),
};
WXLAT(aio_rwf_flags);

#endif /* AIO_XLAT_H */