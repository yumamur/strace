#include "../ft_print.h"
#include "../ft_utils.h"
#include "socket.xlat.h"
#include <string.h>

void printsockaddr_struct(void *buf, int addrlen)
{
	if (!buf)
		return printaddr((__kernel_ulong_t) buf);
	const struct sockaddr *sapt = buf;

	print_struct_start();

	print_struct_member("sa_family=");
	printflag(address_families, sapt->sa_family, "AF_???");

	print_struct_member_sep();
	print_has_more();

	print_comment("%p, addrlen=%d", buf, addrlen);
	print_struct_end();
}
void printsockaddr(struct s_td *td, __kernel_ulong_t addr, int addrlen)
{
	union
	{
			struct sockaddr         sa;
			struct sockaddr_storage storage;
			char                    bufpadding[sizeof(struct sockaddr_storage)];
	} buf;

	if (umovemem(td, buf.bufpadding, addr, addrlen) == -1)
	{
		printaddr(addr);
		return;
	}
	printsockaddr_struct(buf.bufpadding, addrlen);
}

void printmsghdr(struct s_td *td, __kernel_ulong_t addr)
{
	struct msghdr buf = {};
}

void printsocket_type(__kernel_ulong_t flags)
{
	const char *type = search_xlat(socket_types, flags & SOCK_MAX);
	flags &= ~SOCK_MAX;
	if (type)
	{
		prints(type);
		if (flags)
			print_or();
	}
	if (flags)
		printflags(socket_type_flags, flags, "SOCK_???");
}

SYS_FUNC(socket)
{
	FIRST_ARG("domain");
	printflag(address_families, td->sc_args[0], "AF_???");

	NEXT_ARG("type");
	printsocket_type(td->sc_args[1]);

	NEXT_ARG("protocol");
	PRINT_ULL(td->sc_args[2]);
	if (td->sc_args[0] < ARRAY_SIZE(af_proto_names))
		print_comment(af_proto_names[td->sc_args[0]]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(connect)
{
	FIRST_ARG("sockfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("addr");
	printsockaddr(td, td->sc_args[1], (int) td->sc_args[2]);

	NEXT_ARG("addrlen");
	PRINT_D(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

int decode_accept(struct s_td *td)
{
	int len_before, len_after;

	if (entering(*td))
	{
		FIRST_ARG("sockfd");
		printfd(td->sc_args[0]);

		if (td->sc_args[1] && td->sc_args[2]
			&& !umovemem(td, &len_before, td->sc_args[2], sizeof(len_before)))
		{
			td_carry_ulong(td, len_before);
			return 0;
		}
		else
		{
			NEXT_ARG("addr");
			printaddr(td->sc_args[1]);

			NEXT_ARG("addr_len");
			printaddr(td->sc_args[2]);

			return SF_DECODE_COMPLETE;
		}
	}
	else
	{
		len_before = (int) td_carry_get_ulong(td);

		if (td->sc_err
			|| umovemem(td, &len_after, td->sc_args[2], sizeof(len_after)) == -1)
		{
			NEXT_ARG("addr");
			printaddr(td->sc_args[1]);

			NEXT_ARG("addr_len");
			printaddr(td->sc_args[2]);
		}
		else
		{
			NEXT_ARG("addr");
			printsockaddr(td, td->sc_args[1], MIN(len_before, len_after));

			NEXT_ARG("addr_len");
			print_arg_start();
			if (len_after != len_before)
			{
				PRINT_D(len_before);
				print_val_change();
			}
			PRINT_D(len_after);
			print_arg_end();
		}
		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(accept)
{
	return decode_accept(td);
}

SYS_FUNC(accept4)
{
	int r = decode_accept(td);

	if (r == SF_DECODE_COMPLETE)
	{
		NEXT_ARG("flags");
		printflags(socket_type_flags, td->sc_args[3], "SOCK_???");
	}
	return r;
}

SYS_FUNC(sendto)
{
	FIRST_ARG("sockfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("buf");
	printnstr(td, td->sc_args[1], td->sc_args[2]);

	NEXT_ARG("size");
	PRINT_ULL(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(msg_flags, td->sc_args[3], "MSG_???");

	NEXT_ARG("dest_addr");
	printsockaddr(td, td->sc_args[4], (int) td->sc_args[5]);

	NEXT_ARG("addr_len");
	PRINT_D(td->sc_args[5]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(recvfrom)
{
}

SYS_FUNC(sendmsg)
{
	FIRST_ARG("sockfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("msg");
	printmsghdr(td, td->sc_args[1]);
}