#include "xlat.h"
#include <stdlib.h>

int xlat_val_comparator(const void *x1_pt, const void *x2_pt)
{
	const t_xlat_data *x1 = x1_pt, *x2 = x2_pt;
	return x1->val == x2->val ? 0 : (x1->val > x2->val ? 1 : -1);
}

const char *search_xlat(const t_xlat *xlat, const uint64_t val)
{
	if (!xlat)
		return 0;
	switch (xlat->sorting)
	{
	case XLAT_INDEXED:
		if (val >= xlat->size)
			return (NULL);
		return (xlat->data[val].name);
	case XLAT_SORTED:
		const t_xlat_data *res;
		res = bsearch(&val, xlat->data, xlat->size, sizeof(xlat->data[0].val),
					  xlat_val_comparator);
		return (res ? res->name : NULL);
	case XLAT_DEFAULT:
		for (unsigned int i = 0; i < xlat->size; i++)
			if (xlat->data[i].val == val)
				return (xlat->data[i].name);
		return (NULL);
	}
	return (NULL);
}

#define SYS_STUB(n)                                              \
	int __attribute__((weak)) SYS_FUNC_NAME(n)(struct s_td * td) \
	{                                                            \
		(void) td; /* TODO: unimplemented */                     \
		return (SF_DECODE_COMPLETE);                             \
	}

#include "xlat.stub.h"
#undef SYS_STUB