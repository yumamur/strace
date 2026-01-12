#include "../ft_common.h"
#include "../ft_print.h"
#include "../ft_utils.h"
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
	{
		printaddr(addr);
		return;
	}
	print_struct_start();
	print_struct_member("sigmask");
	printsigmask(td, (__kernel_ulong_t) buf.sigmask);
	print_struct_member_sep();
	PRINT_MEMBER(&buf, sigsetsize, PRINT_LL);
	print_struct_end();
}

void printfd_set(struct s_td *td, __kernel_ulong_t addr, unsigned long len)
{
	if (len > sizeof(fd_set))
		len = sizeof(fd_set);

	unsigned long buf[FD_SETSIZE / sizeof(long)];

	FD_ZERO((fd_set *) buf);

	if (umovemem(td, buf, addr, len) <= 0)
	{
		printaddr(addr);
		return;
	}

	print_arr_start();

	int i;
	int first = 1;
	int nbits = len * 8;

	for (i = 0; i < nbits; i++)
	{
		int index = i / (sizeof(unsigned long) * 8);
		int bit = i % (sizeof(unsigned long) * 8);

		if (buf[index] & (1UL << bit))
		{
			if (!first)
				print_space();

			printfd(i);
			first = 0;
		}
	}

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
		printfd_set(td, td->sc_args[1], nfds);

		NEXT_ARG("writefds");
		printfd_set(td, td->sc_args[2], nfds);

		NEXT_ARG("exceptfds");
		printfd_set(td, td->sc_args[3], nfds);

		NEXT_ARG("timeout");
		printtimeval(td, td->sc_args[4]);

		return SF_AFTER_RETURN;
	}
	else
	{
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
		printfd_set(td, td->sc_args[1], nfds);

		NEXT_ARG("writefds");
		printfd_set(td, td->sc_args[2], nfds);

		NEXT_ARG("exceptfds");
		printfd_set(td, td->sc_args[3], nfds);

		NEXT_ARG("timeout");
		printtimeval(td, td->sc_args[4]);

		NEXT_ARG("sigmask");
		printsigset_kernel(td, td->sc_args[5]);

		return SF_AFTER_RETURN;
	}
	else
	{
		return SF_DECODE_COMPLETE;
	}
}