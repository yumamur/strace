#include "xlat.h"

#ifndef __USE_GNU
#  define __USE_GNU
#endif
#ifndef __USE_MISC
#  define __USE_MISC
#endif
#include <sys/ipc.h>

const t_xlat_data ipc_key_data[] = {XLAT(IPC_PRIVATE)};
WXLAT(ipc_key);

const t_xlat_data ipc_mode_flags_data[] = {
	XLAT(IPC_CREAT),
	XLAT(IPC_EXCL),
	XLAT(IPC_NOWAIT),
};
WXLAT(ipc_mode_flags);

const t_xlat_data ipc_cmd_flags_data[] = {
	XLAT(IPC_RMID),
	XLAT(IPC_SET),
	XLAT(IPC_STAT),
	XLAT(IPC_INFO),
};
WXLAT(ipc_cmd_flags);