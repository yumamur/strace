#include "../ft_print.h"
#include "futex.xlat.h"

void do_futex(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	FIRST_ARG("uaddr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("futex_op");
	printflags(futex_ops, td->sc_args[1], "FUTEX_???");

	// I thought about defining a flag for the following operations,
	// but a huge switch..case has less operations

	switch (td->sc_args[1] & FUTEX_CMD_MASK)
	{
	case FUTEX_WAIT:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("timeout");
		printtimespec_fn(td, td->sc_args[3]);
		break;
	case FUTEX_FD:
	case FUTEX_WAKE:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);
		break;
	case FUTEX_REQUEUE:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("val2");
		PRINT_LLU(td->sc_args[3]);

		NEXT_ARG("uaddr2");
		printaddr(td->sc_args[4]);
		break;
	case FUTEX_CMP_REQUEUE:
	case FUTEX_CMP_REQUEUE_PI:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("val2");
		PRINT_LLU(td->sc_args[3]);

		NEXT_ARG("uaddr2");
		printaddr(td->sc_args[4]);

		NEXT_ARG("val3");
		PRINT_LLU(td->sc_args[5]);
		break;
	case FUTEX_WAKE_OP:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("val2");
		PRINT_LLU(td->sc_args[3]);

		NEXT_ARG("uaddr2");
		printaddr(td->sc_args[4]);

		NEXT_ARG("val3");
		PRINT_LLU(td->sc_args[5]);
		break;
	case FUTEX_LOCK_PI:
	case FUTEX_LOCK_PI2:
		NEXT_ARG("timeout");
		printtimespec_fn(td, td->sc_args[3]);
		break;
	case FUTEX_UNLOCK_PI:
	case FUTEX_TRYLOCK_PI:
		break;
	case FUTEX_WAIT_BITSET:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("timeout");
		printtimespec_fn(td, td->sc_args[3]);

		NEXT_ARG("val3");
		printflag(futex_bitsets, td->sc_args[5], NULL);
		break;
	case FUTEX_WAKE_BITSET:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("val3");
		printflag(futex_bitsets, td->sc_args[5], NULL);
		break;
	case FUTEX_WAIT_REQUEUE_PI:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("timeout");
		printtimespec_fn(td, td->sc_args[3]);

		NEXT_ARG("uaddr2");
		printaddr(td->sc_args[4]);
		break;
	default:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("timeout");
		printaddr(td->sc_args[3]);

		NEXT_ARG("uaddr2");
		printaddr(td->sc_args[4]);

		NEXT_ARG("val3");
		PRINT_LLX(td->sc_args[5]);
		break;
	}
}

SYS_FUNC(futex_time32)
{
	do_futex(td, printtimespec32);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(futex_time64)
{
	do_futex(td, printtimespec64);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(set_robust_list)
{
	FIRST_ARG("head");
	printaddr(td->sc_args[0]);

	NEXT_ARG("len");
	PRINT_LLU(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(get_robust_list)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("head");
	if (current_wordsize == 8)
		printnum_addr_ptr64(td, td->sc_args[1]);
	else
		printnum_addr_ptr32(td, td->sc_args[1]);

	NEXT_ARG("len");
	PRINT_LLU(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

void print_futex2_flags(unsigned int flags)
{
	printflag(futex2_size_flags, flags & FUTEX2_SIZE_MASK, NULL);
	flags &= ~FUTEX2_SIZE_MASK;
	if (flags)
	{
		print_or();
		printflags(futex2_flags, flags, "FUTEX2_???");
	}
}

int print_futex_waitv_struct(struct s_td *td, void *pt, size_t size)
{
	struct futex_waitv *buf = pt;

	(void) td;
	(void) size;
	print_struct_start();
	PRINT_MEMBER_LLX(*buf, val);
	print_struct_member_sep();
	PRINT_MEMBER(*buf, uaddr, printaddr);
	print_struct_member_sep();
	PRINT_MEMBER(*buf, flags, print_futex2_flags);
	print_struct_end();

	return PRINTARR_STATE_SEP;
}

SYS_FUNC(futex_waitv)
{
	FIRST_ARG("waiters");
	struct futex_waitv buf = {};
	printarray(td, (t_printarray_cfg){
					   .printer = print_futex_waitv_struct,
					   .start_addr = td->sc_args[0],
					   .n_var = td->sc_args[1],
					   .pt_buf_var = &buf,
					   .var_size = sizeof(buf),
					   .max_vars = 5,
				   });

	NEXT_ARG("nr_futexes");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("flags");
	// not implemented yet
	PRINT_X(td->sc_args[2]);

	NEXT_ARG("timeout");
	printtimespec64(td, td->sc_args[3]);

	NEXT_ARG("clock_id");
	print_clock_id(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(futex_wake)
{
	FIRST_ARG("uaddr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("mask");
	printflag(futex2_bitset, td->sc_args[1], NULL);

	NEXT_ARG("nr");
	PRINT_D(td->sc_args[2]);

	NEXT_ARG("flags");
	print_futex2_flags(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(futex_wait)
{
	FIRST_ARG("uaddr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("val");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("mask");
	printflag(futex2_bitset, td->sc_args[2], NULL);

	NEXT_ARG("flags");
	print_futex2_flags(td->sc_args[3]);

	NEXT_ARG("timeout");
	printtimespec64(td, td->sc_args[4]);

	NEXT_ARG("clock_id");
	print_clock_id(td->sc_args[5]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(futex_requeue)
{
	FIRST_ARG("waiters");
	struct futex_waitv buf = {};
	printarray(td, (t_printarray_cfg){
					   .printer = print_futex_waitv_struct,
					   .start_addr = td->sc_args[0],
					   .n_var = td->sc_args[1],
					   .pt_buf_var = &buf,
					   .var_size = sizeof(buf),
					   .max_vars = 5,
				   });

	NEXT_ARG("flags");
	// not implemented yet
	PRINT_X(td->sc_args[1]);

	NEXT_ARG("nr_wake");
	PRINT_D(td->sc_args[2]);

	NEXT_ARG("nr_requeue");
	PRINT_D(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}