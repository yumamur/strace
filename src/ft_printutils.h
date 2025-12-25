#ifndef FT_PRINTUTILS
#define FT_PRINTUTILS

#include "ft_common.h"
#include "ft_print.h"
#include "ft_string.h"
#include "sysent/xlat.h"
#include "trace.h"

#define MAX_PRINTSTR_LEN 32

#define FIRST_ARG(argname) TPUTS(EXTEND_ARGNAME(argname))
#define NEXT_ARG(argname)  TPUTS(", " EXTEND_ARGNAME(argname))

enum e_putnum_fmt
{
	DEC,
	UND,
	OCT,
	OCTO3,
	HEX,
	UPHEX,
};

int  inprintnum(char *ptr, size_t n, uint64_t num, enum e_putnum_fmt fmt);
int  putnum(uint64_t num, enum e_putnum_fmt fmt);
int  putquotstr(const char *str, size_t len);
int  fputfmt(FILE *file, const char *fmt, ...);
void putcomment(const char *str);

#define inprint(buf, fmt, ...) snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__)
#define putfmt(fmt, ...)       fputfmt(FT_OUTFILE, fmt, ##__VA_ARGS__)
#define putfmterr(fmt, ...)    fputfmt(stderr, fmt, ##__VA_ARGS__)

#define putnull() TPUTS("NULL")

#endif