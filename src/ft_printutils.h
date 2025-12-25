#ifndef FT_PRINTUTILS
#define FT_PRINTUTILS

#include "trace.h"

#include "ft_common.h"
#include "ft_string.h"
#include "sysent/xlat.h"

#define MAX_PRINTSTR_LEN 4096

#define TPUTS(x) fputs(x, FT_OUTFILE)
#ifdef DEBUG_ME
#  define EXTEND_ARGNAME(ARGNAME) (ARGNAME "=")
#else
#  define EXTEND_ARGNAME(ARGNAME) ""
#endif

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

#define inprint(buf, fmt, ...) snprintf(buf, sizeof(buf), fmt, __VA_ARGS__)
#define putfmt(fmt, ...)       fputfmt(FT_OUTFILE, fmt, __VA_ARGS__)
#define putfmterr(fmt, ...)    fputfmt(stderr, fmt, __VA_ARGS__)

#define putnull() TPUTS("NULL")

#endif