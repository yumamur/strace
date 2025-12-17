#include "ft_print.h"
#include <ctype.h>

int puthex(__kernel_ulong_t addr)
{
	static char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
						   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	char        buf[17];
	int         i = 1;

	buf[0] = '#';
	while (i < 16)
	{
		buf[i] = hex[(addr >> (i * 4)) & 0xf];
		i++;
	}
	buf[i] = '\0';
	return TPUTS(buf);
}

int putnum(__kernel_ulong_t num)
{
	char buf[21];
	snprintf(buf, sizeof(buf), "%lu", num);
	return TPUTS(buf);
}

int printaddr(__kernel_ulong_t addr)
{
	return puthex(addr);
}

int printnull()
{
	return TPUTS("NULL");
}

int printquotstr(const char *str, size_t len)
{
	if (!str || !len)
		return 0;

	size_t bufsize = MIN(3 + len * 4, MAX_PRINTSTR_LEN);

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
			else if (i < len)
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
	*bufptr = '\0';
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