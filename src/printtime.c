#include "ft_print.h"
#include "ft_utils.h"
#include "sysent/xlat.h"
#include <linux/time.h>
#include <string.h>

static const char *clock_ids[] = {
	XLAT_INDEXED(CLOCK_REALTIME),
	XLAT_INDEXED(CLOCK_MONOTONIC),
	XLAT_INDEXED(CLOCK_PROCESS_CPUTIME_ID),
	XLAT_INDEXED(CLOCK_THREAD_CPUTIME_ID),
	XLAT_INDEXED(CLOCK_MONOTONIC_RAW),
	XLAT_INDEXED(CLOCK_REALTIME_COARSE),
	XLAT_INDEXED(CLOCK_MONOTONIC_COARSE),
	XLAT_INDEXED(CLOCK_BOOTTIME),
	XLAT_INDEXED(CLOCK_REALTIME_ALARM),
	XLAT_INDEXED(CLOCK_BOOTTIME_ALARM),
	XLAT_INDEXED(CLOCK_SGI_CYCLE),
	XLAT_INDEXED(CLOCK_TAI),
};

#define STRUCT_TIMESPEC       t_struct_timespec32
#define PRINTTIMESPEC         printtimespec32
#define PRINTTIMESPEC_STRUCT  printtimespec32_struct
#define SPRINTTIMESPEC        sprinttimespec32
#define SPRINTTIMESPEC_STRUCT sprinttimespec32_struct
#define STRUCT_ITIMERSPEC     t_struct_itimerspec32
#define PRINTITIMERSPEC       printitimerspec32
#define PRECISION_FIELD       tv_nsec
#define PRINTTIMESPEC_UTIMES  printtimespec_utimes32
#include "printtimespec.h"
#undef STRUCT_TIMESPEC
#undef PRINTTIMESPEC
#undef PRINTTIMESPEC_STRUCT
#undef SPRINTTIMESPEC
#undef SPRINTTIMESPEC_STRUCT
#undef STRUCT_ITIMERSPEC
#undef PRINTITIMERSPEC
#undef PRECISION_FIELD
#undef PRINTTIMESPEC_UTIMES

#define STRUCT_TIMESPEC       t_struct_timespec64
#define PRINTTIMESPEC         printtimespec64
#define PRINTTIMESPEC_STRUCT  printtimespec64_struct
#define SPRINTTIMESPEC        sprinttimespec64
#define SPRINTTIMESPEC_STRUCT sprinttimespec64_struct
#define STRUCT_ITIMERSPEC     t_struct_itimerspec64
#define PRINTITIMERSPEC       printitimerspec64
#define PRECISION_FIELD       tv_nsec
#define PRINTTIMESPEC_UTIMES  printtimespec_utimes64
#include "printtimespec.h"
#undef STRUCT_TIMESPEC
#undef PRINTTIMESPEC
#undef PRINTTIMESPEC_STRUCT
#undef SPRINTTIMESPEC
#undef SPRINTTIMESPEC_STRUCT
#undef STRUCT_ITIMERSPEC
#undef PRINTITIMERSPEC
#undef PRECISION_FIELD
#undef PRINTTIMESPEC_UTIMES

#define STRUCT_TIMESPEC       t_struct_timeval32
#define PRINTTIMESPEC         printtimeval32
#define PRINTTIMESPEC_STRUCT  printtimeval32_struct
#define SPRINTTIMESPEC        sprinttimeval32
#define SPRINTTIMESPEC_STRUCT sprinttimeval32_struct
#define STRUCT_ITIMERSPEC     t_struct_itimerval32
#define PRINTITIMERSPEC       printitimerval32
#define PRECISION_FIELD       tv_usec
#define PRINTTIMESPEC_UTIMES  printtimeval_utimes32
#include "printtimespec.h"
#undef STRUCT_TIMESPEC
#undef PRINTTIMESPEC
#undef PRINTTIMESPEC_STRUCT
#undef SPRINTTIMESPEC
#undef SPRINTTIMESPEC_STRUCT
#undef STRUCT_ITIMERSPEC
#undef PRINTITIMERSPEC
#undef PRECISION_FIELD
#undef PRINTTIMESPEC_UTIMES

#define STRUCT_TIMESPEC       t_struct_timeval64
#define PRINTTIMESPEC         printtimeval64
#define PRINTTIMESPEC_STRUCT  printtimeval64_struct
#define SPRINTTIMESPEC        sprinttimeval64
#define SPRINTTIMESPEC_STRUCT sprinttimeval64_struct
#define STRUCT_ITIMERSPEC     t_struct_itimerval64
#define PRINTITIMERSPEC       printitimerval64
#define PRECISION_FIELD       tv_usec
#define PRINTTIMESPEC_UTIMES  printtimeval_utimes64
#include "printtimespec.h"
#undef STRUCT_TIMESPEC
#undef PRINTTIMESPEC
#undef PRINTTIMESPEC_STRUCT
#undef SPRINTTIMESPEC
#undef SPRINTTIMESPEC_STRUCT
#undef STRUCT_ITIMERSPEC
#undef PRINTITIMERSPEC
#undef PRECISION_FIELD
#undef PRINTTIMESPEC_UTIMES

#define timeval_printsize   sizeof("{tv_sec=18446744073709551615, tv_usec=18446744073709551615}")
#define itimerval_printsize sizeof("{it_interval={}, it_value={}}") + timeval_printsize * 2
#define PRINTITIMERVAL      printitimerval64

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

void printtimezone_struct(struct timezone *pt)
{
	print_struct_start();
	PRINT_MEMBER(*pt, tz_minuteswest, PRINT_D);
	print_struct_member_sep();
	PRINT_MEMBER(*pt, tz_dsttime, PRINT_D);
	print_struct_end();
}

void printutimbuf_struct(struct utimbuf *pt)
{
	print_struct_start();
	PRINT_MEMBER_D(*pt, actime);
	printtime(pt->actime, 0);
	PRINT_MEMBER_D(*pt, modtime);
	printtime(pt->modtime, 0);
	print_struct_end();
}

void printtimezone(struct s_td *td, __kernel_ulong_t addr)
{
	struct timezone buf = {};
	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;
	printtimezone_struct(&buf);
}

void printutimbuf(struct s_td *td, __kernel_ulong_t addr)
{
	struct utimbuf buf = {};

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;
	printutimbuf_struct(&buf);
}

// void printutimbuf_utimes(struct s_td *td, __kernel_ulong_t addr)
// {
// 	struct utimbuf buf[2];

// 	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
// 		return;
// 	print_arr_start();
// 	printutimbuf_struct(&buf[0]);
// 	print_arr_sep();
// 	printutimbuf_struct(&buf[1]);
// 	print_arr_end();
// }

const char *sprinttimezone_struct(struct timezone *pt)
{
	static char buf[sizeof("{tz_minuteswest=32767, tz_dsttime=32767}")];
	inprint(buf, "{tz_minuteswest=%d, tz_dsttime=%d}", pt->tz_minuteswest, pt->tz_dsttime);
	return buf;
}

const char *sprinttimezone(struct s_td *td, __kernel_ulong_t addr)
{
	struct timezone buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
		return sprintaddr(addr);
	return sprinttimezone_struct(&buf);
}

void print_clock_id(int clockid)
{
	printflag_indexed(clock_ids, clockid, "CLOCK_???");
}