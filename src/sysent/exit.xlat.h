#ifndef EXIT_XLAT_H
#define EXIT_XLAT_H

#include "xlat.h"

#define __USE_XOPEN_EXTENDED
#include <sys/wait.h>

const t_xlat_data wait4_options_data[] = {
	XLAT(WNOHANG),
	XLAT(WUNTRACED),
	XLAT(WSTOPPED),
	XLAT(WEXITED),
	XLAT(WCONTINUED),
	XLAT(WNOWAIT),
	XLAT_NAMED(__WNOTHREAD, "WNOTHREAD"),
	XLAT_NAMED(__WALL, "WALL"),
	XLAT_NAMED(__WCLONE, "WCLONE"),
};
WXLAT(wait4_options);

const t_xlat_data waitid_idtypes_data[] = {
	XLAT(P_ALL),
	XLAT(P_PID),
	XLAT(P_PGID),
	XLAT(P_PIDFD),
};
WIXLAT(waitid_idtypes);

#endif