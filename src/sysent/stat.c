#include "../ft_print.h"
#include "../ft_utils.h"
#include "stat.xlat.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define __MODE_T_ACC_MASK (__S_IFMT | __S_ISUID | __S_ISGID | __S_ISVTX)

int printmode_t(__mode_t mode)
{
	const char *ifmt = "";

	if (mode & __S_IFMT)
		ifmt = search_xlat(mode_file_types, mode & __S_IFMT);

	char         buffer[256];
	int          written = snprintf(buffer, sizeof(buffer), "%s", ifmt);
	unsigned int remaining_flags = mode & ~__MODE_T_ACC_MASK;
	if (remaining_flags)
	{
		buffer[written++] = '|';
		snprintflags(buffer + written,
					 sizeof(buffer) - written,
					 mode_protection_bits,
					 remaining_flags,
					 "S_???");
	}

	return prints(buffer);
}

int fetchstat(t_td *td, __kernel_ulong_t addr, struct stat *buf)
{
	if (umovemem(td, buf, addr, sizeof(*buf)) == -1)
	{
		print_debug("Invalid td or addr");
		return -1;
	}
	return 0;
}

#define PRINT_MEMBER(holder_, field_, fun_) \
	print_struct_member(#field_);           \
	fun_((holder_)->field_)

void printstat(t_td *td, struct stat *statbuf)
{
	DONT_FORGET("handle 32bit");
	(void) td;

	print_struct_start();

	PRINT_MEMBER(statbuf, st_dev, printdev_t);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_ino, PRINT_ULL);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_mode, printmode_t);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_nlink, PRINT_ULL);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_uid, PRINT_ULL);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_gid, PRINT_ULL);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_blksize, PRINT_ULL);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_blocks, PRINT_ULL);
	if (statbuf->st_mode & (__S_IFCHR | __S_IFBLK))
	{
		print_struct_member_sep();
		PRINT_MEMBER(statbuf, st_rdev, printdev_t);
	}
	else
	{
		print_struct_member_sep();
		PRINT_MEMBER(statbuf, st_size, PRINT_ULL);
	}

	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_atime, PRINT_ULL);
	printtime(statbuf->st_atime, statbuf->st_atimensec);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_atimensec, PRINT_ULL);

	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_mtime, PRINT_ULL);
	printtime(statbuf->st_mtime, statbuf->st_mtimensec);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_mtimensec, PRINT_ULL);

	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_ctime, PRINT_ULL);
	printtime(statbuf->st_ctime, statbuf->st_ctimensec);
	print_struct_member_sep();
	PRINT_MEMBER(statbuf, st_ctimensec, PRINT_ULL);

	print_struct_end();
}

SYS_FUNC(newfstatat)
{
	if (entering(*td))
	{
		FIRST_ARG("dirfd");
		printdirfd(td, td->sc_args[0]);

		NEXT_ARG("pathname");
		printpath(td, td->sc_args[1]);

		return 0;
	}
	else
	{
		NEXT_ARG("statbuf");
		struct stat buf = {};
		fetchstat(td, td->sc_args[2], &buf);
		printstat(td, &buf);

		if (td->sc_args[3])
		{
			NEXT_ARG("flags");
			printflags(fstatat_flags, td->sc_args[3], "AT_???");
		}

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(newfstat)
{
	if (entering(*td))
	{
		FIRST_ARG("fd");
		printfd(td->sc_args[0]);

		return 0;
	}
	else
	{
		struct stat buf = {};
		NEXT_ARG("statbuf");
		fetchstat(td, td->sc_args[1], &buf);
		printstat(td, &buf);
		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(lstat)
{
	if (entering(*td))
	{
		FIRST_ARG("pathname");
		printpath(td, td->sc_args[0]);

		return 0;
	}
	else
	{
		struct stat buf = {};
		NEXT_ARG("statbuf");
		fetchstat(td, td->sc_args[1], &buf);
		printstat(td, &buf);
		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(stat)
{
	if (entering(*td))
	{
		FIRST_ARG("pathname");
		printpath(td, td->sc_args[0]);

		return 0;
	}
	else
	{
		struct stat buf = {};
		NEXT_ARG("statbuf");
		fetchstat(td, td->sc_args[1], &buf);
		printstat(td, &buf);
		return SF_DECODE_COMPLETE;
	}
}