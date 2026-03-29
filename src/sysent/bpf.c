#include "../ft_print.h"
#include "bpf.xlat.h"

SYS_FUNC(bpf)
{
	FIRST_ARG("cmd");
	printflags(bpf_flags, td->sc_args[0], "BPF_???");

	NEXT_ARG("attr");
	// no way I'm decoding this
	printaddr(td->sc_args[1]);

	NEXT_ARG("size");
	PRINT_LLU(td->sc_args[2]);

	return 0;
}