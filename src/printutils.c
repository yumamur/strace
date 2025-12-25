#include "ft_printutils.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>

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

int putquotstr(const char *str, size_t len)
{
	if (!str || !len)
		return 0;

	size_t bufsize = MIN(3 + len * 4, MAX_STR_LEN);

	char   buf[bufsize];
	char  *bufptr = buf + 1;

	for (size_t i = 0; i < len; i++)
	{
		int ch = str[i];
		switch (ch)
		{
		case '\a':
			*bufptr++ = '\\';
			*bufptr++ = 'a';
			break;
		case '\b':
			*bufptr++ = '\\';
			*bufptr++ = 'b';
			break;
		case '\t':
			*bufptr++ = '\\';
			*bufptr++ = 't';
			break;
		case '\n':
			*bufptr++ = '\\';
			*bufptr++ = 'n';
			break;
		case '\v':
			*bufptr++ = '\\';
			*bufptr++ = 'v';
			break;
		case '\f':
			*bufptr++ = '\\';
			*bufptr++ = 'f';
			break;
		case '\\':
			*bufptr++ = '\\';
			*bufptr++ = '\\';
			break;
		case '\"':
			*bufptr++ = '\\';
			*bufptr++ = '\"';
			break;
		default:
			if (isprint(ch))
				*bufptr++ = ch;
			else if (i < len - 1)
			{
				*bufptr++ = '\\';
				if (ch >> 3)
				{
					if (ch >> 6)
						*bufptr++ = '0' + (ch >> 6);
					*bufptr++ = '0' + ((ch >> 3) & 0x7);
				}
				*bufptr++ = '0' + (ch & 0x7);
			}
			break;
		}
	}
	buf[0] = '\"';
	*bufptr++ = '\"';
	*bufptr++ = 0;
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