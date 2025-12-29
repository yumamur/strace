#include "../ft_common.h"
#include "../ft_print.h"
#include "../ft_utils.h"
#include "xlat.h"
#include <inttypes.h>
#include <stdbool.h>

size_t readv(t_td *td, __kernel_ulong_t addr, _Bool should_print)
{
	if (!td || !addr)
	{
		print_debug("Invalid td or addr");
		if (should_print)
			printaddr(addr);
		return 0;
	}

	size_t           ct = 0;
	const size_t     wordbytes = td->abi == ABI_32BIT ? 4U : 8U;
	__kernel_ulong_t prev_addr = 0;
	union u_addrb    addr_buffer;
	while (1)
	{
		if (addr < prev_addr)
		{
			if (should_print)
				printaddr(addr);
			print_debug("memory wrap-around");
			break;
		}

		if (umovemem(td, addr_buffer.raw, addr, wordbytes) == -1)
		{
			if (should_print)
				printaddr(addr);
			break;
		}

		__kernel_ulong_t tword;
		if (wordbytes == sizeof(addr_buffer.ws64))
			tword = addr_buffer.ws64;
		else
			tword = (__kernel_ulong_t) addr_buffer.ws32;

		if (!tword)
			break;

		if (should_print)
		{
			if (!ct)
				print_arr_start();
			else if (ct >= MAX_ARGS)
			{
				print_arg_sep();
				printcomment("I probably should keep counting");
				break;
			}
			else
				print_arg_sep();

			printstr(td, tword);
		}
		ct++;
		prev_addr = addr;
		addr += wordbytes;
	}
	if (should_print)
		print_arr_end();
	return ct;
}

void printenvp(t_td *td, __kernel_ulong_t addr)
{
	size_t ct = readv(td, addr, false);
	printaddr(addr);
	printcomment("%" PRIu64 " variables", ct);
}

SYS_FUNC(execve)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("argv");
	readv(td, td->sc_args[1], true);

	NEXT_ARG("envp");
	printenvp(td, td->sc_args[2]);

	return SC_DECODE_COMPLETE;
}

SYS_FUNC(execveat)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("argv");
	readv(td, td->sc_args[2], true);

	NEXT_ARG("envp");
	printenvp(td, td->sc_args[3]);

	NEXT_ARG("flags");
	printflags(execveat_flags, td->sc_args[4], "AT_???");

	return SC_DECODE_COMPLETE;
}