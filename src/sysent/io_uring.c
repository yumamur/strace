#include "../ft_print.h"
#include "../ft_utils.h"
#include "io_uring.xlat.h"

SYS_FUNC(io_uring_setup)
{
	FIRST_ARG("entries");
	PRINT_U(td->sc_args[0]);

	NEXT_ARG("params");
	// another struct
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(io_uring_enter)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("to_submit");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("min_complete");
	PRINT_U(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(ioring_enter_flags, td->sc_args[3], "IORING_ENTER_???");

	NEXT_ARG("argp");
	printaddr(td->sc_args[4]);

	NEXT_ARG("argsz");
	PRINT_LU(td->sc_args[5]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(io_uring_register)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("opcode");
	unsigned int opc = td->sc_args[1];
	unsigned int use_registered_ring = opc & IORING_REGISTER_USE_REGISTERED_RING;
	opc &= ~IORING_REGISTER_USE_REGISTERED_RING;
	printflag(ioring_register_flags, opc, "IORING_REGISTER_???");
	if (use_registered_ring)
		prints("|IORING_REGISTER_USE_REGISTERED_RING");

	NEXT_ARG("arg");
	printaddr(td->sc_args[2]);

	NEXT_ARG("nr_args");
	PRINT_U(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}