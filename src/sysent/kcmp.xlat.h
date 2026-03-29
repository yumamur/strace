#ifndef KCMP_XLAT_H
#define KCMP_XLAT_H

#include "xlat.h"

#include <linux/kcmp.h>

const t_xlat_data kcmp_types_data[] = {
	XLAT(KCMP_FILE),
	XLAT(KCMP_VM),
	XLAT(KCMP_FILES),
	XLAT(KCMP_FS),
	XLAT(KCMP_SIGHAND),
	XLAT(KCMP_IO),
	XLAT(KCMP_SYSVSEM),
	XLAT(KCMP_EPOLL_TFD),
};
WXLAT(kcmp_types);

#endif /* KCMP_XLAT_H */