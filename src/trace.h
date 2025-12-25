#ifndef FT_TRACE
#define FT_TRACE

#include "ft_common.h"
#include <linux/posix_types.h>
#include <time.h>

#define TD_INSYSCALL   0x1
#define TD_OUT_SUMMARY 0x2

#define entering(tcp) (!((tcp).flags & TD_INSYSCALL))
#define exiting(tcp)  ((tcp).flags & TD_INSYSCALL)

#define MAX_ARGS 6

typedef struct s_td
{
		unsigned int     flags;
		int              pid;
		unsigned int     err;
		__kernel_ulong_t sc_no;
		__kernel_ulong_t sc_args[MAX_ARGS];
		__kernel_ulong_t sc_ret;
		int              sys_ret;
		struct timespec  ptime;
		struct timespec  stime;
} t_td;

#endif