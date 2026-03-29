#include "../ft_print.h"
#include "ioprio.xlat.h"

static void print_prio(unsigned int prio)
{
	PRINT_D(prio);
	print_comment("IOPRIO_PRIO_CLASS(%s, %d)",
				  search_xlat(ioprio_classes, IOPRIO_PRIO_CLASS(prio)),
				  (int) IOPRIO_PRIO_DATA(prio));
}

SYS_FUNC(ioprio_set)
{
	FIRST_ARG("which");
	printflag(ioprio_which, td->sc_args[0], "IOPRIO_WHO_???");

	NEXT_ARG("who");
	PRINT_ID(td->sc_args[1]);

	NEXT_ARG("ioprio");
	print_prio(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(ioprio_get)
{
	if (entering(*td))
	{
		FIRST_ARG("which");
		printflag(ioprio_which, td->sc_args[0], "IOPRIO_WHO_???");

		NEXT_ARG("who");
		PRINT_ID(td->sc_args[1]);

		return SF_AFTER_RETURN;
	}
	else
	{
		if (td->sc_err)
			return SF_DECODE_COMPLETE;
		print_prio(td->sc_ret);

		return SF_DECODE_COMPLETE;
	}
}