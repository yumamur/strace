#ifndef FT__UTILS
#define FT__UTILS

#include "ft_common.h"
#include <asm/posix_types.h>
#include <stddef.h>

struct s_td;

int umovestr(struct s_td *const td, char *laddr, __kernel_ulong_t addr, size_t len);
int umovemem(struct s_td *const td, void *laddr, __kernel_ulong_t taddr, size_t len);

#endif