#ifndef SCHED_XLAT_H
#define SCHED_XLAT_H

#include "xlat.h"

#include <sched.h>

#ifndef SCHED_NORMAL
#  define SCHED_NORMAL 0
#endif
#ifndef SCHED_FIFO
#  define SCHED_FIFO 1
#endif
#ifndef SCHED_RR
#  define SCHED_RR 2
#endif
#ifndef SCHED_BATCH
#  define SCHED_BATCH 3
#endif
#ifndef SCHED_ISO
#  define SCHED_ISO 4
#endif
#ifndef SCHED_IDLE
#  define SCHED_IDLE 5
#endif
#ifndef SCHED_DEADLINE
#  define SCHED_DEADLINE 6
#endif
#ifndef SCHED_EXT
#  define SCHED_EXT 7
#endif
#ifndef SCHED_RESET_ON_FORK
#  define SCHED_RESET_ON_FORK 0x40000000
#endif

const char *scheduler_policies[] = {
	XLAT_INDEXED(SCHED_NORMAL),
	XLAT_INDEXED(SCHED_FIFO),
	XLAT_INDEXED(SCHED_RR),
	XLAT_INDEXED(SCHED_BATCH),
	XLAT_INDEXED(SCHED_ISO),
	XLAT_INDEXED(SCHED_IDLE),
	XLAT_INDEXED(SCHED_DEADLINE),
	XLAT_INDEXED(SCHED_EXT),
};

#endif /* SCHED_XLAT_H */