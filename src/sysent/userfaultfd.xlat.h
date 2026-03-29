#ifndef USERFAULTFD_XLAT_H
#define USERFAULTFD_XLAT_H

#include "xlat.h"

#include <linux/fcntl.h>
#include <linux/userfaultfd.h>

const t_xlat_data uffd_flags_data[] = {
	XLAT(UFFD_USER_MODE_ONLY),
	XLAT(O_NONBLOCK),
	XLAT(O_CLOEXEC),
};
WXLAT(uffd_flags);

#endif /* USERFAULTFD_XLAT_H */