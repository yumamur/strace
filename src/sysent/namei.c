#include "../ft_common.h"
#include "../ft_print.h"

#include "namei.xlat.h"

SYS_FUNC(rename)
{
	FIRST_ARG("oldpath");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("newpath");
	printstr(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(mkdir)
{
	FIRST_ARG("pathname");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("mode");
	printumode(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

void decode_renameat(struct s_td *td)
{
	FIRST_ARG("olddirfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("oldpath");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("newdirfd");
	printfd(td->sc_args[2]);

	NEXT_ARG("newpath");
	printstr(td, td->sc_args[3]);
}

SYS_FUNC(renameat)
{
	decode_renameat(td);

	return SF_DECODE_COMPLETE;
}
SYS_FUNC(renameat2)
{
	decode_renameat(td);

	NEXT_ARG("flags");
	printflags(rename_flags, td->sc_args[4], "RENAME_???");

	return SF_DECODE_COMPLETE;
}