#define __USE_GNU
#include "../ft_print.h"
#include "../ft_utils.h"
#include "mem.xlat.h"
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

unsigned long ft_getpagesize(void)
{
	static unsigned long pagesize = 0;

	if (!pagesize)
	{
		long ret = sysconf(_SC_PAGE_SIZE);
		if (ret < 1)
			perror_and_die(errno, "");
		pagesize = (unsigned long) ret;
	}
	return pagesize;
}

void print_mmap_flags(__kernel_ulong_t flags)
{
	char        buffer[1024];
	const char *map_type_name = search_xlat(mmap_map_types, flags & MAP_TYPE);

	if (!map_type_name)
		map_type_name = "MAP_???";

	int written = snprintf(buffer, sizeof(buffer), "%s", map_type_name);
	flags &= ~MAP_TYPE;

	const unsigned huge_mask = MAP_HUGE_MASK << MAP_HUGE_SHIFT;
	const unsigned huge_size = (flags & huge_mask) >> MAP_HUGE_SHIFT;
	flags &= ~huge_mask;

	if (flags)
	{
		buffer[written++] = '|';
		written = snprintflags(buffer + written,
							   sizeof(buffer) - written,
							   mmap_flags,
							   flags,
							   "MAP_???")
				  - buffer;
	}
	if (huge_size)
	{
		snprintf(buffer + written,
				 sizeof(buffer) - written,
				 "|%u<<MAP_HUGE_SIZE", huge_size);
	}

	prints(buffer);
}

SYS_FUNC(mmap)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("length");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("prot");
	printflags(mmap_prots, td->sc_args[2], "PROT_???");

	NEXT_ARG("flags");
	print_mmap_flags(td->sc_args[3]);

	NEXT_ARG("fd");
	PRINT_D(td->sc_args[4]);

	NEXT_ARG("offset");
	PRINT_X(td->sc_args[5]);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(mmap_pgoff)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("length");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("prot");
	printflags(mmap_prots, td->sc_args[2], "PROT_???");

	NEXT_ARG("flags");
	print_mmap_flags(td->sc_args[3]);

	NEXT_ARG("fd");
	PRINT_D(td->sc_args[4]);

	NEXT_ARG("offset");
	long pagesize = ft_getpagesize();
	PRINT_X(td->sc_args[5] * pagesize);
	print_comment("%li x %li", td->sc_args[5], pagesize);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(mprotect)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("size");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("prot");
	printflags(mprotect_prots, td->sc_args[2], "PROT_???");

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(munmap)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("length");
	PRINT_ULL(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(brk)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(mremap)
{
	FIRST_ARG("old_address");
	printaddr(td->sc_args[0]);

	NEXT_ARG("old_size");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("new_size");
	PRINT_ULL(td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(mremap_flags, td->sc_args[3], "MREMAP_???");

	if (td->sc_args[3] & MREMAP_MAYMOVE)
	{
		NEXT_ARG("new_address");
		printaddr(td->sc_args[4]);
	}

	return SF_DECODE_COMPLETE | SF_PRINT_HEX;
}

SYS_FUNC(msync)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("length");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(msync_flags, td->sc_args[2], "MS_???");

	return SF_DECODE_COMPLETE;
}

int print_mincore_mem(void *buf)
{
	uint8_t val = (*(uint8_t *) buf) & 1;
	return PRINT_ULL(val);
}

SYS_FUNC(mincore)
{
	if (entering(*td))
	{
		FIRST_ARG("addr");
		printaddr(td->sc_args[0]);

		NEXT_ARG("length");
		PRINT_ULL(td->sc_args[1]);

		return 0;
	}
	else
	{
		const unsigned long    pagesize = ft_getpagesize();
		const unsigned long    mask = pagesize - 1;
		const __kernel_ulong_t len = td->sc_args[1];
		const __kernel_ulong_t n = (len / pagesize) + !!(len & mask);

		NEXT_ARG("vec");
		uint8_t buf;
		printarray(
			td,
			print_mincore_mem,
			td->sc_args[2], // vec
			&buf,
			n, // number of pages to fit 'length' amount bytes
			sizeof(buf));
		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(madvise)
{
	FIRST_ARG("addr");
	printaddr(td->sc_args[0]);

	NEXT_ARG("size");
	PRINT_ULL(td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(madvise_values, td->sc_args[2], "MADV_???");

	return SF_DECODE_COMPLETE;
}