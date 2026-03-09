#include "../ft_common.h"
#include "../ft_print.h"
#include "../ft_utils.h"
#include "select.xlat.h"
#include <sys/select.h>

#include <time.h>

struct kernel_sigset_argpack
{
		sigset_t *sigmask;
		size_t    sigsetsize;
};

void printsigset_kernel(struct s_td *td, __kernel_ulong_t addr)
{
	struct kernel_sigset_argpack buf = {};

	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
		return printaddr(addr);

	print_struct_start();
	print_struct_member("sigmask");
	printsigmask(td, (__kernel_ulong_t) buf.sigmask);
	print_struct_member_sep();
	PRINT_MEMBER(&buf, sigsetsize, PRINT_LL);
	print_struct_end();
}

int printfd_set(struct s_td     *td,
				__kernel_ulong_t addr,
				unsigned long    readsize,
				int              expected_max_n_bit)
{
	if (readsize > sizeof(fd_set))
		readsize = sizeof(fd_set);

	unsigned long buf[FD_SETSIZE / sizeof(long)];

	FD_ZERO((fd_set *) buf);

	if (umovemem(td, buf, addr, readsize) <= 0)
	{
		printaddr(addr);
		return 0;
	}

	print_arr_start();

	int i;
	int first = 1;
	int nbits = readsize * 8;
	int total_set = 0;

	for (i = 0; i < nbits && expected_max_n_bit > 0; i++)
	{
		int index = i / (sizeof(unsigned long) * 8);
		int bit = i % (sizeof(unsigned long) * 8);

		if (buf[index] & (1UL << bit))
		{
			if (!first)
				print_space();

			printfd(i);
			++total_set;
			--expected_max_n_bit;
			first = 0;
		}
	}

	print_arr_end();

	return total_set;
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
	struct pollfd buf;
	if (entering(*td))
	{
		FIRST_ARG("fds");
		printarray(
			td,
			print_pollfd,
			td->sc_args[0],
			&buf,
			td->sc_args[1],
			sizeof(buf));

		NEXT_ARG("nfds");
		PRINT_LLU(td->sc_args[1]);

		NEXT_ARG("timeout");
		PRINT_LL(td->sc_args[2]);

		return SF_AFTER_RETURN;
	}
	else
	{
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

void do_select_exiting(struct s_td *td, __kernel_ulong_t fdss[], int nfds, int nmax)
{
	if (nmax == 0)
		return print_comment("Timeout");

	print_arg_start();

	print_struct_member("in");
	nmax -= printfd_set(td, fdss[0], nfds, nmax);

	print_next_struct_member("out");
	nmax -= printfd_set(td, fdss[1], nfds, nmax);

	print_next_struct_member("exc");
	nmax -= printfd_set(td, fdss[2], nfds, nmax);

	print_arr_end();
}

SYS_FUNC(select)
{
	int nfds = (int) td->sc_args[0];
	if (nfds < 0)
		nfds = 0;
	if (nfds > FD_SETSIZE)
		nfds = FD_SETSIZE;

	if (entering(*td))
	{
		FIRST_ARG("nfds");
		PRINT_D(nfds);

		NEXT_ARG("readfds");
		printfd_set(td, td->sc_args[1], nfds, nfds);

		NEXT_ARG("writefds");
		printfd_set(td, td->sc_args[2], nfds, nfds);

		NEXT_ARG("exceptfds");
		printfd_set(td, td->sc_args[3], nfds, nfds);

		NEXT_ARG("timeout");
		printtimeval(td, td->sc_args[4]);

		return SF_AFTER_RETURN;
	}
	else
	{
		do_select_exiting(td, &td->sc_args[1], nfds, td->sc_ret);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(pselect6)
{
	int nfds = (int) td->sc_args[0];
	if (nfds < 0)
		nfds = 0;
	if (nfds > FD_SETSIZE)
		nfds = FD_SETSIZE;

	if (entering(*td))
	{
		FIRST_ARG("nfds");
		PRINT_D(nfds);

		NEXT_ARG("readfds");
		printfd_set(td, td->sc_args[1], nfds, nfds);

		NEXT_ARG("writefds");
		printfd_set(td, td->sc_args[2], nfds, nfds);

		NEXT_ARG("exceptfds");
		printfd_set(td, td->sc_args[3], nfds, nfds);

		NEXT_ARG("timeout");
		printtimeval(td, td->sc_args[4]);

		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[5]);

		return SF_AFTER_RETURN;
	}
	else
	{
		do_select_exiting(td, &td->sc_args[1], nfds, td->sc_ret);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(ppoll)
{
	struct pollfd buf;
	if (entering(*td))
	{
		FIRST_ARG("fds");
		printarray(
			td,
			print_pollfd,
			td->sc_args[0],
			&buf,
			td->sc_args[1],
			sizeof(buf));

		NEXT_ARG("nfds");
		PRINT_LLU(td->sc_args[1]);

		NEXT_ARG("timeout");
		printtimeval(td, td->sc_args[2]);

		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[3]);

		return SF_AFTER_RETURN;
	}
	else
	{
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