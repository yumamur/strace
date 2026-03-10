#include "../ft_print.h"
#include "../ft_utils.h"
#include "printk.xlat.h"

SYS_FUNC(syslog)
{
	__kernel_ulong_t type = td->sc_args[0];

	if (entering(*td))
	{
		FIRST_ARG("type");
		printflag_indexed(syslog_actions, type, "SYSLOG_ACTION_???");

		switch (type)
		{
		case SYSLOG_ACTION_CLOSE:
		case SYSLOG_ACTION_OPEN:
		case SYSLOG_ACTION_CLEAR:
		case SYSLOG_ACTION_CONSOLE_OFF:
		case SYSLOG_ACTION_CONSOLE_ON:
		case SYSLOG_ACTION_SIZE_UNREAD:
		case SYSLOG_ACTION_SIZE_BUFFER:
			break;
		case SYSLOG_ACTION_READ:
		case SYSLOG_ACTION_READ_ALL:
		case SYSLOG_ACTION_READ_CLEAR:
			return 0;
		case SYSLOG_ACTION_CONSOLE_LEVEL:
			NEXT_ARG("bufp");
			printaddr(td->sc_args[1]);

			NEXT_ARG("len");
			printflag_indexed(syslog_levels, td->sc_args[2], "LOGLEVEL_???");
			break;
		default:
			NEXT_ARG("bufp");
			printaddr(td->sc_args[1]);

			NEXT_ARG("len");
			PRINT_D(td->sc_args[2]);
		}
		return SF_DECODE_COMPLETE;
	}
	else
	{
		NEXT_ARG("bufp");
		if (td->sc_err)
			printaddr(td->sc_args[1]);
		else
			printnstr(td, td->sc_args[1], td->sc_ret);

		NEXT_ARG("len");
		PRINT_D(td->sc_args[2]);

		return SF_DECODE_COMPLETE;
	}
}