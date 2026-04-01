#ifndef FT_UTILS
#define FT_UTILS

#include "ft_common.h"
#include <asm/posix_types.h>
#include <bits/types.h>
#include <stddef.h>

unsigned long ft_getpagesize(void);

struct s_td;

__ssize_t    umovestr(struct s_td *const td, char *laddr, __kernel_ulong_t addr, size_t len);
__ssize_t    umovemem(struct s_td *const td, void *laddr, __kernel_ulong_t taddr, size_t len);
__ssize_t    umovemem_ignore_sc_err(struct s_td *const td, void *laddr, __kernel_ulong_t taddr, size_t len);

size_t       count_set_bits(void *addr, size_t size);
int          umovemem_or_printaddr(struct s_td *const td, void *laddr, __kernel_ulong_t taddr, size_t len);
unsigned int get_ll_arg(struct s_td *td, unsigned long long *pt, unsigned iarg);

// umovemem_or_printaddr((td_), (&obj_), (taddr_), sizeof(obj_))
/* #define umovemem_or_printaddr2(td_, obj_, taddr_) \
	umovemem((td_), (&obj_), (taddr_), (sizeof(obj_))) < 0 ? (printaddr(taddr_), -1) : 0 */

// typedef void (*f_map_ent_free)(void *);

// struct s_map
// {
// 		unsigned cap;
// 		unsigned size;
// 		unsigned key_size;
// 		unsigned val_size;
// 		void    *nodes;
// };

// struct s_map      *map_create(unsigned cap, unsigned key_size, unsigned val_size);
// void              map_destroy(struct s_map);

// bool              map_has_key(struct s_map, uint64_t);
// struct s_map_ent *map_get_node(struct s_map, uint64_t);
// struct s_map_ent *map_add_node(struct s_map, uint64_t, void *, size_t n);
// void              map_del_node(struct s_map, uint64_t);

#endif /* FT_UTILS */