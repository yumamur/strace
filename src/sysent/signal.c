#include "../ft_print.h"
#include "../ft_utils.h"
#include "signal.xlat.h"

#include "../regs.h"

#include <string.h>

// including 'stdlib.h' for free() breaks some sigset_t related tweaks, so just declare it here
extern void free(void *ptr);

#define SA_MASK_SIZE (NSIG / sizeof(unsigned long int))
#ifndef _NSIG
#  define _NSIG 8 * sizeof(unsigned long int)
#endif

void printsiginfo(struct s_td *td, __kernel_ulong_t addr)
{
	siginfo_t si;

	if (umovemem(td, &si, addr, sizeof(siginfo_t)) < 0)
		printaddr(addr);

	print_struct_start();

	PRINT_MEMBER(si, si_signo, PRINT_D);

	print_struct_member_sep();
	if (SI_FROMUSER(&si))
	{
		print_struct_member("si_code");
		printflag(siginfo_codes, si.si_code, "SI_???");
	}
	else
		PRINT_MEMBER(si, si_code, PRINT_D);

	if (si.si_errno)
	{
		print_struct_member_sep();
		PRINT_MEMBER(si, si_errno, PRINT_D);
	}

	if (si.si_code != SI_SIGIO && si.si_code != SI_TIMER)
	{
		print_struct_member_sep();
		PRINT_MEMBER(si, si_pid, PRINT_D);
		print_struct_member_sep();
		PRINT_MEMBER(si, si_uid, PRINT_D);
	}
	else if (si.si_code == SI_TIMER)
	{
		print_struct_member_sep();
		PRINT_MEMBER(si, si_tid, PRINT_D);
		print_struct_member_sep();
		PRINT_MEMBER(si, si_overrun, PRINT_D);
	}
	else if (si.si_code == SI_SIGIO)
	{
		print_struct_member_sep();
		PRINT_MEMBER(si, si_band, PRINT_LU);
	}
	// a lot of work afterwards, which this does not deserve imo
	print_has_more();

	print_struct_end();
}

void printsignal(int signum)
{
	if (signum > 0)
	{
		const unsigned int us = signum;
		if (us < ARRAY_SIZE(signal_names))
			prints(signal_names[us]);
		else if (us >= SIGRTMIN && us <= SIGRTMAX)
		{
			prints("SIGRT_");
			PRINT_D(us - SIGRTMIN);
		}
	}
	else
		PRINT_D(signum);
}

void printsigset_t(const uint64_t *addr)
{
	if (!addr)
		return print_null();

	uint64_t           buf = *addr;
	const unsigned int total_bits = 64;
	const unsigned int ct = count_set_bits(&buf, sizeof(buf));
	const unsigned int threshold = total_bits * 2 / 3;

	if (ct >= threshold)
	{
		buf = ~buf;
		prints("~");
	}

	print_arr_start();
	int logged = 0;

	for (unsigned int bit = 0; bit < 64; ++bit)
	{
		if (!(buf & (1ULL << bit)))
			continue;

		const unsigned int sig = bit + 1;

		if (logged)
			print_space();
		logged = 1;

		if (sig >= (unsigned) SIGRTMIN && sig <= (unsigned) SIGRTMAX)
		{
			prints("RT_");
			PRINT_L(sig - SIGRTMIN);
		}
		else if (sig < ARRAY_SIZE(signal_names) && signal_names[sig])
			prints(signal_names[sig] + 3);
		else
			PRINT_L(sig);
	}

	print_arr_end();
}

void printsigmask(struct s_td *td, __kernel_ulong_t addr)
{
	if (!addr)
		return print_null();

	uint64_t buf;
	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
		return printaddr(addr);

	printsigset_t(&buf);
}

void printsigmask_sized(struct s_td *td, __kernel_ulong_t addr, unsigned int sigsetsize)
{
	if (sigsetsize > sizeof(uint64_t))
		return printaddr(addr);

	printsigmask(td, addr);
}

