#include "../ft_print.h"

#include "shm.xlat.h"

void printshm_flags(__kernel_ulong_t flags)
{
	const unsigned int acc_mode = flags & 0777;
	flags &= ~0777;

	const unsigned huge_mask = SHM_HUGE_MASK << SHM_HUGE_SHIFT;
	const unsigned huge_size = (flags & huge_mask) >> SHM_HUGE_SHIFT;
	flags &= ~huge_mask;

	if (flags)
		printflags(shm_resource_flags, flags, NULL);

	if (huge_size)
	{
		if (flags)
			print_or();
		PRINT_ULL(huge_size);
		print_shift_left();
		PRINT_ULL(SHM_HUGE_SHIFT);
	}

	if (flags || huge_size)
		print_or();
	printmode(acc_mode);
}

SYS_FUNC(shmget)
{
	FIRST_ARG("key");
	printflag(ipc_key, td->sc_args[0], NULL);

	NEXT_ARG("size");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("shmflg");
	printshm_flags(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(shmat)
{
	FIRST_ARG("shmid");
	PRINT_LL(td->sc_args[0]);

	NEXT_ARG("shmaddr");
	printaddr(td->sc_args[1]);

	NEXT_ARG("shmflg");
	printshm_flags(td->sc_args[2]);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(shmctl)
{
	FIRST_ARG("shmid");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("op");
	printflags(shmctl_ops, td->sc_args[1], NULL);

	/**
	 * I'm no longer printing every other struct.
	 */
	NEXT_ARG("buf");
	printaddr(td->sc_args[2]);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}