#include "../ft_print.h"
#include "keyctl.xlat.h"

SYS_FUNC(add_key)
{
	FIRST_ARG("type");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("description");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("payload");
	printnstr(td, td->sc_args[2], td->sc_args[3]);

	NEXT_ARG("size");
	PRINT_LU(td->sc_args[3]);

	NEXT_ARG("keyring");
	printflag(key_spec_ids, td->sc_args[4], "KEY_SPEC_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(request_key)
{
	FIRST_ARG("type");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("description");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("callout_info");
	printstr(td, td->sc_args[2]);

	NEXT_ARG("dest_keyring");
	printflag(key_spec_ids, td->sc_args[3], "KEY_SPEC_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(keyctl)
{
	FIRST_ARG("op");
	printflag_indexed(keyctl_cmds, td->sc_args[0], "KEYCTL_???");

	// I won't
	NEXT_ARG("arg2");
	printaddr(td->sc_args[1]);
	NEXT_ARG("arg3");
	printaddr(td->sc_args[2]);
	NEXT_ARG("arg4");
	printaddr(td->sc_args[3]);
	NEXT_ARG("arg5");
	printaddr(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}