#ifndef SHSTK_XLAT_H
#define SHSTK_XLAT_H

#include "xlat.h"

#include <linux/mman.h>

const t_xlat_data shadow_stack_flags_data[] = {
	XLAT(SHADOW_STACK_SET_TOKEN),
	XLAT(SHADOW_STACK_SET_MARKER),
};
WXLAT(shadow_stack_flags);

#endif /* SHSTK_XLAT_H */