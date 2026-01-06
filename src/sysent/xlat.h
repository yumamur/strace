#ifndef FT_XLAT_H
#define FT_XLAT_H

#include "../trace.h"
#include <stdint.h>

#define XLAT_INDEXED(val_)      [val_] = #val_
#define XLAT(val_)              {.val = (uint64_t) (val_), .name = #val_}
#define XLAT_NAMED(val_, name_) {.val = (uint64_t) (val_), .name = name_}
#define XLAT_TYPED(type, val_)  {.val = (type) (val_), .name = #val_}

#define WXLAT(name_)         \
	const t_xlat name_[] = { \
		{.data = name_##_data, .size = ARRAY_SIZE(name_##_data)} \
    }
typedef struct s_xlat_data
{
		uint64_t    val;
		const char *name;
} t_xlat_data;

typedef struct s_xlat
{
		unsigned int       size;
		const t_xlat_data *data;
} t_xlat;

// util functions
const char *search_xlat(const t_xlat *xlat, const uint64_t val);

#endif