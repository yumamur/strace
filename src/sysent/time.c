#include "../ft_print.h"
#include "../ft_utils.h"
#include "time.xlat.h"

#define printtimespec current_abi == ABI_64BIT ? printtimespec64 : printtimespec32
#define printtimeval  current_abi == ABI_64BIT ? printtimeval64 : printtimeval32

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
		printtimex(td, td->sc_args[0]);

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
	print_clock_id(td->sc_args[0]);

	NEXT_ARG("timer_event_spec");
	// this project had its fair share of struct parsing.
	printaddr(td->sc_args[1]);

	NEXT_ARG("created_timer_id");
	printaddr(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

int do_timer_settime_flags_settings(struct s_td *td, typeof(printitimerval32) printitimerval_fn)
{
	if (entering(*td))
	{
		NEXT_ARG("flags");
		printflags(timer_settime_flags, td->sc_args[1], "TIMER_???");

		NEXT_ARG("new_setting");
		printitimerval_fn(td, td->sc_args[2]);
	}
	else
	{
		NEXT_ARG("old_setting");
		printitimerval_fn(td, td->sc_args[3]);
	}
	return 0;
}

int do_timer_settime(struct s_td *td, typeof(printitimerval32) printitimerval_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("timer_id");
		PRINT_LLU(td->sc_args[0]);
	}

	return do_timer_settime_flags_settings(td, printitimerval_fn);
}

SYS_FUNC(timer_settime32)
{
	return do_timer_settime(td, printitimerval32);
}

SYS_FUNC(timer_settime64)
{
	return do_timer_settime(td, printitimerval64);
}

void do_timer_gettime(struct s_td *td)
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
}

SYS_FUNC(timer_gettime32)
{
	do_timer_gettime(td);
	return 0;
}

SYS_FUNC(timer_gettime64)
{
	do_timer_gettime(td);
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
	print_clock_id(td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(timerfd_flags, td->sc_args[1], "TFD_???");

	return SF_DECODE_COMPLETE;
}

int do_timerfd_settime(struct s_td *td, typeof(printitimerval32) printitimerval_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[1]);
	}

	return do_timer_settime_flags_settings(td, printitimerval_fn);
}

SYS_FUNC(timerfd_settime32)
{
	return do_timerfd_settime(td, printitimerval32);
}

SYS_FUNC(timerfd_settime64)
{
	return do_timerfd_settime(td, printitimerval64);
}

int do_timerfd_gettime(struct s_td *td, typeof(printitimerval32) printitimerval_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("setting");
		printitimerval_fn(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(timerfd_gettime32)
{
	return do_timerfd_gettime(td, printitimerval32);
}

SYS_FUNC(timerfd_gettime64)
{
	return do_timerfd_gettime(td, printitimerval64);
}

int do_clock_settime(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	FIRST_ARG("which_clock");
	print_clock_id(td->sc_args[0]);

	NEXT_ARG("tp");
	printtimespec_fn(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(clock_settime32)
{
	return do_clock_settime(td, printtimespec32);
}

SYS_FUNC(clock_settime64)
{
	return do_clock_settime(td, printtimespec64);
}

int do_clock_gettime(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("which_clock");
		print_clock_id(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("tp");
		printtimespec_fn(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(clock_gettime32)
{
	return do_clock_gettime(td, printtimespec32);
}

SYS_FUNC(clock_gettime64)
{
	return do_clock_gettime(td, printtimespec64);
}

void do_clock_nanosleep(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("which_clock");
		print_clock_id(td->sc_args[0]);

		NEXT_ARG("flags");
		printflags(timer_settime_flags, td->sc_args[1], "TIMER_???");

		NEXT_ARG("request");
		printtimespec_fn(td, td->sc_args[2]);
	}
	else
	{
		NEXT_ARG("remain");
		if (!td->sc_args[1] && is_error_erestart(td->sc_err))
			printtimespec_fn(td, td->sc_args[3]);
		else
			printaddr(td->sc_args[3]);
	}
}

SYS_FUNC(clock_nanosleep_time32)
{
	do_clock_nanosleep(td, printtimespec32);
	return 0;
}

SYS_FUNC(clock_nanosleep_time64)
{
	do_clock_nanosleep(td, printtimespec64);
	return 0;
}

int do_clock_adjtime(struct s_td *td, typeof(printtimex) printtimex_fn)
{
	FIRST_ARG("which_clock");
	print_clock_id(td->sc_args[0]);

	NEXT_ARG("utx");
	printtimex_fn(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(clock_adjtime32)
{
	return do_clock_adjtime(td, printtimex32);
}

SYS_FUNC(clock_adjtime64)
{
	return do_clock_adjtime(td, printtimex64);
}