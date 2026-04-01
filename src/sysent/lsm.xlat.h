#ifndef LSM_XLAT_H
#define LSM_XLAT_H

#include "xlat.h"

#include <linux/lsm.h>

const t_xlat_data lsm_flags_data[] = {
	XLAT(LSM_FLAG_SINGLE),
};
WXLAT(lsm_flags);

const t_xlat_data lsm_attributes_data[] = {
	XLAT(LSM_ATTR_UNDEF),
	XLAT(LSM_ATTR_CURRENT),
	XLAT(LSM_ATTR_EXEC),
	XLAT(LSM_ATTR_FSCREATE),
	XLAT(LSM_ATTR_KEYCREATE),
	XLAT(LSM_ATTR_PREV),
	XLAT(LSM_ATTR_SOCKCREATE),
};
WXLAT(lsm_attributes);

const t_xlat_data lsm_ids_data[] = {
	XLAT(LSM_ID_UNDEF),
	XLAT(LSM_ID_CAPABILITY),
	XLAT(LSM_ID_SELINUX),
	XLAT(LSM_ID_SMACK),
	XLAT(LSM_ID_TOMOYO),
	XLAT(LSM_ID_APPARMOR),
	XLAT(LSM_ID_YAMA),
	XLAT(LSM_ID_LOADPIN),
	XLAT(LSM_ID_SAFESETID),
	XLAT(LSM_ID_LOCKDOWN),
	XLAT(LSM_ID_BPF),
	XLAT(LSM_ID_LANDLOCK),
	XLAT(LSM_ID_IMA),
	XLAT(LSM_ID_EVM),
	XLAT(LSM_ID_IPE),
};
WXLAT(lsm_ids);

#endif /* LSM_XLAT_H */