#ifndef FT_TRACE
#define FT_TRACE

#include "ft_common.h"
#include <linux/posix_types.h>
#include <time.h>

#define TD_INSYSCALL   0x01
#define TD_OUT_SUMMARY 0x02

#define SC_DECODE_COMPLETE 0x10
#define SC_PRINT_HEX       0x20
#define SC_MASK            (SC_DECODE_COMPLETE | SC_PRINT_HEX)

#define entering(tcp) (!((tcp).flags & TD_INSYSCALL))
#define exiting(tcp)  ((tcp).flags & TD_INSYSCALL)

#define MAX_ARGS 6

enum e_abi
{
	ABI_64BIT,
	ABI_32BIT,
	ABI_X32
};

typedef struct s_td
{
		enum e_abi       abi;
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