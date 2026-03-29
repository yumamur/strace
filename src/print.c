#include "ft_print.h"
#include "ft_utils.h"
#include <inttypes.h>
#include <linux/fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/sysmacros.h>

void printaddr(__kernel_ulong_t addr)
{
	if (addr)
		putnum(zero_extend_signed_to_ull(addr), HEX);
	else
		print_null();
}

const char *sprintaddr(__kernel_ulong_t addr)
{
	static char buf[2 + sizeof(__kernel_ulong_t) * 2 + 1];
	inprint(buf, "%#llx", zero_extend_signed_to_ull(addr));
	return buf;
}

int printpath(struct s_td *td, __kernel_ulong_t addr)
{
	if (!addr)
	{
		print_null();
		return -1;
	}

	char str[MAX_PATH_LEN];
	int  null_idx = umovestr(td, str, addr, sizeof(str) - 1);
	if (null_idx < 0)
		putnum(zero_extend_signed_to_ull(addr), HEX);
	else
		putquotstr(str, (size_t) null_idx ?: sizeof(str));
	return null_idx;
}

int printstr(struct s_td *td, __kernel_ulong_t addr)
{
	if (!addr)
	{
		print_null();
		return -1;
	}

	char str[MAX_PRINTSTR_LEN + 3];
	int  null_idx = umovestr(td, str, addr, sizeof(str) - 1);
	if (null_idx < 0)
		putnum(zero_extend_signed_to_ull(addr), HEX);
	else
		putquotstr(str, (size_t) null_idx ?: sizeof(str));
	return null_idx;
}

int printmem(struct s_td *td, __kernel_ulong_t addr, size_t n)
{
	if (!addr)
	{
		print_null();
		return -1;
	}

	char str[MAX_PRINTSTR_LEN + 3];
	bzero(str, sizeof(str));
	int r = umovemem(td, str, addr, MIN(sizeof(str) - 1, n));
	if (r < 0)
		putnum(zero_extend_signed_to_ull(addr), HEX);
	else
		putquotmem(str, (size_t) r ?: sizeof(str));
	return r;
}

int printnstr(struct s_td *td, __kernel_ulong_t addr, size_t n)
{
	if (!addr)
	{
		print_null();
		return -1;
	}

	char str[MAX_PRINTSTR_LEN + 3];
	bzero(str, sizeof(str));
	int read = umovemem(td, str, addr, MIN(sizeof(str) - 1, n));
	if (read < 0)
		putnum(zero_extend_signed_to_ull(addr), HEX);
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
	{
		putnum(flags, HEX);
		print_comment(dflt);
		return -1;
	}

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
		else
			TPUTS("0");
	}

	return ct;
}

int printflag(const t_xlat *xlat, uint64_t flag, const char *dflt)
{
	const char *xval = search_xlat(xlat, flag);
	if (xval)
		TPUTS(xval);
	else
	{
		putnum(flag, HEX);
		if (dflt)
			print_comment(dflt);
	}
	return 0;
}

