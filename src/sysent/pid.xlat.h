#ifndef PID_XLAT_H
#define PID_XLAT_H

#include "xlat.h"

#include <linux/pidfd.h>

const t_xlat_data pidfd_open_flags_data[] = {
	XLAT(PIDFD_NONBLOCK),
//	XLAT(PIDFD_THREAD),
};
WXLAT(pidfd_open_flags);

#endif /* PID_XLAT_H */
