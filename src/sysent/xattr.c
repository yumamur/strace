#include "../ft_print.h"
#include "../ft_utils.h"
#include "xattr.xlat.h"
#include <errno.h>
#include <stdlib.h>

#define XATTR_SIZE_MAX 65536

static inline int decode_setxattr_is_fd(struct s_td *td, bool is_fd)
{
	FIRST_ARG(is_fd ? "fd" : "path");
	if (is_fd)
		printfd(td->sc_args[0]);
	else
		printpath(td, td->sc_args[0]);

	NEXT_ARG("name");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("value");
	if (td->sc_args[3] > XATTR_SIZE_MAX)
		printaddr(td->sc_args[2]);
	else
		printnstr(td, td->sc_args[2], td->sc_args[3]);

	NEXT_ARG("size");
	PRINT_LLU(td->sc_args[3]);

	NEXT_ARG("flags");
	printflags(xattr_flags, td->sc_args[4], "XATTR_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setxattr)
{
	return decode_setxattr_is_fd(td, false);
}

SYS_FUNC(fsetxattr)
{
	return decode_setxattr_is_fd(td, true);
}

static inline int decode_getxattr_is_fd(struct s_td *td, bool is_fd)
{
	if (entering(*td))
	{
		FIRST_ARG(is_fd ? "fd" : "path");
		if (is_fd)
			printfd(td->sc_args[0]);
		else
			printpath(td, td->sc_args[0]);

		NEXT_ARG("name");
		printstr(td, td->sc_args[1]);
	}
	else
	{
		NEXT_ARG("value");
		if (td->sc_err || td->sc_ret == -1ULL)
			printaddr(td->sc_args[2]);
		else
			printnstr(td, td->sc_args[2], td->sc_ret);

		NEXT_ARG("size");
		PRINT_LLU(td->sc_args[3]);
	}
	return 0;
}

SYS_FUNC(getxattr)
{
	return decode_getxattr_is_fd(td, false);
}

SYS_FUNC(fgetxattr)
{
	return decode_getxattr_is_fd(td, true);
}

static inline void decode_xattr_list(struct s_td     *td,
									__kernel_ulong_t addr,
									__kernel_ulong_t size)
{
	NEXT_ARG("list");
	if (!size || td->sc_err || (ssize_t) td->sc_ret == -1)
		printaddr(addr);
	else
		printnstr(td, addr, td->sc_ret);

	NEXT_ARG("size");
	PRINT_LLU(size);
}

static inline int decode_listxattr_is_fd(struct s_td *td, bool is_fd)
{
	if (entering(*td))
	{
		FIRST_ARG(is_fd ? "fd" : "path");
		if (is_fd)
			printfd(td->sc_args[0]);
		else
			printpath(td, td->sc_args[0]);
	}
	else
		decode_xattr_list(td, td->sc_args[1], td->sc_args[2]);
	return 0;
}

SYS_FUNC(listxattr)
{
	return decode_listxattr_is_fd(td, false);
}

SYS_FUNC(flistxattr)
{
	return decode_listxattr_is_fd(td, true);
}

static inline int decode_removexattr_is_fd(struct s_td *td, bool is_fd)
{
	FIRST_ARG(is_fd ? "fd" : "path");
	if (is_fd)
		printfd(td->sc_args[0]);
	else
		printpath(td, td->sc_args[0]);

	NEXT_ARG("name");
	printstr(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(removexattr)
{
	return decode_removexattr_is_fd(td, false);
}

SYS_FUNC(fremovexattr)
{
	return decode_removexattr_is_fd(td, true);
}

static inline void decode_dfd_path_flags(struct s_td *td)
{
	FIRST_ARG("dfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("path");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(xattr_at_flags, td->sc_args[2], "AT_???");
}

static inline void decode_dfd_path_flags_name(struct s_td *td)
{
	decode_dfd_path_flags(td);

	NEXT_ARG("name");
	printstr(td, td->sc_args[3]);
}

void printxattr_args(struct s_td *td, __kernel_ulong_t addr, size_t usize,
					 struct xattr_args *buf)
{
	print_struct_start();

	print_struct_member("value");
	if (buf->size > XATTR_SIZE_MAX)
		printaddr(buf->value);
	else
		printnstr(td, buf->value, entering(*td) ? buf->size : td->sc_ret);

	print_struct_member_sep();
	PRINT_MEMBER_U(*buf, size);

	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(*buf, flags, xattr_flags, "XATTR_???");

	if (is_verbose(*td) && usize > sizeof(struct xattr_args))
	{
		print_struct_member_sep();
		print_comment("%u extra bytes at %0#lx",
					  (unsigned) (usize - sizeof(struct xattr_args)),
					  addr + sizeof(struct xattr_args));
	}

	print_struct_end();
}

int fetch_xattr_args_or_printaddr(struct s_td *td, __kernel_ulong_t addr, size_t usize,
								  struct xattr_args *buf)
{
	return usize < sizeof(struct xattr_args) ?
			   -1 :
			   umovemem_or_printaddr(td, buf, addr, MIN(sizeof(*buf), usize));
}

SYS_FUNC(setxattrat)
{
	struct xattr_args buf;

	decode_dfd_path_flags_name(td);

	NEXT_ARG("uargs");
	if (!fetch_xattr_args_or_printaddr(td, td->sc_args[4], td->sc_args[5], &buf))
		printxattr_args(td, td->sc_args[4], td->sc_args[5], &buf);

	NEXT_ARG("usize");
	PRINT_LLU(td->sc_args[5]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(getxattrat)
{
	if (entering(*td))
	{
		struct xattr_args buf;

		decode_dfd_path_flags_name(td);

		if (!fetch_xattr_args_or_printaddr(td, td->sc_args[4], td->sc_args[5], &buf))
		{
			if (buf.size)
			{
				struct xattr_args *cpy = malloc(sizeof(struct xattr_args));
				if (!cpy)
					perror_and_die(errno, "malloc");
				td_carry(td, &buf, free);
				return 0;
			}
			printxattr_args(td, td->sc_args[4], td->sc_args[5], &buf);
		}
	}
	else
	{
		struct xattr_args *buf = td_carry_get_voidptr(td);

		NEXT_ARG("uargs");
		printxattr_args(td, td->sc_args[4], td->sc_args[5], buf);

		NEXT_ARG("usize");
		PRINT_LLU(td->sc_args[5]);
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(listxattrat)
{
	if (entering(*td))
		decode_dfd_path_flags(td);
	else
		decode_xattr_list(td, td->sc_args[3], td->sc_args[4]);
	return 0;
}

SYS_FUNC(removexattrat)
{
	decode_dfd_path_flags_name(td);
	return SF_DECODE_COMPLETE;
}