#include "../ft_print.h"
#include "../ft_utils.h"
#include "signal.xlat.h"

#define SA_MASK_SIZE (_SIGSET_NWORDS / sizeof(unsigned long int))

void printsignal(int signum)
{
	prints(signal_names[signum]);
}

void printsigset_t(struct s_td *td, __kernel_ulong_t set)
{
	if (!set)
		return print_null();

	uint32_t buf;
	if (td)
		umovemem(td, &buf, set, sizeof(buf));
	else
		buf = *(uint32_t *) set;
	if (count_set_bits(&buf, sizeof(buf)) >= sizeof(buf) * 8 * 2 / 3)
	{
		buf = ~buf;
		prints("~");
	}
	print_arr_start();
	int logged = 0;
	for (size_t i = 0; i < ARRAY_SIZE(signal_names); i++)
	{
		if (signal_names[i] && sigismember((sigset_t *) &buf, (int) i))
		{
			if (logged)
				print_or();
			logged = 1;
			prints(signal_names[i] + 3);
		}
	}
	print_arr_end();
}

void printsa_handler(void *handler)
{
	const __kernel_ulong_t addr = (__kernel_ulong_t) handler;
	const char            *sa_handler_name = search_xlat(sa_handlers, addr);

	if (sa_handler_name)
	{
		prints(sa_handler_name);
		print_debug(" /* %lx */", addr);
	}
	else
		printaddr(addr);
}

struct sigaction_64
{
		uint64_t sa_handler_;
		uint64_t sa_flags;
		uint64_t sa_restorer;
		/**
		 * kernel allocates 'sizeof(unsigned long int) * _SIGSET_NWORDS'
		 * (128 bytes??) for this and does not even use past sa_mask[1].
		 */
		uint64_t sa_mask[SA_MASK_SIZE];
};

struct sigaction_32
{
		uint32_t sa_handler_;
		uint32_t sa_flags;
		uint32_t sa_restorer;
		uint32_t sa_mask[SA_MASK_SIZE * 2];
};

void printsigaction(t_td *td, __kernel_ulong_t addr)
{
	struct sigaction_64 buf = {};
	if (!addr)
		return print_null();

	umovemem(td, &buf, addr, sizeof(buf));

	print_struct_start();

	print_struct_member("sa_handler");
	printsa_handler((void *) buf.sa_handler_);

	print_next_struct_member("sa_mask");
	printsigset_t(NULL, (__kernel_ulong_t) buf.sa_mask);

	print_next_struct_member("sa_flags");
	printflags(sigaction_sa_flags, (unsigned) buf.sa_flags, "SA_???");

#ifdef SA_RESTORER
	/**
	 * Kernel headers notes that SA_RESTORER is obsolete and should not be used,
	 * the flag is commented out. But strace still logs, I'll roll with it.
	 */
	if (buf.sa_flags & SA_RESTORER)
	{
		print_next_struct_member("sa_restorer");
		printaddr((__kernel_ulong_t) buf.sa_restorer);
	}
#endif
	print_struct_end();
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

SYS_FUNC(rt_sigprocmask)
{
	if (entering(*td))
	{
		FIRST_ARG("how");
		printflags(sigproc_how, (unsigned) td->sc_args[0], "SIG_???");

		NEXT_ARG("set");
		printsigset_t(td, td->sc_args[1] /* , td->sc_args[3] */);
		return 0;
	}
	else
	{
		NEXT_ARG("oldset");
		printsigset_t(td, td->sc_args[2] /* , td->sc_args[3] */);

		NEXT_ARG("sigsetsize");
		PRINT_ULL(td->sc_args[3]);

		return SF_DECODE_COMPLETE;
	}
}