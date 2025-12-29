#ifndef FT__UTILS
#define FT__UTILS

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#include "trace.h"

#include <sys/syscall.h>
#include <sys/user.h>

int        umovestr(t_td *const td, char *laddr, __kernel_ulong_t addr, size_t len);
int        umovemem(t_td *const td, void *laddr, __kernel_ulong_t taddr, size_t len);
enum e_abi detect_abi(struct user_regs_struct *g_regs);

#endif