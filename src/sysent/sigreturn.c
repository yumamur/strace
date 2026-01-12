#include "../ft_print.h"
#include "../ft_utils.h"
#include "../regs.h"
#include "xlat.h"

#ifndef __USE_XOPEN2K8
#  define __USE_XOPEN2K8
#endif

// #include <bits/types/stack_t.h>

#include <asm/signal.h>

#include <asm/sigcontext.h>
#include <asm/siginfo.h>
#include <asm/ucontext.h>

struct kernel_rt_sigframe
{
		__kernel_ulong_t pretcode;
		// struct siginfo  info;
		struct ucontext uc;
};

extern int      set_stack_pointer_to(struct s_td *td, __kernel_ulong_t *sp);
extern void     printsigset_struct(struct s_td *td, __kernel_ulong_t addr);
extern t_reg64 *get_reg_64(void);

void            print_stack_frame(struct s_td *td, __kernel_ulong_t addr)
{
	// sigset_t               sigset;

	const __kernel_ulong_t sigmask = addr + offsetof(struct kernel_rt_sigframe, uc.uc_sigmask);
	// long                   ret = umovemem(td, &sigset, sigmask, sizeof(sigset));
	// if (ret <= 0)
	// return;
	printsigset_struct(td, (__kernel_ulong_t) sigmask);

	// printsigset_struct(NULL, (__kernel_ulong_t) &sigset);
}

SYS_FUNC(rt_sigreturn)
{
	(void) td;
	// __kernel_ulong_t sp = 0;
	// if (set_stack_pointer_to(td, &sp) == -1)
	// 	printaddr(sp);
	// else
	// {
	// 	const __kernel_ulong_t sigmask = sp + offsetof(struct kernel_rt_sigframe, uc.uc_sigmask);
	// 	printsigset_struct(td, sigmask);
	// }
	return SF_DECODE_COMPLETE;
}