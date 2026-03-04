// #include "../ft_print.h"
// #include "../ft_utils.h"

// #undef dev_t
// #undef gid_t
// #undef ino_t
// #undef loff_t
// #undef mode_t
// #undef nlink_t
// #undef off64_t
// #undef off_t
// #undef time_t
// #undef uid_t

// #define dev_t   __kernel_dev_t
// #define gid_t   __kernel_gid_t
// #define ino_t   __kernel_ino_t
// #define loff_t  __kernel_loff_t
// #define mode_t  __kernel_mode_t
// #define nlink_t __kernel_nlink_t
// #define off64_t __kernel_off64_t
// #define off_t   __kernel_off_t
// #define time_t  __kernel_time_t
// #define uid_t   __kernel_uid_t

// #include <asm/stat.h>

// #undef dev_t
// #undef gid_t
// #undef ino_t
// #undef loff_t
// #undef mode_t
// #undef nlink_t
// #undef off64_t
// #undef off_t
// #undef time_t
// #undef uid_t

// #define dev_t   dev_t
// #define gid_t   gid_t
// #define ino_t   ino_t
// #define loff_t  loff_t
// #define mode_t  mode_t
// #define nlink_t nlink_t
// #define off64_t off64_t
// #define off_t   off_t
// #define time_t  time_t
// #define uid_t   uid_t

// struct strace_stat
// {
// 		unsigned long long dev;
// 		unsigned long long ino;
// 		unsigned long long rdev;
// 		unsigned long long size;
// 		unsigned long long blocks;
// 		unsigned long long blksize;
// 		unsigned long long mode;
// 		unsigned long long nlink;
// 		unsigned long long uid;
// 		unsigned long long gid;
// 		long long          atime;
// 		long long          ctime;
// 		long long          mtime;
// 		unsigned long long atime_nsec;
// 		unsigned long long ctime_nsec;
// 		unsigned long long mtime_nsec;
// };

// int fetchstat(struct s_td *td, __kernel_ulong_t addr, struct stat *const dest)
// {
// 	struct stat buf;
// 	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
// 	{
// 		printaddr(addr);
// 		return -1;
// 	}
// 	dest->st_dev = zero_extend_signed_to_ull(buf.st_dev);
// 	dest->st_ino = zero_extend_signed_to_ull(buf.st_ino);
// 	dest->st_rdev = zero_extend_signed_to_ull(buf.st_rdev);
// 	dest->st_size = zero_extend_signed_to_ull(buf.st_size);
// 	dest->st_blocks = zero_extend_signed_to_ull(buf.st_blocks);
// 	dest->st_blksize = zero_extend_signed_to_ull(buf.st_blksize);
// 	dest->st_mode = zero_extend_signed_to_ull(buf.st_mode);
// 	dest->st_nlink = zero_extend_signed_to_ull(buf.st_nlink);
// 	dest->st_uid = zero_extend_signed_to_ull(buf.st_uid);
// 	dest->st_gid = zero_extend_signed_to_ull(buf.st_gid);
// 	dest->st_atime = zero_extend_signed_to_ll(buf.st_atime);
// 	dest->st_ctime = zero_extend_signed_to_ll(buf.st_ctime);
// 	dest->st_mtime = zero_extend_signed_to_ll(buf.st_mtime);
// 	dest->st_atime_nsec = zero_extend_signed_to_ull(buf.st_atime_nsec);
// 	dest->st_ctime_nsec = zero_extend_signed_to_ull(buf.st_ctime_nsec);
// 	dest->st_mtime_nsec = zero_extend_signed_to_ull(buf.st_mtime_nsec);
// 	return true;
// }

// // struct stat_32
// // {
// // 		__dev_t            st_dev;
// // 		unsigned short int __pad1;
// // 		__ino_t            __st_ino;
// // 		__mode_t           st_mode;  /* File mode.  */
// // 		__nlink_t          st_nlink; /* Link count.  */
// // 		__uid_t            st_uid;   /* User ID of the file's owner.	*/
// // 		__gid_t            st_gid;   /* Group ID of the file's group.*/
// // 		__dev_t            st_rdev;  /* Device number, if device.  */
// // 		unsigned short int __pad2;
// // 		__off64_t          st_size;    /* Size of file, in bytes.  */
// // 		__blksize_t        st_blksize; /* Optimal block size for I/O.  */
// // 		__blkcnt64_t       st_blocks;  /* Number 512-byte blocks allocated. */
// // 		struct timespec    st_atim;    /* Time of last access.  */
// // 		struct timespec    st_mtim;    /* Time of last modification.  */
// // 		struct timespec    st_ctim;    /* Time of last status change.  */
// // 		unsigned long int __glibc_reserved4;
// // 		unsigned long int __glibc_reserved5;
// // };