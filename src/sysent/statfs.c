#include "../ft_print.h"
#include "../ft_utils.h"
#include "statfs.xlat.h"

#include <sys/statfs.h>

void printstatfs(struct s_td *td, __kernel_ulong_t addr)
{
	struct statfs buf = {};

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_FLAG(buf, f_type, filesystem_types, "???_MAGIC");
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_bsize);
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_blocks);
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_bfree);
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_bavail);
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_files);
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_ffree);
	print_struct_member_sep();

	print_struct_member("f_fsid");
	print_arr_start();
	PRINT_X(buf.f_fsid.__val[0]);
	print_arr_sep();
	PRINT_X(buf.f_fsid.__val[1]);
	print_arr_end();
	print_struct_member_sep();

	PRINT_MEMBER_LLU(buf, f_namelen);
	print_struct_member_sep();
	PRINT_MEMBER_LLU(buf, f_frsize);
	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(buf, f_flags, filesystem_flags, "ST_???");

	print_struct_end();
}

SYS_FUNC(ustat)
{
	FIRST_ARG("dev");
	printdev_t(td->sc_args[0]);

	NEXT_ARG("ubuf");
	printaddr(td->sc_args[1]);

	return 0;
}

SYS_FUNC(statfs)
{
	if (entering(*td))
	{
		FIRST_ARG("pathname");
		printpath(td, td->sc_args[0]);

		return 0;
	}
	else
	{
		NEXT_ARG("buf");
		printstatfs(td, td->sc_args[1]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(statfs64)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("buf");
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fstatfs)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);

		return 0;
	}
	else
	{
		NEXT_ARG("buf");
		printstatfs(td, td->sc_args[1]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(fstatfs64)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("buf");
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}