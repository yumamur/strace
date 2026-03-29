#ifndef MEMFD_XLAT_H
#define MEMFD_XLAT_H

#include "xlat.h"

// #ifndef __USE_GNU
// #  define __USE_GNU
// #endif
// #include <sys/mman.h>
#include <fcntl.h>
#include <linux/memfd.h>

const t_xlat_data memfd_create_flags_data[] = {
	XLAT(MFD_CLOEXEC),
	XLAT(MFD_CLOEXEC),
	XLAT(MFD_ALLOW_SEALING),
	XLAT(MFD_HUGETLB),
	XLAT(MFD_NOEXEC_SEAL),
	XLAT(MFD_NOEXEC_SEAL),
	XLAT(MFD_EXEC),
};
WXLAT(memfd_create_flags);

const t_xlat_data memfd_secret_flags_data[] = {
	XLAT(FD_CLOEXEC),
};
WXLAT(memfd_secret_flags);

#endif /* MEMFD_XLAT_H */