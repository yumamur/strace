#ifndef SEM_XLAT_H
#define SEM_XLAT_H

#include "xlat.h"

#ifndef __USE_GNU
#  define __USE_GNU
#endif
#ifndef __USE_MISC
#  define __USE_MISC
#endif
#include <sys/sem.h>

const t_xlat_data semop_flags_data[] = {
	XLAT(IPC_NOWAIT),
	XLAT(SEM_UNDO),
};
WXLAT(semop_flags);

const t_xlat_data semctl_ops_data[] = {
	XLAT(IPC_RMID),
	XLAT(IPC_SET),
	XLAT(IPC_STAT),
	XLAT(IPC_INFO),
	XLAT(GETPID),
	XLAT(GETVAL),
	XLAT(GETALL),
	XLAT(GETNCNT),
	XLAT(GETZCNT),
	XLAT(SETVAL),
	XLAT(SETALL),
	XLAT(SEM_STAT),
	XLAT(SEM_INFO),
	XLAT(SEM_STAT_ANY),
};
WXLAT(semctl_ops);

const t_xlat_data sem_ipc_key_data[] = {
	XLAT(IPC_PRIVATE),
};
WXLAT(sem_ipc_key);

const t_xlat_data sem_ipc_mode_flags_data[] = {
	XLAT(IPC_CREAT),
	XLAT(IPC_EXCL),
	XLAT(IPC_NOWAIT),
};
WXLAT(sem_ipc_mode_flags);

#endif