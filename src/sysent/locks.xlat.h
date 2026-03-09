#ifndef LOCKS_XLAT_H
#define LOCKS_XLAT_H

#include "xlat.h"

#include <sys/file.h>

const t_xlat_data lock_ops_data[] = {
	XLAT(LOCK_SH),
	XLAT(LOCK_EX),
	XLAT(LOCK_UN),
	XLAT(LOCK_NB),
};
WXLAT(lock_ops);

#endif