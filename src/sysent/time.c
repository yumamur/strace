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

SYS_FUNC(adjtimex)
{
	if (exiting(*td))
	{
		if (current_abi == ABI_64BIT)
			printtimex64(td, td->sc_args[0]);
		else
			printtimex32(td, td->sc_args[0]);
	}

	return 0;
}

SYS_FUNC(settimeofday)
{
	FIRST_ARG("tv");
	printtimeval(td, td->sc_args[0]);

	NEXT_ARG("tz");
	printtimezone(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(time)
{
	if (exiting(*td))
	{
		FIRST_ARG("tloc");
		__kernel_time_t tloc;
		if (umovemem_or_printaddr(td, &tloc, td->sc_args[0], sizeof(tloc)))
			return SF_DECODE_COMPLETE;
		print_arg_start();
		PRINT_L(tloc);
		printtime(tloc, 0);
		print_arg_end();
	}
	return 0;
}

SYS_FUNC(timer_create)
{
	FIRST_ARG("which_clock");
	printflag(clock_ids, td->sc_args[0], "CLOCK_???");

	NEXT_ARG("timer_event_spec");
	// this project had its fair share of struct parsing.
	printaddr(td->sc_args[1]);

	NEXT_ARG("created_timer_id");
	printaddr(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

int decode_timer_settime(struct s_td *td)
{
	if (entering(*td))
	{
		NEXT_ARG("flags");
		printflags(timer_settime_flags, td->sc_args[1], "TIMER_???");

		NEXT_ARG("new_setting");
		printitimerval(td, td->sc_args[2]);
	}
	else
	{
		NEXT_ARG("old_setting");
		printitimerval(td, td->sc_args[3]);
	}
	return 0;
}

SYS_FUNC(timer_settime)
{
	if (entering(*td))
	{
		FIRST_ARG("timer_id");
		PRINT_LLU(td->sc_args[0]);
	}

	return decode_timer_settime(td);
}

SYS_FUNC(timer_gettime)
{
	if (entering(*td))
	{
		FIRST_ARG("timer_id");
		PRINT_LLU(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("setting");
		printitimerval(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(timer_getoverrun)
{
	FIRST_ARG("timer_id");
	PRINT_LLU(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(timer_delete)
{
	FIRST_ARG("timer_id");
	PRINT_LLU(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(timerfd_create)
{
	FIRST_ARG("clockid");
	printflag(clock_ids, td->sc_args[0], "CLOCK_???");

	NEXT_ARG("flags");
	printflags(timerfd_flags, td->sc_args[1], "TFD_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(timerfd_settime)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[1]);
	}

	return decode_timer_settime(td);
}

SYS_FUNC(timerfd_gettime)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("setting");
		printitimerval(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(clock_settime)
{
	FIRST_ARG("which_clock");
	printflag(clock_ids, td->sc_args[0], "CLOCK_???");

	NEXT_ARG("tp");
	printtimespec(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(clock_gettime)
{
	if (entering(*td))
	{
		FIRST_ARG("which_clock");
		printflag(clock_ids, td->sc_args[0], "CLOCK_???");
	}
	else
	{
		NEXT_ARG("tp");
		printtimespec(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(clock_nanosleep)
{
	if (entering(*td))
	{
		FIRST_ARG("which_clock");
		printflag(clock_ids, td->sc_args[0], "CLOCK_???");

		NEXT_ARG("flags");
		printflags(timer_settime_flags, td->sc_args[1], "TIMER_???");

		NEXT_ARG("request");
		printtimespec(td, td->sc_args[2]);
	}
	else
	{
		NEXT_ARG("remain");
		if (!td->sc_args[1] && is_error_erestart(td->sc_err))
			printtimespec(td, td->sc_args[3]);
		else
			printaddr(td->sc_args[3]);
	}

	return 0;
}

SYS_FUNC(clock_adjtime)
{
	FIRST_ARG("which_clock");
	printflag(clock_ids, td->sc_args[0], "CLOCK_???");

	NEXT_ARG("utx");
	printtimex(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}