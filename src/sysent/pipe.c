#include "../ft_common.h"
#include "../ft_print.h"
#include "pipe.xlat.h"

int print_singlefd(struct s_td *td, void *mem, size_t mem_size)
{
	(void) td;
	(void) mem_size;
	printfd(*(int *) mem);
	return PRINTARR_STATE_CONT;
}

void print_pipefd(struct s_td *td, __kernel_ulong_t addr)
{
	int buf;
	printarray(td, (t_printarray_cfg) {
					   .printer = print_singlefd,
					   .start_addr = addr,
					   .pt_buf_var = &buf,
					   .n_var = 2,
					   .var_size = sizeof(int),
				   });
}

SYS_FUNC(pipe)
{
	if (entering(*td))
		return 0;

	FIRST_ARG("pipefd");
	print_pipefd(td, td->sc_args[0]);
	return SF_DECODE_COMPLETE;
}

SYS_FUNC(pipe2)
{
	if (entering(*td))
		return 0;

	FIRST_ARG("pipefd");
	print_pipefd(td, td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(pipe2_flags, td->sc_args[1], "O_???");
	return SF_DECODE_COMPLETE;
}
