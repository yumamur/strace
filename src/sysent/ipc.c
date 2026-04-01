#include "../ft_print.h"
#include "ipc.xlat.h"

SYS_FUNC(ipc)
{
	FIRST_ARG("call");

	unsigned int call = td->sc_args[0];
	unsigned int version = call >> 16;

	call &= 0xffff;

	if (version)
	{
		printflag(ipc_versions, td->sc_args[0] >> 16, "IPC_???");
		print_or();
	}

	printflag(ipc_calls, call, NULL);

	NEXT_ARG("first");
	PRINT_X(td->sc_args[1]);
	NEXT_ARG("second");
	PRINT_X(td->sc_args[2]);
	NEXT_ARG("third");
	PRINT_X(td->sc_args[1]);
	NEXT_ARG("ptr");
	PRINT_X(td->sc_args[1]);
	NEXT_ARG("fifth");
	PRINT_X(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}