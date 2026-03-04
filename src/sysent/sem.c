#include "../ft_print.h"
#include "../ft_utils.h"
#include "sem.xlat.h"

int printsembuf_struct(struct s_td *td, void *sbvp)
{
	(void) td;
	struct sembuf *sbp = (struct sembuf *) sbvp;
	print_struct_start();

	PRINT_MEMBER(sbp, sem_num, PRINT_U);
	PRINT_MEMBER(sbp, sem_op, PRINT_D);
	printflags(semop_flags, sbp->sem_flg, "SEM_???");

	print_struct_end();
	return 1;
}

void printkey_t(int32_t key)
{
	printflag(ipc_key, key, "IPC_???");
}

SYS_FUNC(semget)
{
	FIRST_ARG("key");
	printkey_t((int) (signed) td->sc_args[0]);

	NEXT_ARG("nsems");
	PRINT_D(td->sc_args[1]);

	NEXT_ARG("semflg");
	__kernel_ulong_t flg = td->sc_args[2] & ~0777;
	if (flg)
		printflags(ipc_mode_flags, flg, "SEM_???");
	printumode(td->sc_args[2] & 0777);

	return SF_DECODE_COMPLETE;
}

void decode_semop(struct s_td *td)
{
	FIRST_ARG("semid");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("sops");
	struct sembuf sb;
	printarray(td, printsembuf_struct, td->sc_args[1], &sb, td->sc_args[2], sizeof(sb));

	NEXT_ARG("nsops");
	PRINT_D(td->sc_args[2]);
}

SYS_FUNC(semop)
{
	decode_semop(td);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(semctl)
{
	FIRST_ARG("semid");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("semnum");
	PRINT_D(td->sc_args[1]);

	NEXT_ARG("op");
	printflag(semctl_ops, td->sc_args[2], "SEM_???");

	NEXT_ARG("arg");
	// this also requires some effort to parse
	printaddr(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(semtimedop)
{
	decode_semop(td);

	if (td->sc_args[4])
	{
		NEXT_ARG("timeout");
		printtimespec(td, td->sc_args[4]);
	}

	return SF_DECODE_COMPLETE;
}