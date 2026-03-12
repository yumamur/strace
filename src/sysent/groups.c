#include "../ft_print.h"
#include "../ft_utils.h"
#include <linux/limits.h>

void printgrouplist(struct s_td *td, __kernel_ulong_t addr, size_t ng)
{
	__gid_t buf[NGROUPS_MAX] = {};

	if (ng > NGROUPS_MAX || umovemem(td, buf, addr, ng * sizeof(__gid_t)) < 0)
		return printaddr(addr);

	print_arr_start();
	for (size_t i = 0; i < ng; i++)
	{
		PRINT_ID(buf[i]);
		if (i)
			print_arg_sep();
	}
	print_arr_end();
}

SYS_FUNC(getgroups)
{
	if (entering(*td))
	{
		FIRST_ARG("gidsetsize");
		PRINT_D(td->sc_args[0]);

		return 0;
	}
	else
	{
		NEXT_ARG("grouplist");
		printgrouplist(td, td->sc_args[1], td->sc_args[0]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(setgroups)
{
	FIRST_ARG("gidsetsize");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("grouplist");
	printgrouplist(td, td->sc_args[1], td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}