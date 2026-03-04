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

#define FIRST_ARG(argname) TPUTS(EXTEND_ARGNAME(argname))
#define NEXT_ARG(argname)  TPUTS(", " EXTEND_ARGNAME(argname))

#define PRINT_ULL(num) fprintf(FT_OUTFILE, "%llu", zero_extend_signed_to_ull(num))
#define PRINT_LL(num)  fprintf(FT_OUTFILE, "%lld", zero_extend_signed_to_ll(num))
#define PRINT_LD(num)  fprintf(FT_OUTFILE, "%ld", zero_extend_signed_to_l(num))
#define PRINT_LU(num)  fprintf(FT_OUTFILE, "%lu", zero_extend_signed_to_ul(num))
#define PRINT_U(num)   fprintf(FT_OUTFILE, "%u", (unsigned) num)
#define PRINT_D(num)   fprintf(FT_OUTFILE, "%d", (int) num)
#define PRINT_X(num)   fprintf(FT_OUTFILE, "%#llx", zero_extend_signed_to_ull(num))

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

typedef int (*t_printer)(struct s_td *, void *);

void        printexit(int status);
void        printkillsig(int sig);

void        print_syscall_enter(const char *name);
void        print_syscall_return(struct s_td *td);
void        printaddr(__kernel_ulong_t addr);

int         printargs(struct s_td *td);
int         printstr(struct s_td *td, __kernel_ulong_t addr);
int         printnstr(struct s_td *td, __kernel_ulong_t addr, size_t n);
int         printmem(struct s_td *td, __kernel_ulong_t addr, size_t n);

#define printpath printstr

/**
 * printer returns:
 *   == 0 => continue
 *   >  0  => add separator (", ") inbetween elements
 *   <  0  => stop
 */
void        printarray(struct s_td     *td,
					   t_printer        printer,
					   __kernel_ulong_t start_addr,
					   void *const      mem_addr,
					   size_t           nmem,
					   size_t           mem_size);

int         printflag(const t_xlat *xlat, uint64_t flag, const char *dflt);
int         printflags(const t_xlat *xlat, uint64_t flags, const char *dflt);

const char *snprintflags(char         *dst,
						 size_t        n,
						 const t_xlat *xlat,
						 uint64_t      flags,
						 const char   *dflt);

void        printumode(uint64_t mode);
void        printdirfd(struct s_td *td, int fd);
void        printfd(int fd);
void        printdev_t(__dev_t dev);
void        printsigmask(struct s_td *td, __kernel_ulong_t set);
void        printsigset_t(const uint64_t *addr);
void        printsignal(int signum);

void        printiov(struct s_td *td, __kernel_ulong_t iovp, size_t iovcn, t_printer);
int         printiov_str(struct s_td *td, void *iovp);

void        printtime(unsigned long sec, unsigned long nsec);
void        printtimespec_struct(struct timespec *pt);
void        printitimerval_struct(struct itimerval *pt);
void        printtimeval_struct(struct timeval *pt);
void        printitimerval(struct s_td *td, __kernel_ulong_t addr);
void        printtimeval(struct s_td *td, __kernel_ulong_t addr);
void        printtimespec(struct s_td *td, __kernel_ulong_t addr);
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
	fun_((holder_)->field_)

#define PRINT_MEMBER_ADDR(holder_, field_, fun_) \
	print_struct_member(#field_);                \
	fun_(&(holder_)->field_)

#define PRINT_MEMBER_STRQ(holder_, field_) \
	print_struct_member(#field_);          \
	putquotstr((holder_).field_, sizeof((holder_).field_))

#undef FT_SIVP_

#endif