void printumode(uint64_t mode)
{
	putnum(mode, OCTO3);
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

void print_err_status(int status)
{
	if (!status)
		TPUTS("OK");
	else
		putfmt("%d (%s)", status, get_errname(status < 0 ? -status : status));
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

#define FETCH_PRINT_F(name_, type_, printer_)            \
	FETCH_PRINT_F_DECL(name_)                            \
	{                                                    \
		type_ buf;                                       \
		if (umovemem(td, &buf, addr, sizeof(buf)) == -1) \
		{                                                \
			printaddr(addr);                             \
			return -1;                                   \
		}                                                \
		print_arg_start();                               \
		printer_(buf);                                   \
		print_arg_end();                                 \
		return 0;                                        \
	}

FETCH_PRINT_F(int32, int32_t, PRINT_D)
FETCH_PRINT_F(int64, int64_t, PRINT_LL)
FETCH_PRINT_F(uint32, int32_t, PRINT_U)
FETCH_PRINT_F(uint64, uint64_t, PRINT_LU)
FETCH_PRINT_F(ptr32, unsigned int, printaddr)
FETCH_PRINT_F(ptr64, uint64_t, printaddr)

#undef FETCH_PRINT_F

void printdev_t(__dev_t dev)
{
	unsigned long maj = major(dev);
	unsigned long min = minor(dev);

	prints("makedev");
	print_arg_start();
	PRINT_LLX(maj);
	print_arg_sep();
	PRINT_LLX(min);
	print_arg_end();
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

// void printarray_old(struct s_td     *td,
// 					t_printer        printer,
// 					__kernel_ulong_t start_addr,
// 					void *const      mem_addr,
// 					size_t           nmem,
// 					size_t           mem_size)
// {
// 	if (!start_addr || !mem_size)
// 		return print_null();

// 	const size_t           size = nmem * mem_size;
// 	const __kernel_ulong_t end_addr = start_addr + size;

// 	if (end_addr < start_addr || size / mem_size != nmem)
// 	{
// 		print_debug("size overflow");
// 		return printaddr(start_addr);
// 	}
// 	__kernel_ulong_t cur_addr = start_addr;
// 	int              put_sep = 0;

// 	print_arr_start();
// 	for (cur_addr = start_addr;
// 		 cur_addr < end_addr;
// 		 cur_addr += mem_size)
// 	{
// 		if (put_sep)
// 			print_arr_sep();

// 		if (umovemem(td, mem_addr, cur_addr, mem_size) < 0)
// 		{
// 			printaddr(cur_addr);
// 			if (cur_addr != start_addr)
// 				print_has_more();
// 			break;
// 		}

// 		if (cur_addr < start_addr)
// 		{
// 			print_debug("memory wrap-around");
// 			break;
// 		}

// 		if (printer)
// 		{
// 			put_sep = printer(td, mem_addr, mem_size);
// 			if (put_sep < 0)
// 				break;
// 		}
// 	}
// 	print_arr_end();
// }

void printarray(struct s_td *td, const t_printarray_cfg cfg)
{
	if (!cfg.start_addr || !cfg.var_size)
		return print_null();

	const size_t           size = cfg.n_var * cfg.var_size;
	const __kernel_ulong_t end_addr = cfg.start_addr + size;

	if (end_addr < cfg.start_addr || size / cfg.var_size != cfg.n_var)
	{
		print_debug("size overflow");
		return printaddr(cfg.start_addr);
	}
	__kernel_ulong_t      cur_addr = cfg.start_addr;
	enum e_printarr_state state = PRINTARR_STATE_CONT;

	print_arr_start();
	for (cur_addr = cfg.start_addr;
		 cur_addr < end_addr;
		 cur_addr += cfg.var_size)
	{
		if (state == PRINTARR_STATE_SEP)
			cfg.separator ? TPUTS(cfg.separator) : print_arr_sep();

		if (umovemem(td, cfg.pt_buf_var, cur_addr, cfg.var_size) < 0)
		{
			printaddr(cur_addr);
			if (cur_addr != cfg.start_addr)
				print_has_more();
			break;
		}

		if (cur_addr < cfg.start_addr)
		{
			print_debug("memory wrap-around");
			break;
		}

		if (cfg.printer)
		{
			state = cfg.printer(td, cfg.pt_buf_var, cfg.var_size);
			if (state == PRINTARR_STATE_STOP)
				break;
		}
	}
	print_arr_end();
}

/* some 32-bit syscalls use 2 arguments to pass a 64-bit value,
   these helper functions:
	- consume required number of arguments,
	- print the 64-bit value,
	- return the next argument's index
 */
unsigned int print_llu_arg(struct s_td *td, unsigned int iarg)
{
	uint64_t val;
	if (current_wordsize == 4)
		val = ((uint64_t) td->sc_args[iarg + 1] << 32) | td->sc_args[iarg];
	else
		val = td->sc_args[iarg];
	PRINT_LLU(val);
	return iarg + (current_wordsize == 4 ? 2 : 1);
}

unsigned int print_ll_arg(struct s_td *td, unsigned int iarg)
{
	uint64_t val;
	if (current_wordsize == 4)
	{
		uint32_t low = td->sc_args[iarg], high = td->sc_args[iarg + 1];
		val = ((uint64_t) high << 32) | low;
	}
	else
		val = td->sc_args[iarg];
	PRINT_LL(val);
	return iarg + (current_wordsize == 4 ? 2 : 1);
}

int print_byte(struct s_td *td, void *byte, size_t mem_size)
{
	(void) td;
	for (size_t i = 0; i < mem_size; i++)
	{
		uint8_t cur_byte = ((uint8_t *) byte)[i];
		for (int j = 7; j >= 0; --j)
			TPUTS(cur_byte & (1U << j) ? "1" : "0");
		// if (i != mem_size - 1)
		// print_space();
	}
	// uint8_t cur_byte = *(uint8_t *) byte;
	// for (int i = 7; i >= 0; --i)
	// 	TPUTS(cur_byte & (1U << i) ? "1" : "0");
	return PRINTARR_STATE_SEP;
}