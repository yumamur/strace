#ifndef POLL_XLAT_H
#define POLL_XLAT_H

#include "xlat.h"
#include <sys/poll.h>

const t_xlat_data pollfd_events_data[] = {
	XLAT(POLLIN),
	XLAT(POLLPRI),
	XLAT(POLLOUT),
	XLAT(POLLERR),
	XLAT(POLLHUP),
	XLAT(POLLNVAL),
};
WXLAT(pollfd_events);

#endif