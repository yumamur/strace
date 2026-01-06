#ifndef SYSCALL_ENT_H
#define SYSCALL_ENT_H

#include "ft_common.h"
#include <stddef.h>

#include "decoders.h"

struct s_td;

typedef struct s_entry
{
		unsigned int nargs;
		int          (*logger)(struct s_td *);
		const char  *call_name;
		unsigned int traced : 1;
} t_entry;

//
int mark_syscall_to_trace(const char *scname);

// void set_sc_no(struct s_td *td);

void syscallstart(struct s_td *);
void syscallend(struct s_td *);

#endif /* SYSCALL_ENT_H */