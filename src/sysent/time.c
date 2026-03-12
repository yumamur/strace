#include "../ft_print.h"
#include "../ft_utils.h"
#include "time.xlat.h"

SYS_FUNC(nanosleep)
{
	if (entering(*td))
	{
		FIRST_ARG("duration");
		printtimespec(td, td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("rem");
		if (is_error_erestart(td->sc_err))
			printtimespec(td, td->sc_args[1]);
		else
			printaddr(td->sc_args[1]);
	}
	return 0;
}

SYS_FUNC(getitimer)
{
	if (entering(*td))
	{
		FIRST_ARG("which");
		printflag(timer_which, td->sc_args[0], "ITIMER_???");
	}
	else
	{
		NEXT_ARG("curr_value");
		printitimerval(td, td->sc_args[1]);
	}
	return 0;
}

SYS_FUNC(alarm)
{
	FIRST_ARG("seconds");
	PRINT_U(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setitimer)
{
	if (entering(*td))
	{
		FIRST_ARG("which");
		printflag(timer_which, td->sc_args[0], "ITIMER_???");

		NEXT_ARG("new_value");
		printitimerval(td, td->sc_args[1]);
	}
	else
	{
		NEXT_ARG("old_value");
		printitimerval(td, td->sc_args[2]);
	}
	return 0;
}

SYS_FUNC(gettimeofday)
{
	if (exiting(*td))
	{
		FIRST_ARG("tv");
		printtimeval(td, td->sc_args[0]);

		NEXT_ARG("tz");
		printtimezone(td, td->sc_args[1]);

		return SF_DECODE_COMPLETE;
	}
	return 0;
}