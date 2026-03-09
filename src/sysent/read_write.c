#include "../ft_common.h"
#include "../ft_print.h"
#include "read_write.xlat.h"
#include <linux/fcntl.h>
#include <string.h>
#include <sys/uio.h>

int printiov_str(struct s_td *td, void *iovp)
{
	__kernel_ulong_t base;
	size_t           len;
	if (current_wordsize < sizeof(__kernel_ulong_t))
	{
		const unsigned int *const pt = iovp;
		base = pt[0];
		len = pt[1];
	}
	else
	{
		struct iovec *pt = iovp;
		base = (__kernel_ulong_t) pt->iov_base;
		len = pt->iov_len;
	}
	print_struct_start();
	print_struct_member("iov_base");
	int n = printmem(td, base, len);
	print_next_struct_member("iov_len");
	PRINT_LU(len);
	print_struct_end();
	return n;
}

void printiov(struct s_td     *td,
			  __kernel_ulong_t iovp,
			  size_t           iovcnt,
			  t_printer        p)
{
	struct iovec buf = {};
	printarray(td, p, iovp, &buf, iovcnt, current_wordsize * 2);
}

SYS_FUNC(read)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
		return (0);
	}
	else
	{
		NEXT_ARG("buf");
		printnstr(td, td->sc_args[1], td->sc_ret);

		NEXT_ARG("count");
		PRINT_LLU(td->sc_args[2]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(write)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("buf");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("count");
	PRINT_LLU(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(lseek)
{
	FIRST_ARG("fd");
	PRINT_LL(td->sc_args[0]);

	NEXT_ARG("offset");
	PRINT_LL(td->sc_args[1]);

	NEXT_ARG("whence");
	printflags(lseek_whence_values, td->sc_args[2], "UNKNOWN");
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(pread64)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
		return (0);
	}
	else
	{
		NEXT_ARG("buf");
		printnstr(td, td->sc_args[1], td->sc_ret);

		NEXT_ARG("count");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("offset");
		PRINT_LLU(td->sc_args[3]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(pwrite64)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("buf");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("count");
	PRINT_LLU(td->sc_args[2]);

	NEXT_ARG("offset");
	PRINT_LLU(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(readv)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
		return (0);
	}
	else
	{
		NEXT_ARG("iov");
		printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

		NEXT_ARG("iovcnt");
		PRINT_LLU(td->sc_args[2]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(writev)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("iov");
	printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

	NEXT_ARG("iovcnt");
	PRINT_LLU(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sendfile64)
{
	if (entering(*td))
	{
		FIRST_ARG("out_fd");
		printfd(td->sc_args[0]);

		NEXT_ARG("in_fd");
		printfd(td->sc_args[1]);

		NEXT_ARG("offset");
		if (printnum_addr_int64(td, td->sc_args[2]) == -1)
		{
			NEXT_ARG("count");
			PRINT_LLU(td->sc_args[3]);

			return SF_DECODE_COMPLETE;
		}
	}
	else
	{
		if (!td->sc_err && td->sc_ret)
		{
			print_val_change();

			printnum_addr_int64(td, td->sc_args[2]);
		}

		NEXT_ARG("count");
		PRINT_LLU(td->sc_args[3]);
	}
	return 0;
}

int decode_preadv(struct s_td *td)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);
		return 0;
	}
	else
	{
		NEXT_ARG("iov");
		printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

		NEXT_ARG("iovcnt");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("offset");
		if (current_klongsize < KLONG_SIZE)
			PRINT_LL((td->sc_args[4] << 32) | td->sc_args[3]);
		else
			PRINT_LL(td->sc_args[3]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(preadv)
{
	return decode_preadv(td);
}

SYS_FUNC(preadv2)
{
	int r = decode_preadv(td);
	if (r & SF_DECODE_COMPLETE)
	{
		NEXT_ARG("flags");
		printflags(rwf_flags, td->sc_args[5], "RFW_???");
	}
	return r;
}

static void decode_pwritev(struct s_td *td)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("iov");
	printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

	NEXT_ARG("iovcnt");
	PRINT_LLU(td->sc_args[2]);

	NEXT_ARG("offset");
	if (current_klongsize < KLONG_SIZE)
		PRINT_LL((td->sc_args[4] << 32) | td->sc_args[3]);
	else
		PRINT_LL(td->sc_args[3]);
}

SYS_FUNC(pwritev)
{
	decode_pwritev(td);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(pwritev2)
{
	decode_pwritev(td);

	NEXT_ARG("flags");
	printflags(rwf_flags, td->sc_args[5], "RWF_???");

	return SF_DECODE_COMPLETE;
}