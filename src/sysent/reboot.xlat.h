#ifndef REBOOT_XLAT_H
#define REBOOT_XLAT_H

#include "xlat.h"

#include <linux/reboot.h>
#include <sys/reboot.h>

const t_xlat_data reboot_magic1_data[] = {
	XLAT(LINUX_REBOOT_MAGIC1),
};
WXLAT(reboot_magic1);

const t_xlat_data reboot_magic2_data[] = {
	XLAT(LINUX_REBOOT_MAGIC2),
	XLAT(LINUX_REBOOT_MAGIC2A),
	XLAT(LINUX_REBOOT_MAGIC2B),
	XLAT(LINUX_REBOOT_MAGIC2C),
};
WXLAT(reboot_magic2);

const t_xlat_data reboot_cmds_data[] = {
	XLAT(LINUX_REBOOT_CMD_RESTART),
	XLAT(LINUX_REBOOT_CMD_HALT),
	XLAT(LINUX_REBOOT_CMD_CAD_ON),
	XLAT(LINUX_REBOOT_CMD_CAD_OFF),
	XLAT(LINUX_REBOOT_CMD_POWER_OFF),
	XLAT(LINUX_REBOOT_CMD_RESTART2),
	XLAT(LINUX_REBOOT_CMD_SW_SUSPEND),
	XLAT(LINUX_REBOOT_CMD_KEXEC),
};
WXLAT(reboot_cmds);

#endif /* REBOOT_XLAT_H */