#ifndef FT_COMMON
#define FT_COMMON

#ifdef DEBUG_ME
#  define DONT_FORGET(msg)
#else
#  define DONT_FORGET(msg) _Pragma(#msg);
#endif

#include <linux/posix_types.h>

void __attribute__((__noreturn__, __format__(__printf__, 2, 3)))
perror_and_die_(int errno_, const char *fmt, ...);

void __attribute__((__noreturn__, __format__(__printf__, 1, 2)))
die_(const char *fmt, ...);

#define perror_and_die(erno_, fmt, ...) perror_and_die_(erno_, "%s: " fmt, __func__, ##__VA_ARGS__)
#define die(fmt_, ...)                  die_("%s: " fmt_, __func__, ##__VA_ARGS__)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

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

#define zero_extend_signed_to_size_t(v)                           \
	(sizeof(v) == sizeof(char)  ? (size_t) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (size_t) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (size_t) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (size_t) (unsigned long) (v) :  \
								  (size_t) (v))

#define SYS_FUNC_NAME(syscall_name) sys_##syscall_name

// Now I see that some syscalls require more decoding after syscall return

#define SYS_FUNC(syscall_name) \
	int SYS_FUNC_NAME(syscall_name)(struct s_td * td)

union u_addrb
{
		unsigned int     ws32;
		__kernel_ulong_t ws64;
		unsigned char    raw[sizeof(__kernel_ulong_t)];
};

#endif