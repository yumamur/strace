#include "../ft_print.h"
#include "../ft_utils.h"
#include "mqueue.xlat.h"

#include <linux/mqueue.h>

void printmq_attr(struct s_td *td, __kernel_ulong_t addr)
{
	struct mq_attr buf;

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_FLAGS(buf, mq_flags, mq_attr_flags, "O_???");
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, mq_maxmsg);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, mq_msgsize);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, mq_curmsgs);
	print_struct_end();
}

SYS_FUNC(mq_open)
{
	FIRST_ARG("name");
	printstr(td, td->sc_args[0]);

	NEXT_ARG("oflag");
	print_open_flags(td->sc_args[1]);

	if (td->sc_args[1] & (O_CREAT | __O_TMPFILE))
	{
		NEXT_ARG("mode");
		printumode(td->sc_args[2]);

		NEXT_ARG("attr");
		printmq_attr(td, td->sc_args[3]);
	}

	return SF_DECODE_COMPLETE;
}

void decode_mq_timedsend(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	FIRST_ARG("mqdes");
	printfd(td->sc_args[0]);

	NEXT_ARG("msg_ptr");
	printnstr(td, td->sc_args[1], td->sc_args[2]);

	NEXT_ARG("msg_len");
	PRINT_LLU(td->sc_args[2]);

	NEXT_ARG("msg_prio");
	PRINT_LLU((unsigned int) td->sc_args[3]);

	NEXT_ARG("abs_timeout");
	printtimespec_fn(td, td->sc_args[4]);
}

SYS_FUNC(mq_timedsend_time32)
{
	decode_mq_timedsend(td, printtimespec32);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(mq_timedsend_time64)
{
	decode_mq_timedsend(td, printtimespec64);
	return SF_DECODE_COMPLETE;
}

int do_mq_timedreceive(struct s_td *td, typeof(printtimespec32) printtimespec_fn)
{
	if (entering(*td))
	{
		FIRST_ARG("mqdes");
		printfd(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("msg_ptr");
		if (td->sc_err)
			printaddr(td->sc_args[1]);
		else
			printnstr(td, td->sc_args[1], td->sc_args[2]);

		NEXT_ARG("msg_len");
		PRINT_LLU(td->sc_args[2]);

		NEXT_ARG("msg_prio");
		printnum_addr_int32(td, td->sc_args[3]);

		NEXT_ARG("abs_timeout");
		printtimespec_fn(td, td->sc_args[4]);
	}

	return 0;
}

SYS_FUNC(mq_timedreceive_time32)
{
	return do_mq_timedreceive(td, printtimespec32);
}

SYS_FUNC(mq_timedreceive_time64)
{
	return do_mq_timedreceive(td, printtimespec64);
}

SYS_FUNC(mq_notify)
{
	FIRST_ARG("mqdes");
	printfd(td->sc_args[0]);

	NEXT_ARG("u_notification");
	// I'm not parsing sigevent
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(mq_getsetattr)
{
	if (entering(*td))
	{
		FIRST_ARG("mqdes");
		printfd(td->sc_args[0]);

		NEXT_ARG("newattr");
		printmq_attr(td, td->sc_args[1]);
	}
	else
	{
		NEXT_ARG("oldattr");
		if (td->sc_err)
			printaddr(td->sc_args[2]);
		else
			printmq_attr(td, td->sc_args[2]);
	}

	return 0;
}