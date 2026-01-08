#ifndef FT_COMMON_H
#define FT_COMMON_H

#include <linux/posix_types.h>
#include <stdint.h>
#include <time.h>

void __attribute__((__noreturn__, __format__(__printf__, 2, 3)))
perror_and_die_(int errno_, const char *fmt, ...);

void __attribute__((__noreturn__, __format__(__printf__, 1, 2)))
die_(const char *fmt, ...);

#define perror_and_die(erno_, fmt, ...) perror_and_die_(erno_, "%s: " fmt, __func__, ##__VA_ARGS__)
#define die(fmt_, ...)                  die_("%s: " fmt_, __func__, ##__VA_ARGS__)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define zero_extend_signed_to_ull(v)                                          \
	(sizeof(v) == sizeof(char)  ? (unsigned long long) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (unsigned long long) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (unsigned long long) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (unsigned long long) (unsigned long) (v) :  \
								  (unsigned long long) (v))

#define zero_extend_signed_to_ll(v)                         \
	(sizeof(v) == sizeof(char)  ? (long long) (char) (v) :  \
	 sizeof(v) == sizeof(short) ? (long long) (short) (v) : \
	 sizeof(v) == sizeof(int)   ? (long long) (int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (long long) (long) (v) :  \
								  (long long) (v))

#define zero_extend_signed_to_l(v)                     \
	(sizeof(v) == sizeof(char)  ? (long) (char) (v) :  \
	 sizeof(v) == sizeof(short) ? (long) (short) (v) : \
	 sizeof(v) == sizeof(int)   ? (long) (int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (long) (long) (v) :  \
								  (long) (v))

#define zero_extend_signed_to_size_t(v)                           \
	(sizeof(v) == sizeof(char)  ? (size_t) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (size_t) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (size_t) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (size_t) (unsigned long) (v) :  \
								  (size_t) (v))

#define MAX_ERRNO_VAL 600

#define TD_INSYSCALL   0x01
#define TD_OUT_SUMMARY 0x02

#define SF_DECODE_COMPLETE 0x10
#define SF_PRINT_HEX       0x20
#define SF_AFTER_RETURN    0x40
#define SF_MASK            (SF_DECODE_COMPLETE | SF_PRINT_HEX | SF_AFTER_RETURN)

// forward declare
struct s_td;

// Now I see that some syscalls require more decoding after syscall return
#define SYS_FUNC_NAME(syscall_name) sys_##syscall_name
#define SYS_FUNC(syscall_name)      int SYS_FUNC_NAME(syscall_name)(struct s_td * td)

#include "decoders.h"

typedef struct s_entry
{
		unsigned int nargs;
		int          (*logger)(struct s_td *);
		const char  *call_name;
		unsigned int traced : 1;
} t_entry;

#define TOTAL_ABI 2

enum e_abi
{
	ABI_64BIT = 0,
	ABI_32BIT = 1,
	// ABI_X32 = 2  I can't even compile this, most distros does not support it
};

#define MAX_ARGS 6

typedef struct s_td
{
		enum e_abi       abi;
		unsigned int     flags;
		int              pid;
		struct s_entry  *entry;
		__kernel_ulong_t sc_no;
		__kernel_ulong_t sc_args[MAX_ARGS];
		__kernel_ulong_t sc_ret;
		uint32_t         sc_err;
		struct timespec  ptime;
		struct timespec  stime;
} t_td;

#define entering(td_) (!((td_).flags & TD_INSYSCALL))
#define exiting(td_)  ((td_).flags & TD_INSYSCALL)

typedef union u_addr
{
		unsigned int     ws32;
		__kernel_ulong_t ws64;
		unsigned char    raw[sizeof(__kernel_ulong_t)];
} t_addr;

extern unsigned int sysent_size;
extern unsigned int current_wordsize;
extern unsigned int current_klongsize;

const char         *get_errmsg(unsigned int num);

#endif