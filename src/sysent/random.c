#include "../ft_print.h"
// #include "../ft_utils.h"
#include "random.xlat.h"

SYS_FUNC(getrandom)
{
	if (exiting(*td))
	{
		FIRST_ARG("buf");
		printmem(td, td->sc_args[0], td->sc_args[1]);

		NEXT_ARG("buflen");
		PRINT_LLU(td->sc_args[1]);

		NEXT_ARG("flags");
		printflags(getrandom_flags, td->sc_args[2], "GRND_???");
	}

	return 0;
}