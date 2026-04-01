#ifndef IPC_XLAT_H
#define IPC_XLAT_H

#include "xlat.h"

#ifndef __USE_GNU
#  define __USE_GNU
#endif
#ifndef __USE_MISC
#  define __USE_MISC
#endif
#include <linux/ipc.h>

const t_xlat_data ipc_calls_data[] = {
	XLAT(SEMOP),
	XLAT(SEMGET),
	XLAT(SEMCTL),
	XLAT(SEMTIMEDOP),
	XLAT(MSGSND),
	XLAT(MSGRCV),
	XLAT(MSGGET),
	XLAT(MSGCTL),
	XLAT(SHMAT),
	XLAT(SHMDT),
	XLAT(SHMGET),
	XLAT(SHMCTL),
	XLAT(DIPC),
};
WXLAT(ipc_calls);

const t_xlat_data ipc_versions_data[] = {
	XLAT(IPC_OLD),
	XLAT(IPC_64),
};
WXLAT(ipc_versions);

const t_xlat_data ipc_key_data[] = {
	XLAT(IPC_PRIVATE),
};
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

#endif /* IPC_XLAT_H */
