#include "ft_printutils.h"
#include "ft_utils.h"
#include <linux/fcntl.h>

int printaddr(__kernel_ulong_t addr)
{
	return putnum(zero_extend_signed_to_ull(addr), HEX);
}

int printpath(t_td *td, __kernel_ulong_t addr)
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

int printstr(t_td *td, __kernel_ulong_t addr)
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
			i_dst += inprintnum(&dst[i_dst], n - i_dst, flags, HEX);
		}
	}
	else
	{
		if (flags)
		{
			i_dst += inprintnum(&dst[i_dst], n - i_dst, flags, HEX);
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
				TPUTS("|");
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
			TPUTS("|");
			putnum(flags, HEX);
		}
	}
	else
	{
		if (flags)
		{
			putnum(flags, HEX);
			putcomment(dflt);
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

void printsyscallstart(const char *name)
{
	putfmt("%s(", name);
}

void printsyscallend(t_td *td)
{
	putfmt(") = %lld\n", *(int *) &td->sc_ret);
}

void print_dirfd(t_td *td, int fd)
{
	(void) td;
	if (fd == AT_FDCWD)
		TPUTS("AT_FDCWD");
	else
		putnum(fd, DEC);
}