#include "../ft_print.h"
#include "futex.xlat.h"

SYS_FUNC(futex)
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
		printtimespec(td, td->sc_args[3]);
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
		printtimespec(td, td->sc_args[3]);
		break;
	case FUTEX_UNLOCK_PI:
	case FUTEX_TRYLOCK_PI:
		break;
	case FUTEX_WAIT_BITSET:
		NEXT_ARG("val");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("timeout");
		printtimespec(td, td->sc_args[3]);

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
		printtimespec(td, td->sc_args[3]);

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