#include "../ft_print.h"

SYS_FUNC(listns)
{
	if (entering(*td))
	{
		FIRST_ARG("req");
		printaddr(td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("ns_ids");
		uint64_t buf;
		printarray(td, (t_printarray_cfg){
						   .printer = print_uint64,
						   .start_addr = td->sc_args[1],
						   .n_var = MIN(td->sc_args[2], td->sc_ret),
						   .pt_buf_var = &buf,
						   .var_size = sizeof(buf),
					   });

		NEXT_ARG("nr_ns_ids");
		PRINT_LU(td->sc_args[2]);

		NEXT_ARG("flags");
		// not implemented yet
		PRINT_X(td->sc_args[3]);
	}

	return 0;
}