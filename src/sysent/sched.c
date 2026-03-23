#include "../ft_print.h"
#include "../ft_utils.h"
#include "sched.xlat.h"

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

SYS_FUNC(get_priority_max)
{
	FIRST_ARG("policy");
	print_policy(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sched_rr_get_interval)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("tp");
		printtimespec(td, td->sc_args[1]);
	}
	return 0;
}