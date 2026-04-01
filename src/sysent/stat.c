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

int fetchstat(t_td *td, __kernel_ulong_t addr, struct stat *dst)
{
	struct stat buf;
	if (umovemem(td, &buf, addr, sizeof(buf)) <= 0)
	{
		print_debug("Invalid td or addr");
		return -1;
	}

	dst->st_dev = zero_extend_signed_to_ull(buf.st_dev);
	dst->st_ino = zero_extend_signed_to_ull(buf.st_ino);
	dst->st_rdev = zero_extend_signed_to_ull(buf.st_rdev);
	dst->st_size = zero_extend_signed_to_ull(buf.st_size);
	dst->st_blocks = zero_extend_signed_to_ull(buf.st_blocks);
	dst->st_blksize = zero_extend_signed_to_ull(buf.st_blksize);
	dst->st_mode = zero_extend_signed_to_ull(buf.st_mode);
	dst->st_nlink = zero_extend_signed_to_ull(buf.st_nlink);
	dst->st_uid = zero_extend_signed_to_ull(buf.st_uid);
	dst->st_gid = zero_extend_signed_to_ull(buf.st_gid);
	dst->st_atime = sign_extend_unsigned_to_l(buf.st_atime);
	dst->st_ctime = sign_extend_unsigned_to_l(buf.st_ctime);
	dst->st_mtime = sign_extend_unsigned_to_l(buf.st_mtime);
	dst->st_atimensec = zero_extend_signed_to_ull(buf.st_atimensec);
	dst->st_ctimensec = zero_extend_signed_to_ull(buf.st_ctimensec);
	dst->st_mtimensec = zero_extend_signed_to_ull(buf.st_mtimensec);

	return 0;
}

void printstat(t_td *td, struct stat *statbuf)
{
	// #warning "handle 32bit";
	(void) td;

	print_struct_start();

	if (is_verbose(*td))
	{
		PRINT_MEMBER(*statbuf, st_dev, printdev_t);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_ino, PRINT_LLU);
		print_struct_member_sep();
	}
	PRINT_MEMBER(*statbuf, st_mode, printmode_t);
	if (is_verbose(*td))
	{
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_nlink, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_uid, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_gid, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_blksize, PRINT_LLU);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_blocks, PRINT_LLU);
		print_struct_member_sep();
	}
	if (statbuf->st_mode & (__S_IFCHR | __S_IFBLK))
	{
		PRINT_MEMBER(*statbuf, st_rdev, printdev_t);
	}
	else
	{
		PRINT_MEMBER(*statbuf, st_size, PRINT_LLU);
	}

	if (is_verbose(*td))
	{
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_atime, PRINT_LLU);
		printtime(statbuf->st_atime, statbuf->st_atimensec);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_atimensec, PRINT_LLU);

		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_mtime, PRINT_LLU);
		printtime(statbuf->st_mtime, statbuf->st_mtimensec);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_mtimensec, PRINT_LLU);

		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_ctime, PRINT_LLU);
		printtime(statbuf->st_ctime, statbuf->st_ctimensec);
		print_struct_member_sep();
		PRINT_MEMBER(*statbuf, st_ctimensec, PRINT_LLU);
	}

	if (!is_verbose(*td))
	{
		print_struct_member_sep();
		print_has_more();
	}

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

SYS_FUNC(fstat)
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

SYS_FUNC(fstatat64)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("statbuf");
	printaddr(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(fstatat_flags, td->sc_args[3], "AT_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(fstat64)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("statbuf");
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(stat64)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("statbuf");
	printaddr(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
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

int decode_readlink(struct s_td *td, unsigned off)
{
	if (entering(*td))
	{
		if (off)
			NEXT_ARG("pathname");
		else
			FIRST_ARG("pathname");
		printpath(td, td->sc_args[off]);

		return 0;
	}

	NEXT_ARG("buf");
	if (td->sc_err)
		printaddr(td->sc_args[off + 1]);
	else
		printnstr(td, td->sc_args[off + 1], td->sc_ret);

	NEXT_ARG("bufsiz");
	PRINT_LLU(td->sc_args[off + 2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(readlink)
{
	return decode_readlink(td, 0);
}

SYS_FUNC(readlinkat)
{
	if (entering(*td))
	{
		FIRST_ARG("dirfd");
		printdirfd(td, td->sc_args[0]);
	}

	return decode_readlink(td, 1);
}

SYS_FUNC(statx)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("filename");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	unsigned flags = td->sc_args[2];
	printflags(at_statx_sync_types, flags & AT_STATX_SYNC_TYPE, 0);
	flags &= ~AT_STATX_SYNC_TYPE;
	if (flags)
	{
		printflags(statx_flags, flags, "STATX_???");
	}

	return SF_DECODE_COMPLETE;
}