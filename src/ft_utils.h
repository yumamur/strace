#ifndef FT__UTILS
#define FT__UTILS

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ABI_64BIT 0
#define ABI_32BIT 1
#define ABI_X32   2

#define FT_OUTFILE  stderr
#define MAX_STR_LEN 4096

#include "trace.h"

#include <sys/syscall.h>
#include <sys/user.h>

int umovestr(t_td *const td, char *laddr, __kernel_ulong_t addr, size_t len);
int detect_abi(struct user_regs_struct *g_regs);

#endif