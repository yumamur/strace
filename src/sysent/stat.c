#include "../ft_common.h"
#include "../ft_print.h"
#include "../ft_utils.h"
#include "xlat.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

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

void printstat(t_td *td, struct stat *statbuf)
{
	DONT_FORGET("handle 32bit");
	(void) td;

	print_struct_start();

	print_struct_member("st_dev");
	printdev_t(statbuf->st_dev);

	print_struct_member("st_ino");
	PRINT_ULL(statbuf->st_ino);

	print_struct_member("st_mode");
	printmode_t(statbuf->st_mode);

	print_struct_member("st_nlink");
	PRINT_ULL(statbuf->st_nlink);

	print_struct_member("st_uid");
	PRINT_ULL(statbuf->st_uid);

	print_struct_member("st_gid");
	PRINT_ULL(statbuf->st_gid);

	if (statbuf->st_mode & (__S_IFCHR | __S_IFBLK))
	{
		print_struct_member("st_rdev");
		printdev_t(statbuf->st_rdev);
	}
	else
	{
		print_struct_member("st_size");
		PRINT_ULL(statbuf->st_size);
	}

	print_struct_member("st_blksize");
	PRINT_ULL(statbuf->st_blksize);

	print_struct_member("st_blocks");
	PRINT_ULL(statbuf->st_blocks);

	print_struct_end();
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
		return SC_DECODE_COMPLETE;
	}
}