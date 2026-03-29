#include "../ft_print.h"
#include "../ft_utils.h"
#include "rseq.xlat.h"
#include <linux/rseq.h>

void printrseq(struct s_td *td, __kernel_ulong_t addr)
{
	struct rseq rseq;
	if (umovemem_or_printaddr(td, &rseq, addr, sizeof(rseq)))
		return;

	print_struct_start();

	print_struct_member("cpu_id");
	printflags(rseq_cpu_id_states, rseq.cpu_id, "RSEQ_CPU_ID_???");

	print_next_struct_member("rseq_cs");
	PRINT_LLX(rseq.rseq_cs);

	print_next_struct_member("flags");
	printflags(rseq_cs_flags, rseq.flags, "RSEQ_CS_FLAG_???");

	print_next_struct_member("node_id");
	PRINT_U(rseq.node_id);

	print_next_struct_member("mm_cid");
	PRINT_U(rseq.mm_cid);

	print_struct_end();
}

SYS_FUNC(rseq)
{
	FIRST_ARG("rseq");
	printrseq(td, td->sc_args[0]);

	NEXT_ARG("rseq_len");
	PRINT_U(td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(rseq_flags, td->sc_args[2], "RSEQ_FLAG_???");

	NEXT_ARG("sig");
	PRINT_D(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}