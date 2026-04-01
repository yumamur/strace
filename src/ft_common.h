#ifndef FT_COMMON_H
#define FT_COMMON_H

#include "types_time.h"
#include <stddef.h>
#include <inttypes.h>
#include <linux/posix_types.h>
#include <stdbool.h>
#include <stdint.h>

void __attribute__((__format__(__printf__, 2, 3)))
perror_(int errno, const char *fmt, ...);

void __attribute__((__noreturn__, __format__(__printf__, 2, 3)))
perror_and_die_(int errno_, const char *fmt, ...);

void __attribute__((__noreturn__, __format__(__printf__, 1, 2)))
die_(const char *fmt, ...);

#define perror_and_cont(erno_, fmt_, ...) perror_(erno_, "%s: " fmt_, __func__, ##__VA_ARGS__)
#define perror_and_die(erno_, fmt, ...)   perror_and_die_(erno_, "%s: " fmt, __func__, ##__VA_ARGS__)
#define die(fmt_, ...)                    die_("%s: " fmt_, __func__, ##__VA_ARGS__)

bool is_error_erestart(unsigned int err);

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define TO_STRING(...) #__VA_ARGS__

#define zero_extend_signed_to_ull(v)                                          \
	(sizeof(v) == sizeof(char)  ? (unsigned long long) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (unsigned long long) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (unsigned long long) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (unsigned long long) (unsigned long) (v) :  \
								  (unsigned long long) (v))

#define zero_extend_signed_to_ul(v)                                      \
	(sizeof(v) == sizeof(char)  ? (unsigned long) (unsigned char) (v) :  \
	 sizeof(v) == sizeof(short) ? (unsigned long) (unsigned short) (v) : \
	 sizeof(v) == sizeof(int)   ? (unsigned long) (unsigned int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (unsigned long) (unsigned long) (v) :  \
								  (unsigned long) (v))

#define sign_extend_unsigned_to_ll(v)                       \
	(sizeof(v) == sizeof(char)  ? (long long) (char) (v) :  \
	 sizeof(v) == sizeof(short) ? (long long) (short) (v) : \
	 sizeof(v) == sizeof(int)   ? (long long) (int) (v) :   \
	 sizeof(v) == sizeof(long)  ? (long long) (long) (v) :  \
								  (long long) (v))

#define sign_extend_unsigned_to_l(v)                   \
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

#ifndef NT_PRSTATUS
#  define NT_PRSTATUS 1
#endif

// forward declare
struct s_td;
typedef int (*t_logger)(struct s_td *);

// Now I see that some syscalls require more decoding after syscall return
#define SYS_FUNC_NAME(syscall_name) sys_##syscall_name
#define SYS_FUNC(syscall_name)      int SYS_FUNC_NAME(syscall_name)(struct s_td * td)
// #define SYS_FUNC_ARGS(index_)       (&syscall_argnames[index_])

int syscall_noop(struct s_td *);

#include "decoders.h"

// deprecated
#define sys_sysfs           printargs
#define sys_olduname        printargs
#define sys_oldolduname     printargs
#define sys_oldfstat        printargs
#define sys_oldstat         printargs
#define sys_vm86            printargs
#define sys_vm86old         printargs
#define sys_old_mmap        printargs
#define sys_create_module   printargs
#define sys_get_kernel_syms printargs
#define sys_query_module    printargs
#define sys_bdflush         printargs
#define sys_oldselect       printargs

// not implemented
#define sys_afs_syscall printargs
#define sys_break       printargs
#define sys_fattach     printargs
#define sys_fdetach     printargs
#define sys_ftime       printargs
#define sys_getmsg      printargs
#define sys_getpmsg     printargs
#define sys_gtty        printargs
#define sys_isastream   printargs
#define sys_lock        printargs
#define sys_madvise1    printargs
#define sys_mpx         printargs
#define sys_prof        printargs
#define sys_profil      printargs
#define sys_nfsservctl  printargs
#define sys_putmsg      printargs
#define sys_putpmsg     printargs
#define sys_security    printargs
#define sys_stty        printargs
#define sys_tuxcall     printargs
#define sys_ulimit      printargs
#define sys_vserver     printargs

// too tired to do
#define sys_signal      printargs
#define sys_sigaction   printargs
#define sys_ssetmask    printargs
#define sys_sigsuspend  printargs
#define sys_sigpending  printargs
#define sys_sigprocmask printargs

// same thing?
#define sys_getsockname            sys_accept
#define sys_getpeername            sys_accept
#define sys_bind                   sys_connect
#define sys_rmdir                  sys_chdir
#define sys_uselib                 sys_chdir
#define sys_unlink                 sys_chdir
#define sys_umount                 sys_chdir
#define sys_chroot                 sys_chdir
#define sys_acct                   sys_chdir
#define sys_swapoff                sys_chdir
#define sys_mq_unlink              sys_chdir
#define sys_lchown                 sys_chown
#define sys_lchown16               sys_chown16
#define sys_clock_getres_time32    sys_clock_gettime32
#define sys_clock_getres_time64    sys_clock_gettime64
#define sys_fsync                  sys_close
#define sys_fdatasync              sys_close
#define sys_fchdir                 sys_close
#define sys_syncfs                 sys_close
#define sys_exit_group             sys_exit
#define sys_getdents64             sys_getdents
#define sys_readdir                sys_getdents
#define sys_getsid                 sys_getpgid
#define sys_lgetxattr              sys_getxattr
#define sys_tkill                  sys_kill
#define sys_llistxattr             sys_listxattr
#define sys_creat                  sys_mkdir
#define sys_chmod                  sys_mkdir
#define sys_mlock                  sys_munmap
#define sys_munlock                sys_munmap
#define sys_oldlstat               sys_oldstat
#define sys_lremovexattr           sys_removexattr
#define sys_link                   sys_rename
#define sys_symlink                sys_rename
#define sys_pivotroot              sys_rename
#define sys_sched_get_priority_min sys_sched_get_priority_max
#define sys_setdomainname          sys_sethostname
#define sys_setregid               sys_setreuid
#define sys_setregid16             sys_setreuid16
#define sys_setgid                 sys_setuid
#define sys_setgid16               sys_setuid16
#define sys_lsetxattr              sys_setxattr
#define sys_lstat                  sys_stat
#define sys_lstat64                sys_stat64
#define sys_stime                  sys_time

// no-op
#define sys_fork            syscall_noop
#define sys_getegid         syscall_noop
#define sys_getegid16       syscall_noop
#define sys_geteuid         syscall_noop
#define sys_geteuid16       syscall_noop
#define sys_getgid          syscall_noop
#define sys_getgid16        syscall_noop
#define sys_getpgrp         syscall_noop
#define sys_getpid          syscall_noop
#define sys_getppid         syscall_noop
#define sys_gettid          syscall_noop
#define sys_getuid          syscall_noop
#define sys_getuid16        syscall_noop
#define sys_idle            syscall_noop
#define sys_inotify_init    syscall_noop
#define sys_munlockall      syscall_noop
#define sys_pause           syscall_noop
#define sys_restart_syscall syscall_noop
#define sys_sched_yield     syscall_noop
#define sys_setsid          syscall_noop
#define sys_sgetmask        syscall_noop
#define sys_sigreturn       syscall_noop
#define sys_sync            syscall_noop
#define sys_uretprobe       syscall_noop
#define sys_vfork           syscall_noop
#define sys_vhangup         syscall_noop

typedef struct s_entry
{
		unsigned int nargs;
		t_logger     logger;
		const char  *call_name;
		unsigned int flags : 2;
} t_entry;

#define SENFL_TRACE   0x1
#define SENFL_VERBOSE 0x2

#define TOTAL_ABI 2

enum e_abi
{
	ABI_64BIT = 0,
	ABI_32BIT = 1,
	// ABI_X32 = 2  I can't even compile this, most distros does not support it
};

#define MAX_ARGS 6

typedef void (*free_carry_func)(void *);

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
		t_struct_timespec64  ptime;
		t_struct_timespec64  stime;
		void            *carry;
		free_carry_func  free_carry;

} t_td;

