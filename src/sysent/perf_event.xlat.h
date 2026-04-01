#ifndef PERF_EVENT_XLAT_H
#define PERF_EVENT_XLAT_H

#include "xlat.h"

#include <linux/perf_event.h>

const t_xlat_data perf_event_types_data[] = {
	XLAT(PERF_TYPE_HARDWARE),
	XLAT(PERF_TYPE_SOFTWARE),
	XLAT(PERF_TYPE_TRACEPOINT),
	XLAT(PERF_TYPE_HW_CACHE),
	XLAT(PERF_TYPE_RAW),
	XLAT(PERF_TYPE_BREAKPOINT),
};
WXLAT(perf_event_types);

const t_xlat_data perf_attr_size_versions_data[] = {
	XLAT(PERF_ATTR_SIZE_VER0),
	XLAT(PERF_ATTR_SIZE_VER1),
	XLAT(PERF_ATTR_SIZE_VER2),
	XLAT(PERF_ATTR_SIZE_VER3),
	XLAT(PERF_ATTR_SIZE_VER4),
	XLAT(PERF_ATTR_SIZE_VER5),
	XLAT(PERF_ATTR_SIZE_VER6),
	XLAT(PERF_ATTR_SIZE_VER7),
	XLAT(PERF_ATTR_SIZE_VER8),
//	XLAT(PERF_ATTR_SIZE_VER9),
};
WXLAT(perf_attr_size_versions);

const t_xlat_data perf_flags_data[] = {
	XLAT(PERF_FLAG_FD_NO_GROUP),
	XLAT(PERF_FLAG_FD_OUTPUT),
	XLAT(PERF_FLAG_PID_CGROUP),
	XLAT(PERF_FLAG_FD_CLOEXEC),
};
WXLAT(perf_flags);

#endif /* PERF_EVENT_XLAT_H */
