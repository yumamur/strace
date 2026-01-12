#ifndef SHM_XLAT_H
#define SHM_XLAT_H

#include "xlat.h"

#include <linux/shm.h>

const t_xlat_data ipc_key_data[] = {XLAT(IPC_PRIVATE)};
WXLAT(ipc_key);

const t_xlat_data shm_resource_flags_data[] = {
	XLAT(IPC_CREAT),
	XLAT(IPC_EXCL),
	XLAT(SHM_HUGETLB),
	XLAT(SHM_NORESERVE),
};
WXLAT(shm_resource_flags);

const t_xlat_data shmctl_ops_data[] = {
	XLAT(IPC_RMID),
	XLAT(IPC_SET),
	XLAT(IPC_STAT),
	XLAT(IPC_INFO),
	XLAT(SHM_LOCK),
	XLAT(SHM_UNLOCK),
	XLAT(SHM_STAT),
	XLAT(SHM_INFO),
	XLAT(SHM_STAT_ANY),
};
WXLAT(shmctl_ops);

#endif