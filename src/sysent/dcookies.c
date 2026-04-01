#include "../ft_print.h"

SYS_FUNC(lookup_dcookie)
{
	if (exiting(*td))
	{
		FIRST_ARG("cookie");
		unsigned argi = print_llu_arg(td, 0);

		NEXT_ARG("buffer");
		printnstr(td, td->sc_args[argi], td->sc_args[argi + 1]);

		NEXT_ARG("size");
		PRINT_U(td->sc_args[argi + 1]);
	}
	return 0;
}