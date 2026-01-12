#include "../ft_print.h"
#include "../ft_utils.h"
#include "time.xlat.h"

#include <string.h>
#include <time.h>

void printtime(time_t sec, unsigned long nsec)
{
	static char      buf[sizeof("2000-10-11T17:17:17.171717171+0300") + 5];

	size_t           pos = 0;
	const struct tm *tp = localtime(&sec);

	pos = strftime(buf, sizeof(buf), "%FT%T", tp);
	if (!pos)
		return;

	if (nsec)
		pos += snprintf(buf + pos, sizeof(buf) - pos, ".%09lu", nsec);
	strftime(buf + pos, sizeof(buf) - pos, "%z", tp);

	print_comment("%s", buf);
}

void printtimespec_struct(struct timespec *pt)
{
	print_struct_start();
	PRINT_MEMBER(pt, tv_sec, PRINT_LL);
	print_struct_member_sep();
	PRINT_MEMBER(pt, tv_nsec, PRINT_LL);
	print_struct_end();
}

void printtimeval_struct(struct timeval *pt)
{
	print_struct_start();
	PRINT_MEMBER(pt, tv_sec, PRINT_LL);
	print_struct_member_sep();
	PRINT_MEMBER(pt, tv_usec, PRINT_LL);
	print_struct_end();
}

void printitimerval_struct(struct itimerval *pt)
{
	print_struct_start();
	PRINT_MEMBER_ADDR(pt, it_interval, printtimeval_struct);
	print_struct_member_sep();
	PRINT_MEMBER_ADDR(pt, it_value, printtimeval_struct);
	print_struct_end();
}

void printitimerval(struct s_td *td, __kernel_ulong_t addr)
{
	struct itimerval buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) <= 0)
	{
		printaddr(addr);
		return;
	}
	printitimerval_struct(&buf);
}

void printtimeval(struct s_td *td, __kernel_ulong_t addr)
{
	struct timeval buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) <= 0)
	{
		printaddr(addr);
		return;
	}
	printtimeval_struct(&buf);
}

void printtimespec(struct s_td *td, __kernel_ulong_t addr)
{
	struct timespec buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) <= 0)
	{
		printaddr(addr);
		return;
	}
	printtimespec_struct(&buf);
}

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