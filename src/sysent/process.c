#include "../ft_print.h"
#include "../ft_common.h"
#include "process.xlat.h"

SYS_FUNC(arch_prctl)
{
	FIRST_ARG("op");
	printflag(arch_prctl_operations, td->sc_args[0], "ARCH_???");

	// another half a ton of if else logging, skipped
	NEXT_ARG("addr");
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}