#include "../ft_common.h"
#include "../ft_print.h"
#include "read_write.xlat.h"
#include <linux/fcntl.h>
#include <string.h>
#include <sys/uio.h>

#define _FILE_OFFSET_BITS 64

int printiov_str(struct s_td *td, void *iovp, size_t mem_size)
{
	__kernel_ulong_t base;
	size_t           len;

	if (mem_size < sizeof(struct iovec))
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
	return n > 0 ? PRINTARR_STATE_SEP : PRINTARR_STATE_STOP;
}

int printiov_addr(struct s_td *td, void *iovp, size_t mem_size)
{
	(void) td;
	(void) mem_size;
	printaddr(*(__kernel_ulong_t *) iovp);
	return PRINTARR_STATE_CONT;
}

void printiov(struct s_td     *td,
			  __kernel_ulong_t iovp,
			  size_t           iovcnt,
			  t_printer        p)
{
	struct iovec buf = {};
	printarray(td, (t_printarray_cfg){
					   .printer = p,
					   .start_addr = iovp,
					   .pt_buf_var = &buf,
					   .n_var = iovcnt,
					   .var_size = current_wordsize * 2,
				   });
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

SYS_FUNC(process_vm_readv)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("lvec");
		printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

		NEXT_ARG("liovcnt");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("rvec");
		printiov(td, td->sc_args[3], td->sc_args[4], printiov_str);

		NEXT_ARG("riovcnt");
		PRINT_LLU(td->sc_args[4]);

		NEXT_ARG("flags");
		// should be zero, it's unused for now, but let's print it just in case
		PRINT_U(td->sc_args[5]);
	}

	return 0;
}

SYS_FUNC(process_vm_writev)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("lvec");
	printiov(td, td->sc_args[1], td->sc_args[2], printiov_str);

	NEXT_ARG("liovcnt");
	PRINT_LLU(td->sc_args[2]);

	NEXT_ARG("rvec");
	printiov(td, td->sc_args[3], td->sc_args[4], printiov_str);

	NEXT_ARG("riovcnt");
	PRINT_LLU(td->sc_args[4]);

	NEXT_ARG("flags");
	PRINT_U(td->sc_args[5]);

	return 0;
}

SYS_FUNC(copy_file_range)
{
	FIRST_ARG("fd_in");
	printfd(td->sc_args[0]);

	NEXT_ARG("off_in");
	printnum_addr_int64(td, td->sc_args[1]);

	NEXT_ARG("fd_out");
	printfd(td->sc_args[2]);

	NEXT_ARG("off_out");
	printnum_addr_int64(td, td->sc_args[3]);

	NEXT_ARG("len");
	PRINT_U(td->sc_args[4]);

	NEXT_ARG("flags");
	// not implemented yet
	PRINT_U(td->sc_args[5]);

	return SF_DECODE_COMPLETE;
}
