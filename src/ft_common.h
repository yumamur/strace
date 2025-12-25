#ifndef FT_COMMON
#define FT_COMMON

void __attribute__((__noreturn__, __format__(__printf__, 1, 0)))
perror_and_die(const char *fmt, ...);

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define zero_extend_signed_to_ull(v)                                          \
	(sizeof(v) == sizeof(char)  ? (unsigned long long) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (unsigned long long) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (unsigned long long) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (unsigned long long) (unsigned long) (v) :  \
								  (unsigned long long) (v))

#define SYS_FUNC_NAME(syscall_name) sys_##syscall_name
#define SYS_FUNC(syscall_name) \
	void SYS_FUNC_NAME(syscall_name)(struct s_td * td)

#endif