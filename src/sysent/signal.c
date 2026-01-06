#include "../ft_print.h"
#include "../ft_utils.h"
#include "signal.xlat.h"

void printsignal(int signum)
{
	PRINT_D(signum);
	print_comment(signal_names[signum]);
}

void printsa_handler(void *handler)
{
	const __kernel_ulong_t addr = (__kernel_ulong_t) handler;
	const char            *sa_handler_name = search_xlat(sa_handlers, addr);

	if (sa_handler_name)
	{
		prints(sa_handler_name);
		print_comment("%lx", addr);
	}
	else
		printaddr(addr);
}

void printsigaction(t_td *td, __kernel_ulong_t addr)
{
	struct sigaction buf;
	umovemem(td, &buf, addr, sizeof(buf));

	printsa_handler(buf.sa_handler);
}

SYS_FUNC(rt_sigaction)
{
	if (entering(*td))
	{
		FIRST_ARG("signum");
		printsignal(td->sc_args[0]);

		NEXT_ARG("act");
		printsigaction(td, td->sc_args[1]);

		return 0;
	}
	else
	{
		NEXT_ARG("oldact");
		printsigaction(td, td->sc_args[2]);

		NEXT_ARG("sigsetsize");
		PRINT_ULL(td->sc_args[3]);

		return SF_DECODE_COMPLETE;
	}
}