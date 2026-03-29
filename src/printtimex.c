#include "ft_print.h"
#include "ft_utils.h"
#include "sysent/xlat.h"
#include <linux/timex.h>

const t_xlat_data adjtimex_modes_data[] = {
	XLAT(ADJ_OFFSET),
	XLAT(ADJ_FREQUENCY),
	XLAT(ADJ_MAXERROR),
	XLAT(ADJ_ESTERROR),
	XLAT(ADJ_STATUS),
	XLAT(ADJ_TIMECONST),
	XLAT(ADJ_TAI),
	XLAT(ADJ_SETOFFSET),
	XLAT(ADJ_MICRO),
	XLAT(ADJ_NANO),
	XLAT(ADJ_TICK),
};
WXLAT(adjtimex_modes);

const t_xlat_data adjtimex_status_data[] = {
	XLAT(STA_PLL),
	XLAT(STA_PPSFREQ),
	XLAT(STA_PPSTIME),
	XLAT(STA_FLL),
	XLAT(STA_INS),
	XLAT(STA_DEL),
	XLAT(STA_UNSYNC),
	XLAT(STA_FREQHOLD),
	XLAT(STA_PPSSIGNAL),
	XLAT(STA_PPSJITTER),
	XLAT(STA_PPSWANDER),
	XLAT(STA_PPSERROR),
	XLAT(STA_CLOCKERR),
	XLAT(STA_NANO),
	XLAT(STA_MODE),
	XLAT(STA_CLK),
};
WXLAT(adjtimex_status);

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