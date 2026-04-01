#include "../ft_print.h"
#include "../ft_utils.h"
#include "lsm.xlat.h"

SYS_FUNC(lsm_get_self_attr)
{
	FIRST_ARG("attr");
	printflag(lsm_attributes, td->sc_args[0], "LSM_ATTR_???");

	NEXT_ARG("ctx");
	// too cumbersome to decode
	printaddr(td->sc_args[1]);

	NEXT_ARG("size");
	printnum_addr_uint32(td, td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(lsm_flags, td->sc_args[3], "LSM_FLAG_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(lsm_set_self_attr)
{
	FIRST_ARG("attr");
	printflag(lsm_attributes, td->sc_args[0], "LSM_ATTR_???");

	NEXT_ARG("ctx");
	printaddr(td->sc_args[1]);

	NEXT_ARG("size");
	PRINT_U(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(lsm_flags, td->sc_args[3], "LSM_FLAG_???");

	return SF_DECODE_COMPLETE;
}

int print_lsm_id(struct s_td *td, void *pt, size_t size)
{
	(void) td;
	(void) size;
	printflag(lsm_ids, *(uint64_t *) pt, "LSM_ID_???");
	return PRINTARR_STATE_SEP;
}

SYS_FUNC(lsm_list_modules)
{
	uint32_t size;

	FIRST_ARG("ids");
	if (entering(*td))
	{
		if (umovemem(td, &size, td->sc_args[1], sizeof(size)) <= 0)
		{
			printaddr(td->sc_args[0]);

			NEXT_ARG("size");
			printnum_addr_uint32(td, td->sc_args[1]);

			NEXT_ARG("flags");
			printflags(lsm_flags, td->sc_args[2], "LSM_FLAG_???");

			return SF_DECODE_COMPLETE;
		}

		td_carry_ulong(td, size);

		return 0;
	}

	uint64_t buf;
	printarray(td, (t_printarray_cfg){
					   .printer = print_lsm_id,
					   .start_addr = td->sc_args[0],
					   .pt_buf_var = &buf,
					   .var_size = sizeof(buf),
					   .n_var = td->sc_ret,
				   });

	NEXT_ARG("size");
	size = (uint32_t) td_carry_get_ulong(td);
	printnum_addr_uint32(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(lsm_flags, td->sc_args[2], "LSM_FLAG_???");

	return SF_DECODE_COMPLETE;
}