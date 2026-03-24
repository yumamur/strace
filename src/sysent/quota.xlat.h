#ifndef QUOTA_XLAT_H
#define QUOTA_XLAT_H

#include "xlat.h"

#include <linux/dqblk_xfs.h>
#include <linux/quota.h>

const t_xlat_data quotactl_cmds_data[] = {
	XLAT(Q_SYNC),
	XLAT(Q_QUOTAON),
	XLAT(Q_QUOTAOFF),
	XLAT(Q_GETFMT),
	XLAT(Q_GETINFO),
	XLAT(Q_SETINFO),
	XLAT(Q_GETQUOTA),
	XLAT(Q_SETQUOTA),
	XLAT(Q_GETNEXTQUOTA),
	XLAT(Q_XQUOTAON),
	XLAT(Q_XQUOTAOFF),
	XLAT(Q_XGETQUOTA),
	XLAT(Q_XSETQLIM),
	XLAT(Q_XGETQSTAT),
	XLAT(Q_XQUOTARM),
	XLAT(Q_XQUOTASYNC),
	XLAT(Q_XGETQSTATV),
	XLAT(Q_XGETNEXTQUOTA),
};
WXLAT(quotactl_cmds);

const t_xlat_data quotactl_subcmds_data[] = {
	XLAT(USRQUOTA),
	XLAT(GRPQUOTA),
	XLAT(PRJQUOTA),
};
WXLAT(quotactl_subcmds);

#endif /* QUOTA_XLAT_H */