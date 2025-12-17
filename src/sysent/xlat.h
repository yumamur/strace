#ifndef FT_XLAT
#define FT_XLAT

#include <stdint.h>

typedef struct s_xlat_data
{
		uint64_t val;
		char    *name;
} t_xlat_data;

typedef struct s_xlat
{
		const t_xlat_data *data;
		unsigned int       size;
} t_xlat;

#define XLAT(flag) {.val = flag, .name = #flag}

#endif