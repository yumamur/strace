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
	PRINT_MEMBER(buf, sigsetsize, PRINT_LL);
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

int print_pollfd(struct s_td *td, void *buf, size_t mem_size)
{
	(void) mem_size;
	struct pollfd *pt = (struct pollfd *) buf;

	if (!(uint64_t) pt->revents)
		return PRINTARR_STATE_CONT;

	(void) td;
	print_struct_start();

	print_struct_member("fd");
	printfd(pt->fd);

	print_next_struct_member("revents");
	printflags(pollfd_events, (uint64_t) pt->revents, "POLL???");

	print_struct_end();

	return PRINTARR_STATE_SEP;
}

static void print_pollfds(struct s_td *td, __kernel_ulong_t addr, unsigned long nfds)
{
	struct pollfd buf;

	printarray(td, (t_printarray_cfg){
					   .printer = print_pollfd,
					   .start_addr = addr,
					   .pt_buf_var = &buf,
					   .n_var = nfds,
					   .var_size = sizeof(buf),
				   });
}

SYS_FUNC(poll)
{
	if (entering(*td))
	{
		FIRST_ARG("fds");
		print_pollfds(td, td->sc_args[0], td->sc_args[1]);

		NEXT_ARG("nfds");
		PRINT_LLU(td->sc_args[1]);

		NEXT_ARG("timeout");
		PRINT_LL(td->sc_args[2]);

		return SF_AFTER_RETURN;
	}
	else
	{
		print_arg_start();
		print_pollfds(td, td->sc_args[0], MIN(td->sc_args[1], td->sc_ret));
		print_arg_end();

		return SF_DECODE_COMPLETE;
	}
}

int do_select(struct s_td *td, typeof(printtimeval32) printtimeval_fn)
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
		printtimeval_fn(td, td->sc_args[4]);

		return SF_AFTER_RETURN;
	}
	else
	{
		int nmax = td->sc_ret;
		if (nmax == 0)
			print_comment("Timeout");
		else
		{
			print_arg_start();

			print_struct_member("in");
			nmax -= printfd_set(td, td->sc_args[1], nfds, nmax);

			print_next_struct_member("out");
			nmax -= printfd_set(td, td->sc_args[2], nfds, nmax);

			print_next_struct_member("exc");
			nmax -= printfd_set(td, td->sc_args[3], nfds, nmax);

			print_arr_end();
		}

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(select)
{
	return do_select(td, printtimeval64);
}

int do_pselect6(struct s_td *td, typeof(printtimeval32) printtimeval_fn)
{
	int ret = do_select(td, printtimeval_fn);
	if (entering(*td))
	{
		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[5]);
	}
	return ret;
}

SYS_FUNC(pselect6_time32)
{
	return do_pselect6(td, printtimeval32);
}

SYS_FUNC(pselect6_time64)
{
	return do_pselect6(td, printtimeval64);
}

SYS_FUNC(ppoll_time32)
{
	if (entering(*td))
	{
		FIRST_ARG("fds");
		print_pollfds(td, td->sc_args[0], td->sc_args[1]);

		NEXT_ARG("nfds");
		PRINT_LLU(td->sc_args[1]);

		NEXT_ARG("timeout");
		printtimeval32(td, td->sc_args[2]);

		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[3]);

		return SF_AFTER_RETURN;
	}
	else
	{
		print_arg_start();
		print_pollfds(td, td->sc_args[0], td->sc_args[1]);
		print_arg_end();

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(ppoll_time64)
{
	if (entering(*td))
	{
		FIRST_ARG("fds");
		print_pollfds(td, td->sc_args[0], td->sc_args[1]);

		NEXT_ARG("nfds");
		PRINT_LLU(td->sc_args[1]);

		NEXT_ARG("timeout");
		printtimeval64(td, td->sc_args[2]);

		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[3]);

		return SF_AFTER_RETURN;
	}
	else
	{
		print_arg_start();
		print_pollfds(td, td->sc_args[0], td->sc_args[1]);
		print_arg_end();

		return SF_DECODE_COMPLETE;
	}
}