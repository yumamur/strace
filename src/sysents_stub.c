#include "ft_common.h"

#define SYS_STUB(n)                                                \
	void SYS_FUNC_NAME(n)(struct s_td * td) __attribute__((weak)); \
	void SYS_FUNC_NAME(n)(struct s_td * td)                        \
	{                                                              \
		(void) td; /* TODO: unimplemented */                       \
	}

#define X(name) SYS_STUB(name)
#include "syscalls.def"
#undef X