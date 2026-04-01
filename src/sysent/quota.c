#include "../ft_print.h"
#include "quota.xlat.h"

SYS_FUNC(quotactl)
{
	FIRST_ARG("cmd");
	uint32_t cmd = td->sc_args[0];
	uint32_t subcmd = cmd & SUBCMDMASK;
	prints("QCMD");
	print_arg_start();
	printflag(quotactl_cmds, cmd, "Q_???");
	print_arg_sep();
	printflag(quotactl_subcmds, subcmd, "???QUOTA");
	print_arg_end();

	NEXT_ARG("special");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("id");
	PRINT_LU(td->sc_args[2]);

	// guess who'll ignore 200+ lines of if..else and just print the address
	NEXT_ARG("addr");
	printaddr(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(quotactl_fd)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("cmd");
	uint32_t cmd = td->sc_args[1];
	uint32_t subcmd = cmd & SUBCMDMASK;
	prints("QCMD");
	print_arg_start();
	printflag(quotactl_cmds, cmd, "Q_???");
	print_arg_sep();
	printflag(quotactl_subcmds, subcmd, "???QUOTA");
	print_arg_end();

	NEXT_ARG("id");
	PRINT_LU(td->sc_args[2]);

	NEXT_ARG("addr");
	printaddr(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}