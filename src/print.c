#include "ft_printutils.h"
#include "ft_utils.h"
#include "syscall_ent.h"
#include <inttypes.h>
#include <linux/fcntl.h>
#include <string.h>
#include <sys/sysmacros.h>

int printaddr(__kernel_ulong_t addr)
{
	if (addr)
		return putnum(zero_extend_signed_to_ull(addr), HEX);
	else
		return putnull();
}

int printpath(struct s_td *td, __kernel_ulong_t addr)
{
	char path[4096];

	if (!addr)
		return -1;

	int null_idx = umovestr(td, path, addr, sizeof(path) - 1);
	if (null_idx < 0)
		putnum(addr, HEX);
	else
		putquotstr(path, (size_t) null_idx ?: sizeof(path));
	return null_idx;
}

int printstr(struct s_td *td, __kernel_ulong_t addr)
{
	if (!addr)
		return -1;

	char str[MAX_PRINTSTR_LEN + 2];
	int  null_idx = umovestr(td, str, addr, sizeof(str) - 1);
	if (null_idx < 0)
		putnum(addr, HEX);
	else
		putquotstr(str, (size_t) null_idx ?: sizeof(str));
	return null_idx;
}

int printnstr(struct s_td *td, __kernel_ulong_t addr, size_t n)
{
	if (!addr)
		return -1;

	char str[MAX_PRINTSTR_LEN + 2];
	int  read = umovemem(td, str, addr, MIN(sizeof(str) - 1, n));
	if (read < 0)
		putnum(addr, HEX);
	else
		putquotstr(str, (size_t) read ?: sizeof(str));
	return read;
}

const char *snprintflags(char         *dst,
						 size_t        n,
						 const t_xlat *xlat,
						 uint64_t      flags,
						 const char   *dflt)
{
	if (!dst || !n || !xlat || !xlat->data || !xlat->size || !flags)
		return NULL;

	int i_dst = 0;
	for (unsigned int i = 0; i < xlat->size; i++)
	{
		uint64_t val = xlat->data[i].val;
		if (xlat->data[i].name
			&& (flags == val || (val && val == (flags & val))))
		{
			if (i_dst)
				dst[i_dst++] = '|';
			i_dst += stpcpy(&dst[i_dst], xlat->data[i].name) - &dst[i_dst];
			flags &= ~val;
		}
		if (!flags)
			break;
	}

	if (i_dst)
	{
		if (flags)
		{
			dst[i_dst++] = '|';
			i_dst += sprintnum(&dst[i_dst], n - i_dst, flags, OCTO3);
		}
	}
	else
	{
		if (flags)
		{
			i_dst += sprintnum(&dst[i_dst], n - i_dst, flags, OCTO3);
			snprintf(&dst[i_dst], n - i_dst, "/* %s */", dflt);
		}
		else if (dflt)
			dst[i_dst++] = 0;
	}

	dst[i_dst] = '\0';

	return dst;
}

int printflags(const t_xlat *xlat, uint64_t flags, const char *dflt)
{
	if (!(xlat && xlat->data && xlat->size))
		return -1;

	if (!flags)
		return 0;

	int ct = 0;
	for (unsigned int i = 0; i < xlat->size; i++)
	{
		uint64_t val = xlat->data[i].val;
		if (xlat->data[i].name
			&& (flags == val || (val && val == (flags & val))))
		{
			if (ct++)
				print_or();
			TPUTS(xlat->data[i].name);
			// uncheck processed flag
			flags &= ~val;
		}
		if (!flags)
			break;
	}

	// should not enter here under normal circumstances
	if (ct)
	{
		if (flags)
		{
			print_or();
			putnum(flags, HEX);
		}
	}
	else
	{
		if (flags)
		{
			putnum(flags, HEX);
			print_comment(dflt);
		}
	}

	return ct;
}

int printmode(uint64_t mode)
{
	return putnum(mode, OCTO3);
}

void printexit(int status)
{
	putfmt("+++ exited with %d +++\n", status);
}

void printkillsig(int sig)
{
	putfmt("+++ killed by signal %d +++\n", sig);
}

void print_syscall_enter(const char *name)
{
	putfmt("%s(", name);
}

void print_syscall_return(struct s_td *td)
{
	if (td->sc_err)
	{
		putfmt(") = -1 %s", get_errmsg(-td->sc_err));
	}
	else if (td->flags & SF_PRINT_HEX)
		putfmt(") = %#0" PRIx64, *(__kernel_ulong_t *) &td->sc_ret);
	else
		putfmt(") = %" PRIu64, *(__kernel_ulong_t *) &td->sc_ret);
}

void printdirfd(struct s_td *td, int fd)
{
	(void) td;
	if (fd == AT_FDCWD)
		TPUTS("AT_FDCWD");
	else
		putnum(fd, DEC);
}

void printfd(int fd)
{
	putnum(fd, DEC);
}

void printdev_t(__dev_t dev)
{
	unsigned long maj = major(dev);
	unsigned long min = minor(dev);

	prints("makedev");
	print_arg_start();
	PRINT_X(maj);
	print_arg_sep();
	PRINT_X(min);
	print_arg_end();
}

void printtime(time_t sec, unsigned long nsec)
{
	static char      buf[sizeof("2000-10-11T17:17:17.171717171+0300") + 5];

	size_t           pos = 0;
	const struct tm *tp = localtime(&sec);

	pos = strftime(buf, sizeof(buf), "%FT%T", tp);
	if (!pos)
		return;

	if (nsec)
		pos += snprintf(buf + pos, sizeof(buf) - pos, ".%09lu", nsec);
	strftime(buf + pos, sizeof(buf) - pos, "%z", tp);

	print_comment("%s", buf);
}

int printargs(struct s_td *td)
{
	unsigned int n = td->entry->nargs;
	for (size_t i = 0; i < n; i++)
	{
		if (i)
			NEXT_ARG("");
		else
			FIRST_ARG("");
		printaddr(td->sc_args[i]);
	}
	return SF_DECODE_COMPLETE;
}

void printarray(struct s_td     *td,
				t_printer        printer,
				__kernel_ulong_t start_addr,
				void *const      mem_addr,
				size_t           nmem,
				size_t           mem_size)
{
	const size_t           size = nmem * mem_size;
	const __kernel_ulong_t end_addr = start_addr + size;

	if (end_addr < start_addr || size / mem_size != nmem)
	{
		print_debug("size overflow");
		printaddr(start_addr);
		return;
	}
	__kernel_ulong_t cur_addr = start_addr;
	int              read_bytes;
	int              logged = 0;

	print_arr_start();
	for (cur_addr = start_addr;
		 cur_addr < end_addr;
		 cur_addr += mem_size)
	{
		if (logged)
			print_arg_sep();

		read_bytes = umovemem(td, mem_addr, cur_addr, mem_size);

		if (read_bytes < 0)
		{
			printaddr(cur_addr);
			if (cur_addr != start_addr)
				print_has_more();
			break;
		}

		if (cur_addr < start_addr)
		{
			print_debug("memory wrap-around");
			break;
		}

		if (printer)
		{
			if (printer(mem_addr))
				logged = 1;
		}
	}
	print_arr_end();
}