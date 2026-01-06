#include "../syscall_ent.h"

#define SYS_STUB(n)                                              \
	int __attribute__((weak)) SYS_FUNC_NAME(n)(struct s_td * td) \
	{                                                            \
		(void) td; /* TODO: unimplemented */                     \
		return SF_DECODE_COMPLETE;                               \
	}

#include "xlat_stub.h"