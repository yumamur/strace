#include "ft_common.h"
#include "ft_print.h"
#include "ft_utils.h"

#include <linux/time.h>

#include <string.h>

const char *sprinttime(unsigned long sec, unsigned long nsec)
{
	static char      buf[sizeof("1970-01-01T00:00:00.123456789+0300") + 5];

	size_t           pos = 0;
	time_t           local = (time_t) sec;
	const struct tm *tp = localtime(&local);

	pos = strftime(buf, sizeof(buf), "%FT%T", tp);
	if (!pos)
		return NULL;

	if (nsec)
		pos += snprintf(buf + pos, sizeof(buf) - pos, ".%09lu", nsec);
	strftime(buf + pos, sizeof(buf) - pos, "%z", tp);

	return buf;
}

void printtime(unsigned long sec, unsigned long nsec)
{
	const char *s = sprinttime(sec, nsec);
	if (s)
		print_comment("%s", s);
}

void printtimespec_struct(struct timespec *pt)
{
	print_struct_start();
	PRINT_MEMBER(*pt, tv_sec, PRINT_LL);
	print_struct_member_sep();
	PRINT_MEMBER(*pt, tv_nsec, PRINT_LL);
	print_struct_end();
}

void printtimeval_struct(struct timeval *pt)
{
	print_struct_start();
	PRINT_MEMBER(*pt, tv_sec, PRINT_LL);
	print_struct_member_sep();
	PRINT_MEMBER(*pt, tv_usec, PRINT_LL);
	print_struct_end();
}

void printitimerval_struct(struct itimerval *pt)
{
	print_struct_start();
	PRINT_MEMBER_ADDR(*pt, it_interval, printtimeval_struct);
	print_struct_member_sep();
	PRINT_MEMBER_ADDR(*pt, it_value, printtimeval_struct);
	print_struct_end();
}

void printtimezone_struct(struct timezone *pt)
{
	print_struct_start();
	PRINT_MEMBER(*pt, tz_minuteswest, PRINT_D);
	print_struct_member_sep();
	PRINT_MEMBER(*pt, tz_dsttime, PRINT_D);
	print_struct_end();
}

void printitimerval(struct s_td *td, __kernel_ulong_t addr)
{
	struct itimerval buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
	{
		printaddr(addr);
		return;
	}
	printitimerval_struct(&buf);
}

void printtimeval(struct s_td *td, __kernel_ulong_t addr)
{
	struct timeval buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
	{
		printaddr(addr);
		return;
	}
	printtimeval_struct(&buf);
}

void printtimespec(struct s_td *td, __kernel_ulong_t addr)
{
	struct timespec buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
	{
		printaddr(addr);
		return;
	}
	printtimespec_struct(&buf);
}

void printtimezone(struct s_td *td, __kernel_ulong_t addr)
{
	struct timezone buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
	{
		printaddr(addr);
		return;
	}
	printtimezone_struct(&buf);
}

#define timespec_printsize  sizeof("{tv_sec=18446744073709551615, tv_nsec=18446744073709551615}")
#define timeval_printsize   timespec_printsize
#define itimerval_printsize sizeof("{it_interval={}, it_value}") + timeval_printsize * 2
#define timezone_printsize  sizeof("{tz_minuteswest=32767, tz_dsttime=32767}")

const char *sprinttimespec_struct(struct timespec *pt)
{
	static char buf[timespec_printsize];
	inprint(buf, "{tv_sec=%lld, tv_nsec=%lld}", (long long) pt->tv_sec, (long long) pt->tv_nsec);
	return buf;
}

const char *sprinttimeval_struct(struct timeval *pt)
{
	static char buf[timeval_printsize];
	inprint(buf, "{tv_sec=%lld, tv_usec=%lld}", (long long) pt->tv_sec, (long long) pt->tv_usec);
	return buf;
}

const char *sprintitimerval_struct(struct itimerval *pt)
{
	static char buf[itimerval_printsize];
	inprint(buf, "{it_interval=%s, it_value=%s}",
			sprinttimeval_struct(&pt->it_interval), sprinttimeval_struct(&pt->it_value));
	return buf;
}

const char *sprinttimezone_struct(struct timezone *pt)
{
	static char buf[timezone_printsize];
	inprint(buf, "{tz_minuteswest=%d, tz_dsttime=%d}", pt->tz_minuteswest, pt->tz_dsttime);
	return buf;
}

const char *sprintitimerval(struct s_td *td, __kernel_ulong_t addr)
{
	struct itimerval buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
		return sprintaddr(addr);
	return sprintitimerval_struct(&buf);
}

const char *sprinttimeval(struct s_td *td, __kernel_ulong_t addr)
{
	struct timeval buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
		return sprintaddr(addr);
	return sprinttimeval_struct(&buf);
}

const char *sprinttimespec(struct s_td *td, __kernel_ulong_t addr)
{
	struct timespec buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
		return sprintaddr(addr);
	return sprinttimespec_struct(&buf);
}

const char *sprinttimezone(struct s_td *td, __kernel_ulong_t addr)
{
	struct timezone buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
		return sprintaddr(addr);
	return sprinttimezone_struct(&buf);
}