#define entering(td_)   (!((td_).flags & TD_INSYSCALL))
#define exiting(td_)    ((td_).flags & TD_INSYSCALL)
#define is_traced(td_)  ((td_).entry && (td_).entry->flags & SENFL_TRACE)
#define is_verbose(td_) ((td_).entry && (td_).entry->flags & SENFL_VERBOSE)

static inline void td_free_carry(struct s_td *td)
{
	if (td->carry && td->free_carry)
		td->free_carry(td->carry);
	td->carry = NULL;
	td->free_carry = NULL;
}

static inline void td_carry(struct s_td *td, void *carry, free_carry_func free_func)
{
	td_free_carry(td);
	td->carry = carry;
	td->free_carry = free_func;
}

static inline void td_carry_ulong(struct s_td *td, unsigned long val)
{
	td_carry(td, (void *) (uintptr_t) val, NULL);
}

#define TD_CARRY_GET_FUNC(name_, type_)                       \
	static inline type_ td_carry_get_##name_(struct s_td *td) \
	{                                                         \
		return (type_) td->carry;                             \
	}

TD_CARRY_GET_FUNC(ulong, unsigned long)

TD_CARRY_GET_FUNC(voidptr, void *)

#undef TD_CARRY_GET_FUNC

typedef union u_addr
{
		unsigned int     ws32;
		__kernel_ulong_t ws64;
		unsigned char    raw[sizeof(__kernel_ulong_t)];
} t_addr;

#ifdef __x86_64__
#  define KLONG_SIZE 8
#else
#  define KLONG_SIZE 4
#endif

extern unsigned int sysent_size;
extern enum e_abi   current_abi;
extern unsigned int current_wordsize;
extern unsigned int current_klongsize;

const char         *get_errmsg(unsigned int num);
const char         *get_errname(unsigned int num);

// __kernel_ulong_t    get_sp_reg(void);

#endif