void printsigset_struct(struct s_td *td, __kernel_ulong_t addr)
{
	print_struct_start();
	print_struct_member("mask");
	printsigmask(td, addr);
	print_struct_end();
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

	if (current_wordsize < sizeof(buf.sa_handler_))
	{
		struct sigaction_32 buf32 = {};
		if (umovemem(td, &buf32, addr, sizeof(buf32)) < 0)
			return printaddr(addr);

		buf.sa_handler_ = buf32.sa_handler_;
		buf.sa_flags = buf32.sa_flags;
#ifdef SA_RESTORER
		buf.sa_restorer = buf32.sa_restorer;
#endif
/**
 * it's unsettling that this matters..
 */
#if defined(__BIG_ENDIAN__)
		buf.sa_mask[0] = (uint64_t) buf32.sa_mask[1] | ((uint64_t) (buf32.sa_mask[0]) << 32);
#elif defined(__LITTLE_ENDIAN__)
		buf.sa_mask[0] = (uint64_t) buf32.sa_mask[0] | ((uint64_t) (buf32.sa_mask[1]) << 32);
#endif
	}
	else
	{
		if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
			return printaddr(addr);
	}

	print_struct_start();

	print_struct_member("sa_handler");
	printsa_handler((void *) buf.sa_handler_);

	print_next_struct_member("sa_mask");
	printsigset_t(buf.sa_mask);

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
		PRINT_LLU(td->sc_args[3]);

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
		printsigmask(td, td->sc_args[1] /* , td->sc_args[3] */);
		return 0;
	}
	else
	{
		NEXT_ARG("oldset");
		printsigmask(td, td->sc_args[2] /* , td->sc_args[3] */);

		NEXT_ARG("sigsetsize");
		PRINT_LLU(td->sc_args[3]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(kill)
{
	FIRST_ARG("pid");
	PRINT_LL(td->sc_args[0]);

	NEXT_ARG("sig");
	printsignal(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(rt_sigpending)
{
	NEXT_ARG("set");
	printsigmask(td, td->sc_args[0]);

	NEXT_ARG("sigsetsize");
	PRINT_LU(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(rt_sigtimedwait)
{
	if (entering(*td))
	{
		NEXT_ARG("set");
		printsigmask(td, td->sc_args[0]);

		if (!td->sc_args[1])
		{
			NEXT_ARG("info");
			print_null();

			NEXT_ARG("timeout");
			printtimespec(td, td->sc_args[2]);

			NEXT_ARG("sigsetsize");
			PRINT_LU(td->sc_args[3]);

			return SF_DECODE_COMPLETE;
		}
		else
			td_carry(td, strdup(sprinttimespec(td, td->sc_args[2])), free);
		return 0;
	}
	else
	{
		if (td->sc_args[1])
		{
			NEXT_ARG("info");
			printsiginfo(td, td->sc_args[1]);

			NEXT_ARG("timeout");
			prints((char *) td->carry);
			td_free_carry(td);

			NEXT_ARG("sigsetsize");
			PRINT_LU(td->sc_args[3]);
		}

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(rt_sigqueueinfo)
{
	FIRST_ARG("pid");
	PRINT_LL(td->sc_args[0]);

	NEXT_ARG("sig");
	printsignal(td->sc_args[1]);

	NEXT_ARG("info");
	printsiginfo(td, td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(rt_sigsuspend)
{
	FIRST_ARG("unewset");
	printsigmask_sized(td, td->sc_args[0], td->sc_args[1]);

	NEXT_ARG("sigsetsize");
	PRINT_LLU(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

void printstack_t(struct s_td *td, __kernel_ulong_t addr)
{
	stack_t buf;

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_PTR(buf, ss_sp);
	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(buf, ss_flags, signalstack_flags, "SS_???");
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, ss_size);
	print_struct_end();
}

SYS_FUNC(sigaltstack)
{
	if (entering(*td))
	{
		FIRST_ARG("ss");
	}
	else
	{
		NEXT_ARG("old_ss");
	}

	return 0;
}