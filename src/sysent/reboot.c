#include "../ft_print.h"
#include "../ft_common.h"
#include "reboot.xlat.h"

SYS_FUNC(reboot)
{
	FIRST_ARG("magic1");
	printflag(reboot_magic1, td->sc_args[0], "LINUX_REBOOT_MAGIC1???");

	NEXT_ARG("magic2");
	printflag(reboot_magic2, td->sc_args[1], "LINUX_REBOOT_MAGIC2???");

	NEXT_ARG("cmd");
	printflag(reboot_cmds, td->sc_args[2], "LINUX_REBOOT_CMD_???");

	if (td->sc_args[2] == LINUX_REBOOT_CMD_RESTART2)
	{
		NEXT_ARG("arg");
		printnstr(td, td->sc_args[3], 255);
	}

	return SF_DECODE_COMPLETE;
}
