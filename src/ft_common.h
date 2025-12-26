#ifndef FT_COMMON
#define FT_COMMON

#include <linux/posix_types.h>

void __attribute__((__noreturn__, __format__(__printf__, 2, 3)))
perror_and_die_(int errno_, const char *fmt, ...);

#define perror_and_die(erno_, fmt, ...) perror_and_die_(erno_, "%s: " fmt, __func__, ##__VA_ARGS__)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define zero_extend_signed_to_ull(v)                                          \
	(sizeof(v) == sizeof(char)  ? (unsigned long long) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (unsigned long long) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (unsigned long long) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (unsigned long long) (unsigned long) (v) :  \
								  (unsigned long long) (v))

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