#include "../ft_print.h"
#include "../ft_utils.h"
#include "msg.xlat.h"

void printmsgbuf(struct s_td *td, __kernel_ulong_t addr, size_t len)
{
	struct msgbuf mb;

	print_struct_start();

	PRINT_MEMBER(&mb, mtype, PRINT_D);
	print_struct_member_sep();

	if (umovemem(td, &mb, addr, sizeof(mb)) == -1)
		printaddr(addr);
	else
		printnstr(td, addr + offsetof(struct msgbuf, mtext), len);

	print_struct_end();
}

void printipc_perm_struct(struct ipc_perm *perm)
{
	if (perm)
	{
		PRINT_MEMBER(perm, uid, PRINT_D);
		print_struct_member_sep();
		PRINT_MEMBER(perm, gid, PRINT_D);
		print_struct_member_sep();
		PRINT_MEMBER(perm, mode, printumode);
	}
}

void printmsqid_ds(struct s_td *td, __kernel_ulong_t addr, unsigned int op)
{
	struct msqid_ds ds;

	if (umovemem(td, &ds, addr, sizeof(ds)) == -1)
		return printaddr(addr);

	print_struct_start();

	PRINT_MEMBER_ADDR(&ds, msg_perm, printipc_perm_struct);
	if (op == IPC_SET)
	{
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_qbytes, PRINT_LLU);
	}
	else
	{
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_stime, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_rtime, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_ctime, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_qnum, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_lspid, PRINT_D);
		print_struct_member_sep();
		PRINT_MEMBER(&ds, msg_lrpid, PRINT_D);
	}

	print_struct_end();
}

SYS_FUNC(msgget)
{
	FIRST_ARG("key");
	printkey_t((int32_t) td->sc_args[0]);

	NEXT_ARG("msgflg");
	__kernel_ulong_t flg = td->sc_args[1] & ~0777;
	if (flg)
		printflags(ipc_mode_flags, flg, "MSG_???");
	printumode(td->sc_args[2] & 0777);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(msgsnd)
{
	FIRST_ARG("msqid");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("msgp");
	printmsgbuf(td, td->sc_args[1], td->sc_args[2]);

	NEXT_ARG("msgsz");
	PRINT_U(td->sc_args[2]);

	NEXT_ARG("msgflg");
	printflags(ipc_msg_flags, td->sc_args[3], "MSG_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(msgrcv)
{
	if (entering(*td))
	{
		FIRST_ARG("msqid");
		PRINT_D(td->sc_args[0]);

		return 0;
	}
	else
	{
		NEXT_ARG("msgp");
		printmsgbuf(td, td->sc_args[1], td->sc_args[2]);

		NEXT_ARG("msgsz");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("msgtyp");
		if (current_klongsize < sizeof(long))
			PRINT_D(td->sc_args[3]);
		else
			PRINT_LD(td->sc_args[3]);

		NEXT_ARG("msgflg");
		printflags(ipc_msg_flags, td->sc_args[4], "MSG_???");

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(msgctl)
{
	if (entering(*td))
	{
		FIRST_ARG("msqid");
		PRINT_D(td->sc_args[0]);

		NEXT_ARG("op");
		printflag(msgctl_flags, td->sc_args[1], "MSG_???");

		return 0;
	}
	else
	{
		printmsqid_ds(td, td->sc_args[2], td->sc_args[1]);
		return SF_DECODE_COMPLETE;
	}
}