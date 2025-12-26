#ifndef FT_SYSENT
#define FT_SYSENT

struct s_td;

typedef struct s_entry
{
		unsigned int nargs;
		int          (*logger)(struct s_td *);
		const char  *call_name;
} t_entry;

#define SYS_STUB(n)                                               \
	int SYS_FUNC_NAME(n)(struct s_td * td) __attribute__((weak)); \
	int SYS_FUNC_NAME(n)(struct s_td * td)                        \
	{                                                             \
		(void) td; /* TODO: unimplemented */                      \
		return SC_DECODE_COMPLETE;                                \
	}

#define X(name) SYS_STUB(name)
#include "../syscalls.def"
#undef X

#endif