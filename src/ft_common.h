#ifndef FT_COMMON
#define FT_COMMON

#include "sysent/ft_sysent.h"
#include <errno.h>
#include <linux/posix_types.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MAX_ARGS 6

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define TD_INSYSCALL   0x1
#define TD_OUT_SUMMARY 0x2

#define entering(tcp) (!((tcp)->flags & TD_INSYSCALL))
#define exiting(tcp)  ((tcp)->flags & TD_INSYSCALL)

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

#define SYS_FUNC_NAME(syscall_name) sys_##syscall_name
#define SYS_FUNC(syscall_name) \
	void SYS_FUNC_NAME(syscall_name)(struct s_td * td)

#endif