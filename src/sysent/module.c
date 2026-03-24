#include "../ft_common.h"
#include "../ft_print.h"
#include "module.xlat.h"

SYS_FUNC(init_module)
{
	FIRST_ARG("module_image");
	printaddr(td->sc_args[0]);

	NEXT_ARG("size");
	PRINT_LLU(td->sc_args[1]);

	NEXT_ARG("param_values");
	printstr(td, td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(delete_module)
{
	FIRST_ARG("name");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(module_delete_flags, td->sc_args[1], "O_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(finit_module)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("param_values");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(module_init_flags, td->sc_args[2], "MODULE_INIT_???");

	return SF_DECODE_COMPLETE;
}