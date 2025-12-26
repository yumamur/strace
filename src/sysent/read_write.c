#include "../ft_common.h"
#include "../ft_print.h"
#include "xlat.h"
#include <linux/fcntl.h>
#include <string.h>

SYS_FUNC(read)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
		return 0;
	}
	else
	{
		NEXT_ARG("buf");
		printnstr(td, td->sc_args[1], td->sc_ret);

		NEXT_ARG("count");
		PRINT_ULL(td->sc_args[2]);

		return SC_DECODE_COMPLETE;
	}
}

SYS_FUNC(write)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("buf");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("count");
	PRINT_ULL(td->sc_args[2]);

	return SC_DECODE_COMPLETE;
}