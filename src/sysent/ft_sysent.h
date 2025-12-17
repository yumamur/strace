#ifndef FT_SYSENT
#define FT_SYSENT

#include <sys/syscall.h>

struct s_td;

typedef struct s_entry
{
		unsigned int nargs;
		void         (*logger)(struct s_td *);
		const char  *call_name;
} t_entry;

#endif