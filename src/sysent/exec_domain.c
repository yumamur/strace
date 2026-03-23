#include "../ft_common.h"
#include "../ft_print.h"

#include "exec_domain.xlat.h"

SYS_FUNC(personality)
{
	uint32_t persona = td->sc_args[0];

	if (entering(*td))
	{
		FIRST_ARG("persona");
		printflag(persona_types, persona, "PER_???");
		if (persona & PER_MASK)
		{
			persona &= ~PER_MASK;
			print_or();
			printflags(persona_flags, persona, NULL);
		}

		return SF_AFTER_RETURN | SF_PRINT_HEX;
	}

	if (td->sc_err != 0)
		return 0;

	persona = td->sc_ret;
	printflag(persona_types, persona, "PER_???");
	if (persona & PER_MASK)
	{
		persona &= ~PER_MASK;
		print_or();
		printflags(persona_flags, persona, NULL);
	}

	return SF_DECODE_COMPLETE;
}