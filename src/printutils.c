#include "ft_printutils.h"
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>

int inprintnum(char *ptr, size_t n, uint64_t num, enum e_putnum_fmt fmt)
{
	switch (fmt)
	{
	case DEC:
		return snprintf(ptr, n, "%" PRId64, num);
	case UND:
		return snprintf(ptr, n, "%" PRIu64, num);
	case OCT:
		return snprintf(ptr, n, "%" PRIo64, num);
	case OCTO3:
		return snprintf(ptr, n, "%#03" PRIo64, num);
	case HEX:
		return snprintf(ptr, n, "%#" PRIx64, num);
	case UPHEX:
		return snprintf(ptr, n, "%#" PRIX64, num);
	}
	return -1;
}

int putnum(uint64_t num, enum e_putnum_fmt fmt)
{
	static char buf[sizeof(num) * 3];
	inprintnum(buf, sizeof(buf), num, fmt);
	return TPUTS(buf);
}

/**
 * @param src: ascii only, up to `len` OR NULL
 * @param dst: caller is responsible for sufficient size
 */
int inprintquotstr(const char *src, char *dst, size_t len, int truncated)
{
	size_t i_src = 0;
	size_t i_dst = 1;

	if (!src || !dst || !len)
		return -1;
	if (truncated)
		len -= 3;
	while (i_src < len)
	{
		int ch = src[i_src++];
		switch (ch)
		{
		case '\a':
			dst[i_dst++] = '\\';
			dst[i_dst++] = 'a';
			break;
		case '\b':
			dst[i_dst++] = '\\';
			dst[i_dst++] = 'b';
			break;
		case '\t':
			dst[i_dst++] = '\\';
			dst[i_dst++] = 't';
			break;
		case '\n':
			dst[i_dst++] = '\\';
			dst[i_dst++] = 'n';
			break;
		case '\v':
			dst[i_dst++] = '\\';
			dst[i_dst++] = 'v';
			break;
		case '\f':
			dst[i_dst++] = '\\';
			dst[i_dst++] = 'f';
			break;
		case '\\':
			dst[i_dst++] = '\\';
			dst[i_dst++] = '\\';
			break;
		case '\"':
			dst[i_dst++] = '\\';
			dst[i_dst++] = '\"';
			break;
		default:
			if (isprint(ch))
				dst[i_dst++] = ch;
			else if (i_src < len - 1)
			{
				dst[i_dst++] = '\\';
				if (ch >> 3)
				{
					if (ch >> 6)
						dst[i_dst++] = '0' + (ch >> 6);
					dst[i_dst++] = '0' + ((ch >> 3) & 0x7);
				}
				dst[i_dst++] = '0' + (ch & 0x7);
			}
			break;
		}
	}
	dst[0] = '\"';
	if (truncated)
	{
		dst[i_dst++] = '.';
		dst[i_dst++] = '.';
		dst[i_dst++] = '.';
	}
	dst[i_dst++] = '\"';
	dst[i_dst++] = 0;
	return 0;
}

int putquotstr(const char *str, size_t len)
{
	static char *buf;
	size_t       bufsize;

	if (!str || !len)
		return 0;
	bufsize = MIN(len, MAX_PRINTSTR_LEN) * 4 + 3;
	if (!buf)
		buf = malloc(bufsize);
	if (!buf)
		perror_and_die(errno, "Memory allocation failed");
	inprintquotstr(str, buf, len, len > MAX_PRINTSTR_LEN);
	return TPUTS(buf);
}

int fputfmt(FILE *file, const char *fmt, ...)
{
	va_list args;
	int     ret;

	va_start(args, fmt);
	ret = vfprintf(file, fmt, args);
	fflush(file);
	va_end(args);
	return ret;
}

void putcomment(const char *str)
{
	if (str && *str)
	{
		TPUTS("/* ");
		TPUTS(str);
		TPUTS(" */");
	}
}