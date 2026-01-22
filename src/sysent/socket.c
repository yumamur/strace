#include "../ft_print.h"
#include "../ft_utils.h"
#include "socket.xlat.h"
#include <string.h>

/**
 *  0 on success, -1 on error/null
 */
int fetch_socklen(struct s_td *td, int *bufp,
				  __kernel_ulong_t sockaddr,
				  __kernel_ulong_t addrlen)
{
	if (td->sc_err || !sockaddr || !addrlen)
		return -1;
	return umovemem(td, bufp, addrlen, sizeof(*bufp));
}

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

	if (umovemem(td, buf.bufpadding, addr, addrlen) != addrlen)
		return printaddr(addr);
	printsockaddr_struct(buf.bufpadding, addrlen);
}

void printmsghdr(struct s_td *td, __kernel_ulong_t addr)
{
	struct msghdr buf = {};

	if (umovemem(td, &buf, addr, sizeof(buf)) <= 0)
		return printaddr(addr);

	print_struct_start();

	print_struct_member("msg_name");
	printnstr(td, (__kernel_ulong_t) buf.msg_name, buf.msg_namelen);

	print_next_struct_member("msg_namelen");
	PRINT_ULL(buf.msg_namelen);

	print_next_struct_member("msg_iov");
	printiov(td, (__kernel_ulong_t) buf.msg_iov, buf.msg_iovlen, printiov_str);

	print_next_struct_member("msg_iovlen");
	PRINT_ULL(buf.msg_iovlen);

	print_struct_end();
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
	FIRST_ARG("family");
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

		if (!fetch_socklen(td, &len_before, td->sc_args[1], td->sc_args[2]))
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

		if (!fetch_socklen(td, &len_after, td->sc_args[1], td->sc_args[2]))
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
		else
		{
			NEXT_ARG("addr");
			printaddr(td->sc_args[1]);

			NEXT_ARG("addr_len");
			printaddr(td->sc_args[2]);
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
	int len_before, len_after;

	if (entering(*td))
	{
		FIRST_ARG("sockfd");
		printfd(td->sc_args[0]);

		if (!fetch_socklen(td, &len_before, td->sc_args[4], td->sc_args[5]))
			td_carry_ulong(td, len_before);
	}
	else
	{
		NEXT_ARG("buf");
		if (td->sc_err)
			printaddr(td->sc_args[1]);
		else
			printnstr(td, td->sc_args[1], td->sc_args[2]);

		NEXT_ARG("size");
		PRINT_ULL(td->sc_args[2]);

		NEXT_ARG("flags");
		printflags(msg_flags, td->sc_args[3], "MSG_???");

		len_before = td_carry_get_ulong(td);

		if (fetch_socklen(td, &len_after, td->sc_args[4], td->sc_args[5]))
		{
			NEXT_ARG("src_addr");
			printaddr(td->sc_args[4]);

			NEXT_ARG("addrlen");
			printaddr(td->sc_args[5]);
		}
		else if (td->sc_err)
		{
			NEXT_ARG("src_addr");
			printaddr(td->sc_args[4]);

			NEXT_ARG("addrlen");
			print_arg_start();
			PRINT_D(len_before);
			print_arg_end();
		}
		else
		{
			NEXT_ARG("src_addr");
			printsockaddr(td, td->sc_args[3], MIN(len_before, len_after));

			NEXT_ARG("addrlen");
			print_arg_start();
			if (len_after != len_before)
			{
				PRINT_D(len_before);
				print_val_change();
			}
			PRINT_D(len_after);
			print_arg_end();
		}
	}
	return 0;
}

SYS_FUNC(sendmsg)
{
	FIRST_ARG("sockfd");
	printfd(td->sc_args[0]);

	NEXT_ARG("msg");
	printmsghdr(td, td->sc_args[1]);
	return 0;
}
