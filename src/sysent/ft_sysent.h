#ifndef FT_SYSENT
#define FT_SYSENT

struct s_td;

typedef struct s_entry
{
		unsigned int nargs;
		void         (*logger)(struct s_td *);
		const char  *call_name;
} t_entry;

#define SYS_STUB(n)                                                \
	void SYS_FUNC_NAME(n)(struct s_td * td) __attribute__((weak)); \
	void SYS_FUNC_NAME(n)(struct s_td * td)                        \
	{                                                              \
		(void) td; /* TODO: unimplemented */                       \
	}

#define X(name) SYS_STUB(name)
#include "../syscalls.def"
#undef X

#endif