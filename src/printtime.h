#ifndef PRINTTIME_H
#define PRINTTIME_H

#include "types_time.h"
#include <inttypes.h>
#include <linux/posix_types.h>
#include <utime.h>

const char *sprinttime(unsigned long sec, unsigned long nsec);
void        printtime(unsigned long sec, unsigned long nsec);
void        print_clock_id(int clockid);

const char *sprinttimespec32(struct s_td *td, __kernel_ulong_t addr);
const char *sprinttimespec64(struct s_td *td, __kernel_ulong_t addr);
void        printtimespec32(struct s_td *td, __kernel_ulong_t addr);
void        printtimespec64(struct s_td *td, __kernel_ulong_t addr);

void        printtimeval32_struct(void *pt);
void        printtimeval64_struct(void *pt);
void        printtimeval32(struct s_td *td, __kernel_ulong_t addr);
void        printtimeval64(struct s_td *td, __kernel_ulong_t addr);

void        printitimerval32(struct s_td *td, __kernel_ulong_t addr);
void        printitimerval64(struct s_td *td, __kernel_ulong_t addr);

// #define sprinttimespec_struct (current_abi == ABI_64BIT ? sprinttimespec64_struct : sprinttimespec32_struct)
// #define printtimespec_struct  current_abi == ABI_64BIT ? printtimespec64_struct : printtimespec32_struct
// #define sprinttimespec        (current_abi == ABI_64BIT ? sprinttimespec64 : sprinttimespec32)
// #define printtimespec         current_abi == ABI_64BIT ? printtimespec64 : printtimespec32

// #define printtimeval_struct current_abi == ABI_64BIT ? printtimeval64_struct : printtimeval32_struct
// #define printtimeval        current_abi == ABI_64BIT ? printtimeval64 : printtimeval32

#define printitimerval                  printitimerval64
#define print_kernel_old_timeval_struct printtimeval64_struct

const char *sprinttimeval(struct s_td *td, __kernel_ulong_t addr);
const char *sprinttimezone(struct s_td *td, __kernel_ulong_t addr);
void        printtimezone(struct s_td *td, __kernel_ulong_t addr);
void        printutimbuf(struct s_td *td, __kernel_ulong_t addr);
void        printtimespec_utimes32(struct s_td *td, __kernel_ulong_t addr);
void        printtimespec_utimes64(struct s_td *td, __kernel_ulong_t addr);
void        printtimeval_utimes32(struct s_td *td, __kernel_ulong_t addr);
void        printtimeval_utimes64(struct s_td *td, __kernel_ulong_t addr);
// void        printutimbuf_utimes(struct s_td *td, __kernel_ulong_t addr);
void printtimex64(struct s_td *td, __kernel_ulong_t addr);
void printtimex32(struct s_td *td, __kernel_ulong_t addr);

#define printtimex current_abi == ABI_64BIT ? printtimex64 : printtimex32

#endif /* PRINTTIME_H */