#ifndef TYPES_TIME_H
#define TYPES_TIME_H

#include <linux/posix_types.h>
#include <stdint.h>

typedef struct t_struct_timespec64
{
		long long tv_sec;
		long long tv_nsec;
} t_struct_timespec64;

typedef struct s_struct_itimerspec64
{
		t_struct_timespec64 it_interval;
		t_struct_timespec64 it_value;
} t_struct_itimerspec64;

typedef struct s_struct_timespec32
{
		int tv_sec;
		int tv_nsec;
} t_struct_timespec32;

typedef struct s_struct_itimerspec32
{
		t_struct_timespec32 it_interval;
		t_struct_timespec32 it_value;
} t_struct_itimerspec32;

typedef struct s_struct_timeval64
{
		int64_t tv_sec;
		int64_t tv_usec;
} t_struct_timeval64;

typedef struct s_struct_itimerval64
{
		t_struct_timeval64 it_interval;
		t_struct_timeval64 it_value;
} t_struct_itimerval64;

typedef struct s_struct_timeval32
{
		int32_t tv_sec;
		int32_t tv_usec;
} t_struct_timeval32;

typedef struct s_struct_itimerval32
{
		t_struct_timeval32 it_interval;
		t_struct_timeval32 it_value;
} t_struct_itimerval32;

#endif /* TYPES_TIME_H */