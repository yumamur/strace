#include "ft_print.h"
#include "ft_utils.h"
#include "sysent/timex.xlat.h"
#include <linux/timex.h>

/* approach is almost identical to strace/print_timex.c,
 * since it seemed feasible and with as less boilerplate as possible.
 */

#define PRINTTIMEX_FNAME      printtimex64
#define PRINTTIMEX_TIME_FNAME printtimex64_time
#define STRUCT_TIMEX          struct timex
#include "printtimex.h"
#undef PRINTTIMEX_FNAME
#undef PRINTTIMEX_TIME_FNAME
#undef STRUCT_TIMEX

typedef struct
{
		unsigned int modes;
		int          offset;
		int          freq;
		int          maxerror;
		int          esterror;
		int          status;
		int          constant;
		int          precision;
		int          tolerance;
		struct
		{
				int tv_sec;
				int tv_usec;
		} time;
		int tick;
		int ppsfreq;
		int jitter;
		int shift;
		int stabil;
		int jitcnt;
		int calcnt;
		int errcnt;
		int stbcnt;
		int tai;
		int pad0[11];
} timex_abi32_t;

#define PRINTTIMEX_FNAME      printtimex32
#define PRINTTIMEX_TIME_FNAME printtimex32_time
#define STRUCT_TIMEX          timex_abi32_t
#include "printtimex.h"
#undef PRINTTIMEX_FNAME
#undef PRINTTIMEX_TIME_FNAME
#undef STRUCT_TIMEX