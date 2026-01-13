#include "../ft_print.h"
#include "../ft_utils.h"

SYS_FUNC(sendfile64)
{
	if (entering(*td))
	{
		FIRST_ARG("out_fd");
		printfd(td->sc_args[0]);

		NEXT_ARG("in_fd");
		printfd(td->sc_args[1]);

		NEXT_ARG("offset");
		if (printnum_addr_long(td, td->sc_args[2]) == -1)
		{
			NEXT_ARG("count");
			PRINT_ULL(td->sc_args[3]);

			return SF_DECODE_COMPLETE;
		}
	}
	else
	{
		if (!td->sc_err && td->sc_ret)
		{
			print_val_change();

			printnum_addr_long(td, td->sc_args[2]);
		}

		NEXT_ARG("count");
		PRINT_ULL(td->sc_args[3]);
	}
	return 0;
}