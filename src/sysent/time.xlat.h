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

const t_xlat_data clock_ids_data[] = {
	XLAT(CLOCK_REALTIME),
	XLAT(CLOCK_MONOTONIC),
	XLAT(CLOCK_PROCESS_CPUTIME_ID),
	XLAT(CLOCK_THREAD_CPUTIME_ID),
	XLAT(CLOCK_MONOTONIC_RAW),
	XLAT(CLOCK_REALTIME_COARSE),
	XLAT(CLOCK_MONOTONIC_COARSE),
	XLAT(CLOCK_BOOTTIME),
	XLAT(CLOCK_REALTIME_ALARM),
	XLAT(CLOCK_BOOTTIME_ALARM),
	XLAT(CLOCK_SGI_CYCLE),
	XLAT(CLOCK_TAI),
};
WXLAT(clock_ids);

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