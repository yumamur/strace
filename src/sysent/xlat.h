#ifndef FT_XLAT
#define FT_XLAT

#include "../trace.h"
#include <stdint.h>

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

extern const t_xlat access_modes[];
extern const t_xlat faccessat2_flags[];
extern const t_xlat open_access_flags[];
extern const t_xlat open_flags[];
extern const t_xlat execveat_flags[];
extern const t_xlat mode_file_types[];
extern const t_xlat mode_protection_bits[];
extern const t_xlat fstatat_flags[];

// util functions
const char *search_xlat(const t_xlat *xlat, const uint64_t val);

#endif