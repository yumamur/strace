#ifndef FT_PRINTUTILS
#define FT_PRINTUTILS

#include "ft_common.h"
#include "ft_print.h"
#include "ft_string.h"
#include "sysent/xlat.h"

#ifdef DEBUG_ME
#  define MAX_PRINTSTR_LEN 64
#else
#  define MAX_PRINTSTR_LEN 32
#endif

enum e_putnum_fmt
{
	DEC,
	UND,
	OCT,
	OCTO3,
	HEX,
	UPHEX,
};

int  sprintnum(char *ptr, size_t n, uint64_t num, enum e_putnum_fmt fmt);
void putnum(uint64_t num, enum e_putnum_fmt fmt);

int  putquot_(const char *str, size_t len, bool print_terminating_zero);
#define putquotstr(str_, len_) putquot_(str_, len_, false)
#define putquotmem(mem_, len_) putquot_((const char *) (mem_), len_, true)

int __attribute__((format(printf, 2, 3)))
fputfmt(FILE *file, const char *fmt, ...);

#define inprint(buf, fmt, ...) snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__)
#define putfmt(fmt, ...)       fputfmt(FT_OUTFILE, fmt, ##__VA_ARGS__)
#define putfmterr(fmt, ...)    fputfmt(stderr, fmt, ##__VA_ARGS__)

#endif