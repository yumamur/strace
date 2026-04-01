#include "../ft_print.h"
#include "../ft_utils.h"
#include <linux/limits.h>

void printgrouplist(struct s_td *td, __kernel_ulong_t addr,
					size_t ng, size_t id_t_size)
{
	__gid_t buf[NGROUPS_MAX] = {};

	if (ng > NGROUPS_MAX || umovemem(td, buf, addr, ng * sizeof(__gid_t)) < 0)
		return printaddr(addr);

	print_arr_start();
	for (size_t i = 0; i < ng; i++)
	{
		if (id_t_size == sizeof(__kernel_old_gid_t))
		{
			PRINT_ID((__kernel_old_gid_t) buf[i]);
		}
		else
			PRINT_ID(buf[i]);

		if (i)
			print_arg_sep();
	}
	print_arr_end();
}

int do_getgroups(struct s_td *td, size_t id_t_size)
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
		printgrouplist(td, td->sc_args[1], td->sc_args[0], id_t_size);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(getgroups16)
{
	return do_getgroups(td, sizeof(__kernel_old_gid_t));
}

SYS_FUNC(getgroups)
{
	return do_getgroups(td, sizeof(__kernel_gid_t));
}

int do_setgroups(struct s_td *td, size_t id_t_size)
{
	FIRST_ARG("gidsetsize");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("grouplist");
	printgrouplist(td, td->sc_args[1], td->sc_args[0], id_t_size);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setgroups16)
{
	return do_setgroups(td, sizeof(__kernel_old_gid_t));
}

SYS_FUNC(setgroups)
{
	return do_setgroups(td, sizeof(__kernel_gid_t));
}
