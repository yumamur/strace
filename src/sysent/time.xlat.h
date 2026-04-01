#ifndef STAT_XLAT_H
#define STAT_XLAT_H

#include "xlat.h"

#include <linux/time.h>
#include <linux/timerfd.h>

const t_xlat_data timer_which_data[] = {
	XLAT(ITIMER_REAL),
	XLAT(ITIMER_VIRTUAL),
	XLAT(ITIMER_PROF),
};
WXLAT(timer_which);

const t_xlat_data timer_settime_flags_data[] = {
	XLAT(TIMER_ABSTIME),
};
WXLAT(timer_settime_flags);

const t_xlat_data timerfd_flags_data[] = {
	XLAT(TFD_TIMER_ABSTIME),
	XLAT(TFD_TIMER_CANCEL_ON_SET),
	XLAT(TFD_CLOEXEC),
	XLAT(TFD_NONBLOCK),
};
WXLAT(timerfd_flags);

#endif