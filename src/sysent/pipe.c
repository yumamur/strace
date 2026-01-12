#include "../ft_common.h"
#include "../ft_print.h"
#include "pipe.xlat.h"

int print_single_pipefd(struct s_td *td, void *mem)
{
	(void) td;
	printfd(*(int *) mem);
	return 0;
}

void print_pipefd(struct s_td *td, __kernel_ulong_t addr)
{
	int buf;
	printarray(td, print_single_pipefd, addr, &buf, 2, sizeof(int));
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
