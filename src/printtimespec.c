#include "ft_print.h"
#include "ft_utils.h"
#include "sysent/xlat.h"
#include <linux/time.h>
#include <string.h>

typedef struct t_struct_timespec64
{
		long long tv_sec;
		long long tv_nsec;
} t_struct_timespec64;

typedef struct s_struct_timespec32
{
		int tv_sec;
		int tv_nsec;
} t_struct_timespec32;

#define STRUCT_TIMESPEC       t_struct_timespec64
#define PRINTTIMESPEC         printtimespec64
#define PRINTTIMESPEC_STRUCT  printtimespec64_struct
#define SPRINTTIMESPEC        sprinttimespec64
#define SPRINTTIMESPEC_STRUCT sprinttimespec64_struct
#define PRECISION_FIELD       tv_nsec
#include "printtimespec.h"
#undef STRUCT_TIMESPEC
#undef PRINTTIMESPEC
#undef PRINTTIMESPEC_STRUCT
#undef SPRINTTIMESPEC
#undef SPRINTTIMESPEC_STRUCT
#undef PRECISION_FIELD

#define STRUCT_TIMESPEC       t_struct_timespec32
#define PRINTTIMESPEC         printtimespec32
#define PRINTTIMESPEC_STRUCT  printtimespec32_struct
#define SPRINTTIMESPEC        sprinttimespec32
#define SPRINTTIMESPEC_STRUCT sprinttimespec32_struct
#define PRECISION_FIELD       tv_nsec
#include "printtimespec.h"
#undef STRUCT_TIMESPEC
#undef PRINTTIMESPEC
#undef PRINTTIMESPEC_STRUCT
#undef SPRINTTIMESPEC
#undef SPRINTTIMESPEC_STRUCT
#undef PRECISION_FIELD
