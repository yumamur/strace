#ifndef FT_TRACE
#define FT_TRACE

#include "ft_common.h"
#include "regs.h"
#include <linux/posix_types.h>
#include <time.h>

#define entering(td_) (!((td_).flags & TD_INSYSCALL))
#define exiting(td_)  ((td_).flags & TD_INSYSCALL)

#define MAX_ARGS 6

enum e_abi;
struct s_entry;

typedef struct s_td
{
		enum e_abi       abi;
		unsigned int     flags;
		int              pid;
		struct s_entry  *entry;
		__kernel_ulong_t sc_no;
		__kernel_ulong_t sc_args[MAX_ARGS];
		__kernel_ulong_t sc_ret;
		uint32_t         sc_err;
		struct timespec  ptime;
		struct timespec  stime;
} t_td;

#endif