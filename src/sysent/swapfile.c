#include "../ft_print.h"
#include "../ft_common.h"
#include "swapfile.xlat.h"

SYS_FUNC(swapon)
{
	unsigned int flags = td->sc_args[1];
	unsigned int prio = flags & SWAP_FLAG_PRIO_MASK;
	flags &= ~SWAP_FLAG_PRIO_MASK;
	if (flags & SWAP_FLAG_PREFER)
		prio >>= SWAP_FLAG_PRIO_SHIFT;

	FIRST_ARG("path");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("swapflags");
	if (flags)
	{
		printflags(swap_flags, flags, "SWAP_???");
		print_or();
	}
	PRINT_LLU(prio);

	return SF_DECODE_COMPLETE;
}