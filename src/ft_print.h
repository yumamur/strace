#ifndef FT_PRINT
#define FT_PRINT

#include "trace.h"

#include "ft_common.h"
#include "ft_string.h"
#include "sysent/xlat.h"

#define FT_OUTFILE stderr

#define TPUTS(x) fputs(x, FT_OUTFILE)

#ifdef DEBUG_ME
#  define EXTEND_ARGNAME(argname) argname "="
#  define print_debug(msg, ...)   fprintf(FT_OUTFILE, msg, ##__VA_ARGS__)
#else
#  define EXTEND_ARGNAME(argname) ""
#  define print_debug(msg, ...)
#endif

#define FIRST_ARG(argname) TPUTS(EXTEND_ARGNAME(argname))
#define NEXT_ARG(argname)  TPUTS(", " EXTEND_ARGNAME(argname))

#define PRINT_ULL(num) fprintf(FT_OUTFILE, "%llu", zero_extend_signed_to_ull(num))
#define PRINT_X(num)   fprintf(FT_OUTFILE, "%#llx", zero_extend_signed_to_ull(num))

void        printexit(int status);
void        printkillsig(int sig);

void        printsyscallstart(const char *name);
void        printsyscallend(t_td *td);
int         printaddr(__kernel_ulong_t addr);
int         printpath(t_td *td, __kernel_ulong_t addr);
int         printstr(t_td *td, __kernel_ulong_t addr);
int         printnstr(t_td *td, __kernel_ulong_t addr, size_t n);

int         printflags(const t_xlat *xlat, uint64_t flags, const char *dflt);
const char *snprintflags(char         *dst,
						 size_t        n,
						 const t_xlat *xlat,
						 uint64_t      flags,
						 const char   *dflt);
int         printmode(uint64_t mode);
void        printdirfd(t_td *td, int fd);
void        printfd(int fd);
void        printdev_t(__dev_t dev);
void        printtime(time_t sec, unsigned long nsec);

void __attribute__((format(printf, 1, 2)))
printcomment(const char *fmt, ...);

// some tiny functions

static inline int prints(const char *s)
{
	return TPUTS(s);
}

#define FT_SIVP_(fun_, chars_)            \
	static inline void print_##fun_(void) \
	{                                     \
		TPUTS(chars_);                    \
	}

#define FT_SIVP_B(fun_, before_)                   \
	static inline void print_##fun_(const char *s) \
	{                                              \
		TPUTS(before_);                            \
		TPUTS(s);                                  \
	}
#define FT_SIVP_A(fun_, after_)                    \
	static inline void print_##fun_(const char *s) \
	{                                              \
		TPUTS(s);                                  \
		TPUTS(after_);                             \
	}
#define FT_SIVP_BA(fun_, before_, after_)          \
	static inline void print_##fun_(const char *s) \
	{                                              \
		TPUTS(before_);                            \
		TPUTS(s);                                  \
		TPUTS(after_);                             \
	}

FT_SIVP_A(arg_start, "(")
FT_SIVP_(arg_sep, ", ")
FT_SIVP_(arg_end, ")")
FT_SIVP_(or, "|")
FT_SIVP_(arr_start, "[")
FT_SIVP_(arr_end, "]")
FT_SIVP_(struct_start, "{")
FT_SIVP_(struct_end, "}")
FT_SIVP_(comment_start, " /* ")
FT_SIVP_(comment_end, " */")
FT_SIVP_(has_more, "...")
FT_SIVP_A(struct_member, "=")
FT_SIVP_(struct_member_sep, ", ")

#undef FT_SIVP_

#endif