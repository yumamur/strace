#include "../ft_print.h"
#include "../ft_utils.h"
#include "sched.xlat.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void printsched_param(struct s_td *td, __kernel_ulong_t addr)
{
	struct
	{
			int sched_priority;
	} buf;
	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_D(buf, sched_priority);
	print_struct_end();
}

static void print_policy(int policy)
{
	if (policy & SCHED_RESET_ON_FORK)
	{
		prints("SCHED_RESET_ON_FORK|");
		policy &= ~SCHED_RESET_ON_FORK;
	}
	printflag_indexed(scheduler_policies, policy, "SCHED_???");
}

SYS_FUNC(nice)
{
	FIRST_ARG("increment");
	PRINT_D(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sched_setparam)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("param");
	printsched_param(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sched_getparam)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("param");
		printsched_param(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(sched_setscheduler)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("policy");
	print_policy(td->sc_args[1]);

	NEXT_ARG("param");
	printsched_param(td, td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sched_getscheduler)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);

		return SF_AFTER_RETURN;
	}
	else
	{
		print_comment_start();
		print_policy(td->sc_ret);
		print_comment_end();

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(sched_get_priority_max)
{
	FIRST_ARG("policy");
	print_policy(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

void decode_sched_rr_get_interval(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("tp");
		printtimespec_fn(td, td->sc_args[1]);
	}
}

SYS_FUNC(sched_rr_get_interval_time32)
{
	decode_sched_rr_get_interval(td, printtimespec32);
	return 0;
}

SYS_FUNC(sched_rr_get_interval_time64)
{
	decode_sched_rr_get_interval(td, printtimespec64);
	return 0;
}

unsigned int get_cpu_ct(void)
{
	static unsigned int ct;

	if (!ct)
	{
		ct = sysconf(_SC_NPROCESSORS_CONF);
		if (ct < 1)
			ct = 1024;
	}

	return ct;
}

void print_affinity(struct s_td *td, __kernel_ulong_t addr, unsigned int len)
{
	uint8_t      buf;
	unsigned int cpu_cap = (get_cpu_ct() + 7) / 8;
	unsigned int fetch_size = MIN(cpu_cap, len);
	// getaffinity fails if 'len % 8 != 0', but setaffinity is fine with any val
	printarray(td, (t_printarray_cfg){
					   .printer = print_byte,
					   .start_addr = addr,
					   .pt_buf_var = &buf,
					   .n_var = fetch_size,
					   .var_size = sizeof(buf),
				   });
}

SYS_FUNC(sched_setaffinity)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("cpusetsize");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("mask");
	print_affinity(td, td->sc_args[2], td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sched_getaffinity)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);

		NEXT_ARG("cpusetsize");
		PRINT_U(td->sc_args[1]);
	}
	else
	{
		NEXT_ARG("mask");
		print_affinity(td, td->sc_args[2], td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(sched_setattr)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("attr");
	printaddr(td->sc_args[1]);

	NEXT_ARG("flags");
	// feature not implemented yet
	PRINT_U(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sched_getattr)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("attr");
	// I'm too bored to decode this struct
	printaddr(td->sc_args[1]);

	NEXT_ARG("size");
	PRINT_U(td->sc_args[2]);

	NEXT_ARG("flags");
	PRINT_U(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(membarrier)
{
	if (entering(*td))
	{
		FIRST_ARG("cmd");
		printflags(membarrier_cmds, td->sc_args[0], "MEMBARRIER_CMD_???");

		NEXT_ARG("flags");
		printflags(membarrier_flags, td->sc_args[1], "MEMBARRIER_CMD_FLAG_???");

		if (td->sc_args[1] == MEMBARRIER_CMD_FLAG_CPU)
		{
			NEXT_ARG("cpu_id");
			PRINT_LL(td->sc_args[2]);
		}

		if (td->sc_args[0] == MEMBARRIER_CMD_QUERY)
			return SF_AFTER_RETURN;
	}
	else if (!td->sc_err && td->sc_ret)
		printflags(membarrier_cmds, td->sc_ret, "MEMBARRIER_CMD_???");
	return 0;
}