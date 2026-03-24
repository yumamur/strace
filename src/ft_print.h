#ifndef FT_PRINT_H
#define FT_PRINT_H

#include "ft_common.h"
#include "ft_string.h"
#include "sysent/xlat.h"

#define FT_OUTFILE stderr

#define TPUTS(x) fputs(x, FT_OUTFILE)

#ifdef DEBUG_ME
#  define EXTEND_ARGNAME(argname) argname "="
#  define print_debug(msg, ...)   fprintf(FT_OUTFILE, msg, ##__VA_ARGS__)
#  define MAX_PRINTSTR_LEN        64
#else
#  define EXTEND_ARGNAME(argname) ""
#  define print_debug(msg, ...)
#  define MAX_PRINTSTR_LEN 32
#endif

#define MAX_PATH_LEN 4096

#define FIRST_ARG(argname) is_verbose(*td) ? TPUTS(argname "=") : (void) 0
#define NEXT_ARG(argname)  is_verbose(*td) ? TPUTS(", " argname "=") : (void) 0

#define PRINT_LLU(num) fprintf(FT_OUTFILE, "%llu", zero_extend_signed_to_ull(num))
#define PRINT_LL(num)  fprintf(FT_OUTFILE, "%lld", sign_extend_unsigned_to_ll(num))
#define PRINT_L(num)   fprintf(FT_OUTFILE, "%ld", sign_extend_unsigned_to_l(num))
#define PRINT_LU(num)  fprintf(FT_OUTFILE, "%lu", zero_extend_signed_to_ul(num))
#define PRINT_U(num)   fprintf(FT_OUTFILE, "%u", (unsigned) num)
#define PRINT_D(num)   fprintf(FT_OUTFILE, "%d", (int) num)
#define PRINT_X(num)   fprintf(FT_OUTFILE, "%x", (int) num)
#define PRINT_LX(num)  fprintf(FT_OUTFILE, "%#lx", sign_extend_unsigned_to_ll(num))
#define PRINT_LLX(num) fprintf(FT_OUTFILE, "%#llx", zero_extend_signed_to_ull(num))

#define PRINT_ID(num)                            \
	if (sign_extend_unsigned_to_ll(num) == -1LL) \
		PRINT_D(-1);                             \
	else                                         \
		PRINT_U(num);

#define FETCH_PRINT_F_NAME(name_) printnum_addr_##name_
#define FETCH_PRINT_F_DEC(name_) \
	int FETCH_PRINT_F_NAME(name_)(struct s_td * td, __kernel_ulong_t addr)

FETCH_PRINT_F_DEC(int32);
FETCH_PRINT_F_DEC(uint32);
FETCH_PRINT_F_DEC(int64);
FETCH_PRINT_F_DEC(uint64);

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

struct s_td;
struct timespec;
struct timeval;
struct itimerval;
struct timezone;
struct utimbuf;

typedef int (*t_printer)(struct s_td *, void *);

void        printexit(int status);
void        printkillsig(int sig);

void        print_syscall_enter(const char *name);
void        print_syscall_return(struct s_td *td);
void        printaddr(__kernel_ulong_t addr);
const char *sprintaddr(__kernel_ulong_t addr);

int         printargs(struct s_td *td);
int         printpath(struct s_td *td, __kernel_ulong_t addr);
int         printstr(struct s_td *td, __kernel_ulong_t addr);
int         printnstr(struct s_td *td, __kernel_ulong_t addr, size_t n);
int         printmem(struct s_td *td, __kernel_ulong_t addr, size_t n);

/**
 * printer returns:
 *   == 0 => continue
 *   >  0  => add separator (", ") inbetween elements
 *   <  0  => stop
 */
void printarray(struct s_td     *td,
				t_printer        printer,
				__kernel_ulong_t start_addr,
				void *const      mem_addr,
				size_t           nmem,
				size_t           mem_size);

int  printflag(const t_xlat *xlat, uint64_t flag, const char *dflt);
int  printflags(const t_xlat *xlat, uint64_t flags, const char *dflt);

#define printflag_indexed(xlat, flag, dflt)                    \
	if ((unsigned long) flag < ARRAY_SIZE(xlat) && xlat[flag]) \
		TPUTS(xlat[flag]);                                     \
	else                                                       \
	{                                                          \
		putnum(flag, HEX);                                     \
		if (dflt)                                              \
			print_comment(dflt);                               \
	}

const char *snprintflags(char         *dst,
						 size_t        n,
						 const t_xlat *xlat,
						 uint64_t      flags,
						 const char   *dflt);

void        printumode(uint64_t mode);
void        printdirfd(struct s_td *td, int fd);
int         printmode_t(__mode_t mode);
void        printfd(int fd);
void        printdev_t(__dev_t dev);
void        printuser_desc(struct s_td *td, __kernel_ulong_t addr);
void        printsigmask(struct s_td *td, __kernel_ulong_t set);
void        printsigmask_sized(struct s_td *td, __kernel_ulong_t addr, unsigned int sigsetsize);
void        printsigset_t(const uint64_t *addr);
void        printsiginfo(struct s_td *td, __kernel_ulong_t addr);
void        printsignal(int signum);

void        printiov(struct s_td *td, __kernel_ulong_t iovp, size_t iovcn, t_printer);
int         printiov_str(struct s_td *td, void *iovp);

