#include "poll.xlat.h"

#include "../ft_print.h"
#include "../ft_utils.h"
#include <poll.h>

void print_pollfds(t_td *td)
{
	struct pollfd          buf;
	size_t                 nmem = (size_t) zero_extend_signed_to_size_t(td->sc_args[1]);

	__kernel_ulong_t       start_addr = td->sc_args[0];
	const size_t           size = nmem * sizeof(buf);
	const __kernel_ulong_t end_addr = start_addr + size;

	if (end_addr < start_addr || size / sizeof(buf) != nmem)
	{
		print_debug("size overflow");
		printaddr(start_addr);
		return;
	}

	__kernel_ulong_t cur_addr = start_addr;
	int              read_bytes;

	print_arr_start();
	for (size_t i = 0;
		 i < nmem;
		 ++i, cur_addr += sizeof(buf))
	{
		read_bytes = umovemem(td, &buf, cur_addr, sizeof(buf));

		if (read_bytes < 0)
		{
			printaddr(cur_addr);
			if (cur_addr != start_addr)
				print_has_more();
			break;
		}

		if (cur_addr < start_addr)
		{
			print_debug("memory wrap-around");
			break;
		}

		if (cur_addr != start_addr)
			print_arg_sep();

		print_struct_start();

		print_struct_member("fd");
		printfd(buf.fd);

		print_next_struct_member("events");
		printflags(pollfd_events, (uint64_t) buf.events, "POLL???");

		print_struct_end();
	}
	print_arr_end();
}

int print_pollfd(struct s_td *td, void *buf)
{
	struct pollfd *pt = (struct pollfd *) buf;

	if (!(uint64_t) pt->revents)
		return 0;

	(void) td;
	print_struct_start();

	print_struct_member("fd");
	printfd(pt->fd);

	print_next_struct_member("revents");
	printflags(pollfd_events, (uint64_t) pt->revents, "POLL???");

	print_struct_end();
	return 1;
}

SYS_FUNC(poll)
{
	if (entering(*td))
	{
		FIRST_ARG("fds");
		print_pollfds(td);

		NEXT_ARG("nfds");
		PRINT_ULL(td->sc_args[1]);

		NEXT_ARG("timeout");
		PRINT_LL(td->sc_args[2]);

		return SF_AFTER_RETURN;
	}
	else
	{
		struct pollfd buf;
		print_arg_start();
		printarray(
			td,
			print_pollfd,
			td->sc_args[0],
			&buf,
			td->sc_args[1],
			sizeof(buf));
		print_arg_end();
		return SF_DECODE_COMPLETE;
	}
}