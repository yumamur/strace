#include "../ft_print.h"
#include "../ft_utils.h"
#include "kcmp.xlat.h"

void printkcmp_epoll_slot(struct s_td *const td, __kernel_ulong_t addr)
{
	struct kcmp_epoll_slot buf;
	if (!umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_U(buf, efd);
	print_struct_member_sep();
	PRINT_MEMBER_U(buf, tfd);
	print_struct_member_sep();
	PRINT_MEMBER_U(buf, toff);
	print_struct_end();
}

SYS_FUNC(kcmp)
{
	FIRST_ARG("pid1");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("pid2");
	PRINT_ID(td->sc_args[1]);

	NEXT_ARG("type");
	printflag(kcmp_types, td->sc_args[2], "KCMP_???");

	NEXT_ARG("idx1");
	PRINT_LLX(td->sc_args[3]);

	NEXT_ARG("idx2");
	if (td->sc_args[2] == KCMP_EPOLL_TFD)
		printkcmp_epoll_slot(td, td->sc_args[4]);
	else
		PRINT_LLX(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}