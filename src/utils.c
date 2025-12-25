#include "ft_utils.h"

enum e_abi detect_abi(struct user_regs_struct *g_regs)
{
#ifdef __x86_64__
	if (g_regs->cs == 0x33)
	{
		if (g_regs->orig_rax & __X32_SYSCALL_BIT)
			return ABI_X32;
		return ABI_64BIT;
	}
	else if (g_regs->cs == 0x23)
	{
		return ABI_32BIT;
	}

	// Fallback
	if (g_regs->orig_rax & __X32_SYSCALL_BIT)
		return ABI_X32;

	return ABI_64BIT;
#else
	return ABI_32BIT;
#endif
}