const char *sprinttime(unsigned long sec, unsigned long nsec);
void        printtime(unsigned long sec, unsigned long nsec);
const char *sprinttimespec_struct(struct timespec *pt);
void        printtimespec_struct(struct timespec *pt);
const char *sprintitimerval_struct(struct itimerval *pt);
void        printitimerval_struct(struct itimerval *pt);
const char *sprinttimeval_struct(struct timeval *pt);
void        printtimeval_struct(struct timeval *pt);
const char *sprinttimezone_struct(struct timezone *pt);
void        printtimezone_struct(struct timezone *pt);
void        printutimbuf_struct(struct utimbuf *pt);
const char *sprintitimerval(struct s_td *td, __kernel_ulong_t addr);
void        printitimerval(struct s_td *td, __kernel_ulong_t addr);
const char *sprinttimeval(struct s_td *td, __kernel_ulong_t addr);
void        printtimeval(struct s_td *td, __kernel_ulong_t addr);
const char *sprinttimespec(struct s_td *td, __kernel_ulong_t addr);
void        printtimespec(struct s_td *td, __kernel_ulong_t addr);
const char *sprinttimezone(struct s_td *td, __kernel_ulong_t addr);
void        printtimezone(struct s_td *td, __kernel_ulong_t addr);
void        printutimbuf(struct s_td *td, __kernel_ulong_t addr);
void        printutimbuf_utimes(struct s_td *td, __kernel_ulong_t addr);
void        printtimex64(struct s_td *td, __kernel_ulong_t addr);
void        printtimex32(struct s_td *td, __kernel_ulong_t addr);

void        printrusage(struct s_td *td, __kernel_ulong_t addr);
void        printrlimit(struct s_td *td, __kernel_ulong_t addr);

void        printkey_t(int32_t key);

int __attribute__((format(printf, 1, 2)))
print_flush(const char *fmt, ...);

void __attribute__((format(printf, 1, 2)))
print_comment(const char *fmt, ...);

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

static inline void print_quot_char(unsigned int ch)
{
	fprintf(FT_OUTFILE, "'%c'", ch);
}

FT_SIVP_(space, " ")
FT_SIVP_(null, "NULL")
FT_SIVP_(arg_start, "(")
FT_SIVP_(arg_sep, ", ")
FT_SIVP_(arg_end, ")")
FT_SIVP_(or, "|")
FT_SIVP_(arr_start, "[")
FT_SIVP_(arr_sep, ", ")
FT_SIVP_(arr_end, "]")
FT_SIVP_(struct_start, "{")
FT_SIVP_(struct_end, "}")
FT_SIVP_(comment_start, " /* ")
FT_SIVP_(comment_end, " */")
FT_SIVP_(shift_right, ">>")
FT_SIVP_(shift_left, "<<")
FT_SIVP_(has_more, "...")
FT_SIVP_A(struct_member, "=")
FT_SIVP_BA(next_struct_member, ", ", "=")
FT_SIVP_(struct_member_sep, ", ")
FT_SIVP_(syscall_end, "\n")
FT_SIVP_(new_line, "\n")
FT_SIVP_(val_change, " => ")

#define PRINT_MEMBER(holder_, field_, fun_) \
	print_struct_member(#field_);           \
	fun_((holder_).field_)

#define PRINT_MEMBER_ADDR(holder_, field_, fun_) \
	print_struct_member(#field_);                \
	fun_(&(holder_).field_)

#define PRINT_MEMBER_PTR(holder_, field_) \
	print_struct_member(#field_);         \
	printaddr((unsigned long) (holder_).field_)

#define PRINT_MEMBER_STRQ(holder_, field_) \
	print_struct_member(#field_);          \
	putquotstr((holder_).field_, sizeof((holder_).field_))

#define PRINT_MEMBER_FLAG(holder_, field_, xlat_, dflt_) \
	print_struct_member(#field_);                        \
	printflag(xlat_, (holder_).field_, dflt_)

#define PRINT_MEMBER_FLAGS(holder_, field_, xlat_, dflt_) \
	print_struct_member(#field_);                         \
	printflags(xlat_, (holder_).field_, dflt_)

#define PRINT_MEMBER_D(holder_, field_)   PRINT_MEMBER(holder_, field_, PRINT_D)
#define PRINT_MEMBER_U(holder_, field_)   PRINT_MEMBER(holder_, field_, PRINT_U)
#define PRINT_MEMBER_L(holder_, field_)   PRINT_MEMBER(holder_, field_, PRINT_L)
#define PRINT_MEMBER_LU(holder_, field_)  PRINT_MEMBER(holder_, field_, PRINT_LU)
#define PRINT_MEMBER_LX(holder_, field_)  PRINT_MEMBER(holder_, field_, PRINT_LX)
#define PRINT_MEMBER_LL(holder_, field_)  PRINT_MEMBER(holder_, field_, PRINT_LL)
#define PRINT_MEMBER_LLU(holder_, field_) PRINT_MEMBER(holder_, field_, PRINT_LLU)
#define PRINT_MEMBER_LLX(holder_, field_) PRINT_MEMBER(holder_, field_, PRINT_LLX)

#undef FT_SIVP_

unsigned int print_llu_arg(struct s_td *td, unsigned int iarg);
unsigned int print_ll_arg(struct s_td *td, unsigned int iarg);

#endif