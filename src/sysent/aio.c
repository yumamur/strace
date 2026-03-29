#include "../ft_print.h"
#include "../ft_utils.h"
#include "aio.xlat.h"
#include <linux/aio_abi.h>

SYS_FUNC(io_setup)
{
	if (entering(*td))
	{
		FIRST_ARG("nr_events");
		PRINT_U(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("ctx_idp");
		if (current_wordsize == 8)
			printnum_addr_ptr64(td, td->sc_args[1]);
		else
			printnum_addr_ptr32(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(io_destroy)
{
	FIRST_ARG("ctx");
	printaddr(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

int printio_event_struct(struct s_td *td, void *ptr, size_t mem_size)
{
	(void) td;
	(void) mem_size;
	print_struct_start();
	PRINT_MEMBER_LLX(*(struct io_event *) ptr, data);
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*(struct io_event *) ptr, obj);
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*(struct io_event *) ptr, res);
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*(struct io_event *) ptr, res2);
	print_struct_end();

	return PRINTARR_STATE_SEP;
}

void printio_event(struct s_td *td, __kernel_ulong_t addr)
{
	struct io_event buf;
	if (!umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		printio_event_struct(td, &buf, sizeof(buf));
}

int decode_io_getevents(struct s_td *td)
{
	if (entering(*td))
	{
		FIRST_ARG("ctx_id");
		printaddr(td->sc_args[0]);

		NEXT_ARG("min_nr");
		__kernel_ulong_t min_nr;
		if (current_wordsize == 4)
			min_nr = (int) td->sc_args[1];
		else
			min_nr = td->sc_args[1];
		PRINT_L(min_nr);

		NEXT_ARG("nr");
		if (current_wordsize == 4)
			min_nr = (int) td->sc_args[2];
		else
			min_nr = td->sc_args[2];
		PRINT_L(min_nr);

		return 0;
	}
	else
	{
		NEXT_ARG("events");
		struct io_event buf;
		printarray(td, (t_printarray_cfg){
						   .printer = printio_event_struct,
						   .start_addr = td->sc_args[3],
						   .pt_buf_var = &buf,
						   .n_var = td->sc_ret,
						   .var_size = sizeof(buf),
					   });

		NEXT_ARG("timeout");
		printtimespec(td, td->sc_args[4]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(io_getevents)
{
	return decode_io_getevents(td);
}

SYS_FUNC(io_pgetevents)
{
	decode_io_getevents(td);

	NEXT_ARG("usig");
	printsigset_kernel(td, td->sc_args[5]);
	return SF_DECODE_COMPLETE;
}

void print_iocb_struct(struct iocb *iocb)
{
	print_struct_start();
	PRINT_MEMBER_LLX(*iocb, aio_data);
	print_struct_member_sep();
	PRINT_MEMBER_LU(*iocb, aio_key);
	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(*iocb, aio_rw_flags, aio_rwf_flags, "RWF_???");
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*iocb, aio_fildes);
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*iocb, aio_buf);
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*iocb, aio_nbytes);
	print_struct_member_sep();
	PRINT_MEMBER_LLX(*iocb, aio_offset);
	print_struct_member_sep();
	print_has_more();
	print_struct_end();
}

int print_iocb(struct s_td *td, void *ptr, size_t mem_size)
{
	__kernel_ulong_t addr;
	if (sizeof(addr) > mem_size)
		addr = *(unsigned int *) ptr;
	else
		addr = *(__kernel_ulong_t *) ptr;

	struct iocb buf;
	if (!umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		print_iocb_struct(&buf);
	return PRINTARR_STATE_SEP;
}

SYS_FUNC(io_submit)
{
	FIRST_ARG("ctx_id");
	printaddr(td->sc_args[0]);

	NEXT_ARG("nr");
	__kernel_ulong_t nr;
	if (current_wordsize == 4)
		nr = (int) td->sc_args[1];
	else
		nr = td->sc_args[1];
	PRINT_L(nr);

	NEXT_ARG("iocbpp");
	__kernel_ulong_t buf;
	printarray(td, (t_printarray_cfg){
					   .printer = print_iocb,
					   .start_addr = td->sc_args[2],
					   .pt_buf_var = &buf,
					   .n_var = nr,
					   .var_size = current_wordsize,
				   });

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(io_cancel)
{
	FIRST_ARG("ctx_id");
	printaddr(td->sc_args[0]);

	NEXT_ARG("iocb");
	struct iocb buf;
	if (!umovemem_or_printaddr(td, &buf, td->sc_args[1], sizeof(buf)))
		print_iocb_struct(&buf);

	NEXT_ARG("result");
	printio_event(td, td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}
