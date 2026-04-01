#include "../ft_print.h"
#include "../ft_utils.h"
#include "file_attr.xlat.h"
#include <linux/fs.h>

void printfile_attr(struct s_td *td, __kernel_ulong_t addr, size_t usize, bool is_get)
{
	struct file_attr buf;
	unsigned long    pagesize = ft_getpagesize();
	size_t           fetch_size = MIN(usize, sizeof(buf));

	// struct size is versioned, there is only ver0 is existing at the time
	if (usize > pagesize || fetch_size < FILE_ATTR_SIZE_VER0)
		return printaddr(addr);

	if (umovemem_or_printaddr(td, &buf, addr, fetch_size))
		return;

	print_struct_start();
	PRINT_MEMBER_FLAGS(buf, fa_xflags, fs_xflags, "FS_XFLAG_???");
	print_struct_member_sep();
	PRINT_MEMBER_U(buf, fa_extsize);
	print_struct_member_sep();
	if (is_get)
	{
		// this field is file_getattr only
		PRINT_MEMBER_U(buf, fa_nextents);
		print_struct_member_sep();
	}
	PRINT_MEMBER_U(buf, fa_projid);
	print_struct_member_sep();
	PRINT_MEMBER_U(buf, fa_cowextsize);

	print_struct_end();
}

SYS_FUNC(file_getattr)
{
	if (entering(*td))
	{
		FIRST_ARG("dfd");
		printdirfd(td, td->sc_args[0]);

		NEXT_ARG("filename");
		printpath(td, td->sc_args[1]);
	}
	else
	{
		NEXT_ARG("fattr");
		printfile_attr(td, td->sc_args[2], td->sc_args[3], true);

		NEXT_ARG("usize");
		PRINT_LU(td->sc_args[3]);

		NEXT_ARG("at_flags");
		printflags(file_attr_flags, td->sc_args[4], "AT_???");
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(file_setattr)
{
	FIRST_ARG("dfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("filename");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("fattr");
	printfile_attr(td, td->sc_args[2], td->sc_args[3], false);

	NEXT_ARG("usize");
	PRINT_LU(td->sc_args[3]);

	NEXT_ARG("at_flags");
	printflags(file_attr_flags, td->sc_args[4], "AT_???");

	return SF_DECODE_COMPLETE;
}