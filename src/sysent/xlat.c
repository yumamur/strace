#include "xlat.h"

const char *search_xlat(const t_xlat *xlat, const uint64_t val)
{
	for (unsigned int i = 0; i < xlat->size; i++)
		if (xlat->data[i].val == val)
			return xlat->data[i].name;
	return 0;
}