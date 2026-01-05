#include "../ft_print.h"
#include "../ft_syscall.h"
#include "../ft_utils.h"
#include "xlat.h"

SYS_FUNC(lseek)
{
	FIRST_ARG("fd");
	PRINT_LL(td->sc_args[0]);

	NEXT_ARG("offset");
	PRINT_LL(td->sc_args[1]);

	NEXT_ARG("whence");
	printflags(lseek_whence_values, td->sc_args[2], "UNKNOWN");
	return SC_DECODE_COMPLETE;
}