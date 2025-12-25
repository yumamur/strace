#ifndef FT_PRINT
#define FT_PRINT

#include "trace.h"

#include "ft_common.h"
#include "ft_string.h"
#include "sysent/xlat.h"

#define FT_OUTFILE stderr

#define TPUTS(x) fputs(x, FT_OUTFILE)

#ifdef DEBUG_ME
#  define EXTEND_ARGNAME(ARGNAME) (ARGNAME "=")
#else
#  define EXTEND_ARGNAME(ARGNAME) ""
#endif

#define FIRST_ARG(argname) TPUTS(EXTEND_ARGNAME(argname))
#define NEXT_ARG(argname)  TPUTS(", " EXTEND_ARGNAME(argname))

void        printexit(int status);
void        printkillsig(int sig);

void        printsyscallstart(const char *name);
void        printsyscallend(t_td *td);

void        putcomment(const char *str);
int         printaddr(__kernel_ulong_t addr);
int         printpath(t_td *td, __kernel_ulong_t addr);
int         printstr(t_td *td, __kernel_ulong_t addr);

int         printflags(const t_xlat *xlat, uint64_t flags, const char *dflt);
const char *snprintflags(char         *dst,
						 size_t        n,
						 const t_xlat *xlat,
						 uint64_t      flags,
						 const char   *dflt);
int         printmode(uint64_t mode);
void        print_dirfd(t_td *td, int fd);

#endif