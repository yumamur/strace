#include "../ft_print.h"

SYS_FUNC(getcwd)
{
	if (entering(*td))
		return 0;

	FIRST_ARG("buf");
	printnstr(td, td->sc_args[0], td->sc_args[1]);

	NEXT_ARG("size");
	PRINT_U(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}