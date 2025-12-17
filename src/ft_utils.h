#ifndef FT__UTILS
#define FT__UTILS

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ABI_64BIT 0
#define ABI_32BIT 1
#define ABI_X32   2

#define FT_OUTFILE       stderr
#define MAX_PRINTSTR_LEN 4096

#define TPUTS(x) fputs(x, FT_OUTFILE)
#ifdef DEBUG_ME
#define EXTEND_ARGNAME(ARGNAME) (ARGNAME "=")
#else
#define EXTEND_ARGNAME(ARGNAME) ""
#endif

#define FIRST_ARG(argname) TPUTS(EXTEND_ARGNAME(argname))
#define NEXT_ARG(argname)  TPUTS(", " EXTEND_ARGNAME(argname))

#include "ft_common.h"
#include "ft_string.h"
#include "sysent/xlat.h"
#include <stdarg.h>
#include <stdio.h>
#include <sys/user.h>

int umovestr(t_td *const td, char *laddr, __kernel_ulong_t addr, size_t len);
int detect_abi(struct user_regs_struct *g_regs);

int fputfmt(FILE *file, const char *fmt, ...);

#define putfmt(fmt, ...) fputfmt(FT_OUTFILE, fmt, __VA_ARGS__);

int puthex(__kernel_ulong_t hex);
int putnum(__kernel_ulong_t num);

int printpath(t_td *td, __kernel_ulong_t addr);
int printflags(const t_xlat *xlat, uint64_t flags, const char *dflt);

int puthex(__kernel_ulong_t addr);
int putnum(__kernel_ulong_t num);
int printaddr(__kernel_ulong_t addr);
int printnull();
int printquotstr(const char *str, size_t len);
int fputfmt(FILE *file, const char *fmt, ...);

#endif