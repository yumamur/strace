#include "../ft_print.h"
#include "../ft_utils.h"
#include "eventpoll.xlat.h"
#include <linux/eventpoll.h>

typedef void (*t_print_timeout)(struct s_td *, __kernel_ulong_t);

SYS_FUNC(epoll_create)
{
	FIRST_ARG("size");
	PRINT_U(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

int print_epoll_event(struct s_td *td, void *addr, size_t struct_size)
{
	(void) td;
	(void) struct_size;
	struct epoll_event *buf = addr;

	print_struct_start();
	PRINT_MEMBER_FLAGS(*buf, events, epoll_event_flags, "EPOLL_???");
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*buf, data);
	print_struct_end();
	return PRINTARR_STATE_SEP;
}

int decode_epoll_wait(struct s_td *td, t_print_timeout print_timeout)
{
	if (entering(*td))
	{
		FIRST_ARG("epfd");
		printfd(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("events");
		struct epoll_event buf;
		printarray(td, (t_printarray_cfg){
						   .printer = print_epoll_event,
						   .start_addr = td->sc_args[1],
						   .pt_buf_var = &buf,
						   .n_var = MIN(td->sc_args[2], td->sc_ret),
						   .var_size = sizeof(buf),
					   });

		NEXT_ARG("maxevents");
		PRINT_D(td->sc_args[2]);

		NEXT_ARG("timeout");
		print_timeout(td, td->sc_args[3]);
	}
	return 0;
}

void print_timeout_int(struct s_td *td, __kernel_ulong_t addr)
{
	(void) td;
	PRINT_D(addr);
}

SYS_FUNC(epoll_wait)
{
	return decode_epoll_wait(td, print_timeout_int);
}

SYS_FUNC(epoll_ctl)
{
	FIRST_ARG("epfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("op");
	printflag(epoll_ctl_ops, td->sc_args[1], "EPOLL_CTL_???");

	NEXT_ARG("fd");
	printfd(td->sc_args[2]);

	NEXT_ARG("event");
	struct epoll_event buf;
	if (td->sc_args[1] == EPOLL_CTL_DEL)
		printaddr(td->sc_args[3]);
	else if (!umovemem_or_printaddr(td, &buf, td->sc_args[3], sizeof(buf)))
		print_epoll_event(td, &buf, 0);

	return SF_DECODE_COMPLETE;
}

int decode_epoll_pwait(struct s_td *td, t_print_timeout print_timeout)
{
	decode_epoll_wait(td, print_timeout);
	if (exiting(*td))
	{
		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[4]);

		// libc wrapper sets this internally, so it is not passed by the caller,
		// but we can print it anyway
		NEXT_ARG("sigsetsize");
		PRINT_U(td->sc_args[5]);
	}
	return 0;
}

SYS_FUNC(epoll_pwait)
{
	return decode_epoll_pwait(td, print_timeout_int);
}

SYS_FUNC(epoll_pwait2)
{
	return decode_epoll_pwait(td, printtimespec64);
}

SYS_FUNC(epoll_create1)
{
	FIRST_ARG("flags");
	printflags(epoll_create1_flags, td->sc_args[0], "EPOLL_???");

	return SF_DECODE_COMPLETE;
}
