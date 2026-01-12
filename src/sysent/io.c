#include "../ft_common.h"
#include "../ft_print.h"
#include <linux/fcntl.h>
#include <string.h>
#include <sys/uio.h>

void printiov(struct s_td     *td,
			  __kernel_ulong_t iovp,
			  __kernel_ulong_t iovcnt,
			  t_printer        printer)
{
	struct iovec buf = {};
	printarray(td, printer, iovp, &buf, iovcnt, sizeof(buf));
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
		PRINT_ULL(td->sc_args[2]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(pread)
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
		PRINT_ULL(td->sc_args[2]);

		NEXT_ARG("offset");
		PRINT_ULL(td->sc_args[3]);

		return SF_DECODE_COMPLETE;
	}
}

int print_iov_str(struct s_td *td, void *iov)
{
	struct iovec *pt = iov;
	return printnstr(td, (__kernel_ulong_t) pt->iov_base, pt->iov_len);
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
		printiov(td,
				 td->sc_args[1],
				 td->sc_args[2],
				 print_iov_str);

		NEXT_ARG("iovcnt");
		PRINT_ULL(td->sc_args[2]);

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
	PRINT_ULL(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(pwrite)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("buf");
	printstr(td, td->sc_args[1]);

	NEXT_ARG("count");
	PRINT_ULL(td->sc_args[2]);

	NEXT_ARG("offset");
	PRINT_ULL(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(writev)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("iov");
	printiov(td,
			 td->sc_args[1],
			 td->sc_args[2],
			 print_iov_str);

	NEXT_ARG("iovcnt");
	PRINT_ULL(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}