#ifndef EVENTFD_XLAT_H
#define EVENTFD_XLAT_H

#include "xlat.h"

#include <sys/eventfd.h>

const t_xlat_data eventfd_flags_data[] = {
	XLAT(EFD_SEMAPHORE),
	XLAT(EFD_CLOEXEC),
	XLAT(EFD_NONBLOCK),
};
WXLAT(eventfd_flags);

#endif