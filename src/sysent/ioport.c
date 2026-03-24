#include "../ft_common.h"
#include "../ft_print.h"

SYS_FUNC(iopl)
{
	FIRST_ARG("level");
	PRINT_LLU(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(ioperm)
{
	FIRST_ARG("from");
	PRINT_LLX(td->sc_args[0]);

	FIRST_ARG("num");
	PRINT_LLX(td->sc_args[1]);

	FIRST_ARG("turn_on");
	PRINT_D(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}