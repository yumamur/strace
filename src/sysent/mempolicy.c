#include "../ft_print.h"
#include "../ft_utils.h"
#include "mempolicy.xlat.h"

void print_memmode(__kernel_ulong_t arg)
{
	printflag(memory_policies, arg & ~MPOL_MODE_FLAGS, "MPOL_???");
	const unsigned int flags = arg & MPOL_MODE_FLAGS;
	if (flags)
	{
		printf("|");
		printflags(memory_policy_flags, flags, "MPOL_F_???");
	}
}

int print_node(struct s_td *td, void *buf, size_t mem_size)
{
	(void) td;
	if (mem_size == sizeof(__kernel_ulong_t))
	{
		__kernel_ulong_t val = *(const __kernel_ulong_t *) buf;
		printaddr(val);
	}
	else
	{
		unsigned int val = *(unsigned int *) buf;
		printaddr(val);
	}
	return PRINTARR_STATE_CONT;
}

void print_nodemask(struct s_td *td, __kernel_ulong_t addr, __kernel_ulong_t maxnode_arg)
{
	unsigned int     bits_per_long = 8 * current_wordsize;
	__kernel_ulong_t nmem = (maxnode_arg + bits_per_long - 2) / bits_per_long;

	if (nmem < maxnode_arg / bits_per_long || (maxnode_arg && !nmem))
	{
		printaddr(addr);
		return;
	}

	__kernel_ulong_t buf;
	printarray(td, (t_printarray_cfg){
					   .printer = print_node,
					   .start_addr = addr,
					   .pt_buf_var = &buf,
					   .n_var = nmem,
					   .var_size = current_wordsize,
				   });
}

SYS_FUNC(mbind)
{
	FIRST_ARG("start");
	printaddr(td->sc_args[0]);

	NEXT_ARG("len");
	PRINT_LLU(td->sc_args[1]);

	NEXT_ARG("mode");
	print_memmode(td->sc_args[2]);

	NEXT_ARG("nodemask");
	print_nodemask(td, td->sc_args[3], td->sc_args[4]);

	NEXT_ARG("maxnode");
	PRINT_LLU(td->sc_args[5]);

	NEXT_ARG("flags");
	printflags(mbind_flags, td->sc_args[6], "MPOL_F_???");

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(set_mempolicy)
{
	FIRST_ARG("mode");
	print_memmode(td->sc_args[0]);

	NEXT_ARG("nodemask");
	print_nodemask(td, td->sc_args[1], td->sc_args[2]);

	NEXT_ARG("maxnode");
	PRINT_LLU(td->sc_args[3]);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(get_mempolicy)
{
	if (exiting(*td))
	{
		FIRST_ARG("policy");
		int buf_pol;
		if (!umovemem_or_printaddr(td, &buf_pol, td->sc_args[0], sizeof(buf_pol)))
		{
			print_arg_start();
			print_memmode(buf_pol);
			print_arg_end();
		}

		NEXT_ARG("nodemask");
		print_nodemask(td, td->sc_args[1], td->sc_args[2]);

		NEXT_ARG("maxnode");
		PRINT_LLU(td->sc_args[3]);

		NEXT_ARG("addr");
		printaddr(td->sc_args[4]);

		NEXT_ARG("flags");
		printflags(getmempolicy_flags, td->sc_args[5], "MPOL_F_???");
	}
	return 0;
}

SYS_FUNC(migrate_pages)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("maxnode");
	PRINT_LLU(td->sc_args[1]);

	NEXT_ARG("old_nodes");
	print_nodemask(td, td->sc_args[2], td->sc_args[1]);

	NEXT_ARG("new_nodes");
	print_nodemask(td, td->sc_args[3], td->sc_args[1]);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

int print_page_addr(struct s_td *td, void *pt, size_t wordsize)
{
	(void) td;
	__kernel_ulong_t buf;
	if (wordsize == sizeof(buf))
		buf = *(__kernel_ulong_t *) pt;
	else
		buf = *(unsigned int *) pt;

	printaddr(buf);

	return PRINTARR_STATE_SEP;
}

int print_page_status(struct s_td *td, void *pt, size_t wordsize)
{
	(void) td;
	(void) wordsize;

	print_err_status(*(int *) pt);

	return PRINTARR_STATE_SEP;
}

SYS_FUNC(move_pages)
{
	if (entering(*td))
	{
		FIRST_ARG("pid");
		PRINT_ID(td->sc_args[0]);

		NEXT_ARG("nr_pages");
		PRINT_LU(td->sc_args[1]);

		NEXT_ARG("pages");
		__kernel_ulong_t buf;
		printarray(td, (t_printarray_cfg){
						   .printer = print_page_addr,
						   .start_addr = td->sc_args[2],
						   .pt_buf_var = &buf,
						   .n_var = td->sc_args[1],
						   .var_size = current_wordsize,
					   });

		NEXT_ARG("nodes");
		int nodebuf;
		printarray(td, (t_printarray_cfg){
						   .printer = print_node,
						   .start_addr = td->sc_args[3],
						   .pt_buf_var = &nodebuf,
						   .n_var = td->sc_args[1],
						   .var_size = sizeof(nodebuf),
					   });
	}
	else
	{
		NEXT_ARG("status");
		int statusbuf;
		printarray(td, (t_printarray_cfg){
						   .printer = print_page_status,
						   .start_addr = td->sc_args[4],
						   .pt_buf_var = &statusbuf,
						   .n_var = td->sc_args[1],
						   .var_size = sizeof(statusbuf),
					   });

		NEXT_ARG("flags");
		printflags(move_pages_flags, td->sc_args[5], "MPOL_MFLAG_???");
	}

	return 0;
}

SYS_FUNC(set_mempolicy_home_node)
{
	FIRST_ARG("start");
	printaddr(td->sc_args[0]);

	NEXT_ARG("len");
	PRINT_LU(td->sc_args[1]);

	NEXT_ARG("home_node");
	printaddr(td->sc_args[2]);

	NEXT_ARG("flags");
	PRINT_X(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}