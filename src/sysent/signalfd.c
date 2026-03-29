#include "../ft_print.h"
#include "../ft_utils.h"
#include "signalfd.xlat.h"

int decode_signalfd(struct s_td *td, int has_flags)
{
	FIRST_ARG("ufd");
	printfd(td->sc_args[0]);

	NEXT_ARG("user_mask");
	printsigmask_sized(td, td->sc_args[1], td->sc_args[2]);

	NEXT_ARG("sizemask");
	PRINT_LU(td->sc_args[2]);

	if (has_flags)
	{
		NEXT_ARG("flags");
		printflags(signalfd_flags, td->sc_args[3], "SFD_???");
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(signalfd)
{
	return decode_signalfd(td, 0);
}

SYS_FUNC(signalfd4)
{
	return decode_signalfd(td, 1);
}