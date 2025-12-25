#include "../ft_common.h"
#include "../ft_print.h"
#include "../ft_utils.h"
#include "xlat.h"

void printargv(t_td *td, __kernel_ulong_t addr)
{
	if (!td || !addr)
	{
		printaddr(addr);
		return;
	}

	unsigned int       ct = 0;
	const unsigned int wordbytes = td->abi == ABI_32BIT ? 4U : 8U;
	while (1)
	{
		if (ct == MAX_ARGS)
		{
			TPUTS(", ");
			putcomment("there are more, I'll handle the counting later");
			break;
		}

		union u_addrb addr_buffer;
		if (umovemem(td, addr_buffer.raw, addr, wordbytes))
		{
			printaddr(addr);
			return;
		}

		__kernel_ulong_t tword;
		if (wordbytes == sizeof(addr_buffer.ws64))
			tword = addr_buffer.ws64;
		else
			tword = (__kernel_ulong_t) addr_buffer.ws32;

		if (!ct)
			TPUTS("[");

		if (!tword)
			break;

		if (ct)
			TPUTS(", ");

		printstr(td, tword);
		ct++;
		addr += wordbytes;
	}
	TPUTS("]");
}

SYS_FUNC(execve)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("argv");
	printargv(td, td->sc_args[1]);

	NEXT_ARG("envp");
	printargv(td, td->sc_args[2]);
}

SYS_FUNC(execveat)
{
	FIRST_ARG("dirfd");
	print_dirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("argv");
	printargv(td, td->sc_args[2]);

	NEXT_ARG("envp");
	printargv(td, td->sc_args[3]);

	NEXT_ARG("flags");
	printflags(execveat_flags, td->sc_args[4], "AT_???");
}