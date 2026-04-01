#include "../ft_print.h"
#include "shstk.xlat.h"

SYS_FUNC(map_shadow_stack)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("size");
	PRINT_LU(td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(shadow_stack_flags, td->sc_args[2], "SHADOW_STACK_???");

	return SF_DECODE_COMPLETE;
}