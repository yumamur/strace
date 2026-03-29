#ifndef SIGNALFD_XLAT_H
#define SIGNALFD_XLAT_H

#include "xlat.h"

#include <sys/signalfd.h>

const t_xlat_data signalfd_flags_data[] = {
	XLAT(SFD_NONBLOCK),
	XLAT(SFD_CLOEXEC),
};
WXLAT(signalfd_flags);

#endif