#include "ft_common.h"
#include "sysent/xlat.h"
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ERESTARTSYS
#  define ERESTARTSYS 512
#endif
#ifndef ERESTARTNOINTR
#  define ERESTARTNOINTR 513
#endif
#ifndef ERESTARTNOHAND
#  define ERESTARTNOHAND 514
#endif
#ifndef ENOIOCTLCMD
#  define ENOIOCTLCMD 515
#endif
#ifndef ERESTART_RESTARTBLOCK
#  define ERESTART_RESTARTBLOCK 516
#endif
#ifndef EPROBE_DEFER
#  define EPROBE_DEFER 517
#endif
#ifndef EOPENSTALE
#  define EOPENSTALE 518
#endif
#ifndef EBADHANDLE
#  define EBADHANDLE 521
#endif
#ifndef ENOTSYNC
#  define ENOTSYNC 522
#endif
#ifndef EBADCOOKIE
#  define EBADCOOKIE 523
#endif
#ifndef ENOTSUPP
#  define ENOTSUPP 524
#endif
#ifndef ETOOSMALL
#  define ETOOSMALL 525
#endif
#ifndef ESERVERFAULT
#  define ESERVERFAULT 526
#endif
#ifndef EBADTYPE
#  define EBADTYPE 527
#endif
#ifndef EJUKEBOX
#  define EJUKEBOX 528
#endif
#ifndef EIOCBQUEUED
#  define EIOCBQUEUED 529
#endif
#ifndef ERECALLCONFLICT
#  define ERECALLCONFLICT 530
#endif

void __attribute__((__noreturn__, __format__(__printf__, 2, 3)))
perror_and_die_(int errno_, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fflush(NULL);
	vfprintf(stderr, fmt, va);
	fprintf(stderr, ": %s\n", strerror(errno_));
	fflush(stderr);
	va_end(va);
	exit(1);
}

void __attribute__((__noreturn__, __format__(__printf__, 1, 2)))
die_(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fflush(NULL);
	vfprintf(stderr, fmt, va);
	fflush(stderr);
	va_end(va);
	exit(1);
}

void __attribute__((__format__(__printf__, 2, 3)))
perror_(int errno_, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fflush(NULL);
	vfprintf(stderr, fmt, va);
	fprintf(stderr, ": %s\n", strerror(errno_));
	fflush(stderr);
	va_end(va);
}

const char *err_msgs[] = {
	[EPERM] = "EPERM (Operation not permitted)",
	[ENOENT] = "ENOENT (No such file or directory)",
	[ESRCH] = "ESRCH (No such process)",
	[EINTR] = "EINTR (Interrupted system call)",
	[EIO] = "EIO (IO error)",
	[ENXIO] = "ENXIO (No such device or address)",
	[E2BIG] = "E2BIG (Argument list too long)",
	[ENOEXEC] = "ENOEXEC (Exec format error)",
	[EBADF] = "EBADF (Bad file number)",
	[ECHILD] = "ECHILD (No child processes)",
	[EAGAIN] = "EAGAIN (Try again)",
	[ENOMEM] = "ENOMEM (Out of memory)",
	[EACCES] = "EACCES (Permission denied)",
	[EFAULT] = "EFAULT (Bad address)",
	[ENOTBLK] = "ENOTBLK (Block device required)",
	[EBUSY] = "EBUSY (Device or resource busy)",
	[EEXIST] = "EEXIST (File exists)",
	[EXDEV] = "EXDEV (Cross-device link)",
	[ENODEV] = "ENODEV (No such device)",
	[ENOTDIR] = "ENOTDIR (Not a directory)",
	[EISDIR] = "EISDIR (Is a directory)",
	[EINVAL] = "EINVAL (Invalid argument)",
	[ENFILE] = "ENFILE (File table overflow)",
	[EMFILE] = "EMFILE (Too many open files)",
	[ENOTTY] = "ENOTTY (Not a typewriter)",
	[ETXTBSY] = "ETXTBSY (Text file busy)",
	[EFBIG] = "EFBIG (File too large)",
	[ENOSPC] = "ENOSPC (No space left on device)",
	[ESPIPE] = "ESPIPE (Illegal seek)",
	[EROFS] = "EROFS (Read-only file system)",
	[EMLINK] = "EMLINK (Too many links)",
	[EPIPE] = "EPIPE (Broken pipe)",
	[EDOM] = "EDOM (Math argument out of domain of func)",
	[ERANGE] = "ERANGE (Math result not representable)",
	[EDEADLK] = "EDEADLK (Resource deadlock would occur)",
	[ENAMETOOLONG] = "ENAMETOOLONG (File name too long)",
	[ENOLCK] = "ENOLCK (No record locks available)",
	[ENOSYS] = "ENOSYS (Invalid system call number)",
	[ENOTEMPTY] = "ENOTEMPTY (Directory not empty)",
	[ELOOP] = "ELOOP (Too many symbolic links encountered)",
	[ENOMSG] = "ENOMSG (No message of desired type)",
	[EIDRM] = "EIDRM (Identifier removed)",
	[ECHRNG] = "ECHRNG (Channel number out of range)",
	[EL2NSYNC] = "EL2NSYNC (Level 2 not synchronized)",
	[EL3HLT] = "EL3HLT (Level 3 halted)",
	[EL3RST] = "EL3RST (Level 3 reset)",
	[ELNRNG] = "ELNRNG (Link number out of range)",
	[EUNATCH] = "EUNATCH (Protocol driver not attached)",
	[ENOCSI] = "ENOCSI (No CSI structure available)",
	[EL2HLT] = "EL2HLT (Level 2 halted)",
	[EBADE] = "EBADE (Invalid exchange)",
	[EBADR] = "EBADR (Invalid request descriptor)",
	[EXFULL] = "EXFULL (Exchange full)",
	[ENOANO] = "ENOANO (No anode)",
	[EBADRQC] = "EBADRQC (Invalid request code)",
	[EBADSLT] = "EBADSLT (Invalid slot)",
	[EBFONT] = "EBFONT (Bad font file format)",
	[ENOSTR] = "ENOSTR (Device not a stream)",
	[ENODATA] = "ENODATA (No data available)",
	[ETIME] = "ETIME (Timer expired)",
	[ENOSR] = "ENOSR (Out of streams resources)",
	[ENONET] = "ENONET (Machine is not on the network)",
	[ENOPKG] = "ENOPKG (Package not installed)",
	[EREMOTE] = "EREMOTE (Object is remote)",
	[ENOLINK] = "ENOLINK (Link has been severed)",
	[EADV] = "EADV (Advertise error)",
	[ESRMNT] = "ESRMNT (Srmount error)",
	[ECOMM] = "ECOMM (Communication error on send)",
	[EPROTO] = "EPROTO (Protocol error)",
	[EMULTIHOP] = "EMULTIHOP (Multihop attempted)",
	[EDOTDOT] = "EDOTDOT (RFS specific error)",
	[EBADMSG] = "EBADMSG (Not a data message)",
	[EOVERFLOW] = "EOVERFLOW (Value too large for defined data type)",
	[ENOTUNIQ] = "ENOTUNIQ (Name not unique on network)",
	[EBADFD] = "EBADFD (File descriptor in bad state)",
	[EREMCHG] = "EREMCHG (Remote address changed)",
	[ELIBACC] = "ELIBACC (Can not access a needed shared library)",
	[ELIBBAD] = "ELIBBAD (Accessing a corrupted shared library)",
	[ELIBSCN] = "ELIBSCN (.lib section in a.out corrupted)",
	[ELIBMAX] = "ELIBMAX (Attempting to link in too many shared libraries)",
	[ELIBEXEC] = "ELIBEXEC (Cannot exec a shared library directly)",
	[EILSEQ] = "EILSEQ (Illegal byte sequence)",
	[ERESTART] = "ERESTART (Interrupted system call should be restarted)",
	[ESTRPIPE] = "ESTRPIPE (Streams pipe error)",
	[EUSERS] = "EUSERS (Too many users)",
	[ENOTSOCK] = "ENOTSOCK (Socket operation on non-socket)",
	[EDESTADDRREQ] = "EDESTADDRREQ (Destination address required)",
	[EMSGSIZE] = "EMSGSIZE (Message too long)",
	[EPROTOTYPE] = "EPROTOTYPE (Protocol wrong type for socket)",
	[ENOPROTOOPT] = "ENOPROTOOPT (Protocol not available)",
	[EPROTONOSUPPORT] = "EPROTONOSUPPORT (Protocol not supported)",
	[ESOCKTNOSUPPORT] = "ESOCKTNOSUPPORT (Socket type not supported)",
	[EOPNOTSUPP] = "EOPNOTSUPP (Operation not supported on transport endpoint)",
	[EPFNOSUPPORT] = "EPFNOSUPPORT (Protocol family not supported)",
	[EAFNOSUPPORT] = "EAFNOSUPPORT (Address family not supported by protocol)",
	[EADDRINUSE] = "EADDRINUSE (Address already in use)",
	[EADDRNOTAVAIL] = "EADDRNOTAVAIL (Cannot assign requested address)",
	[ENETDOWN] = "ENETDOWN (Network is down)",
	[ENETUNREACH] = "ENETUNREACH (Network is unreachable)",
	[ENETRESET] = "ENETRESET (Network dropped connection because of reset)",
	[ECONNABORTED] = "ECONNABORTED (Software caused connection abort)",
	[ECONNRESET] = "ECONNRESET (Connection reset by peer)",
	[ENOBUFS] = "ENOBUFS (No buffer space available)",
	[EISCONN] = "EISCONN (Transport endpoint is already connected)",
	[ENOTCONN] = "ENOTCONN (Transport endpoint is not connected)",
	[ESHUTDOWN] = "ESHUTDOWN (Cannot send after transport endpoint shutdown)",
	[ETOOMANYREFS] = "ETOOMANYREFS (Too many references: cannot splice)",
	[ETIMEDOUT] = "ETIMEDOUT (Connection timed out)",
	[ECONNREFUSED] = "ECONNREFUSED (Connection refused)",
	[EHOSTDOWN] = "EHOSTDOWN (Host is down)",
	[EHOSTUNREACH] = "EHOSTUNREACH (No route to host)",
	[EALREADY] = "EALREADY (Operation already in progress)",
	[EINPROGRESS] = "EINPROGRESS (Operation now in progress)",
	[ESTALE] = "ESTALE (Stale file handle)",
	[EUCLEAN] = "EUCLEAN (Structure needs cleaning)",
	[ENOTNAM] = "ENOTNAM (Not a XENIX named type file)",
	[ENAVAIL] = "ENAVAIL (No XENIX semaphores available)",
	[EISNAM] = "EISNAM (Is a named type file)",
	[EREMOTEIO] = "EREMOTEIO (Remote IO error)",
	[EDQUOT] = "EDQUOT (Quota exceeded)",
	[ENOMEDIUM] = "ENOMEDIUM (No medium found)",
	[EMEDIUMTYPE] = "EMEDIUMTYPE (Wrong medium type)",
	[ECANCELED] = "ECANCELED (Operation Canceled)",
	[ENOKEY] = "ENOKEY (Required key not available)",
	[EKEYEXPIRED] = "EKEYEXPIRED (Key has expired)",
	[EKEYREVOKED] = "EKEYREVOKED (Key has been revoked)",
	[EKEYREJECTED] = "EKEYREJECTED (Key was rejected by service)",
	[EOWNERDEAD] = "EOWNERDEAD (Owner died)",
	[ENOTRECOVERABLE] = "ENOTRECOVERABLE (State not recoverable)",
	[ERFKILL] = "ERFKILL (Operation not possible due to RF-kill)",
	[EHWPOISON] = "EHWPOISON (Memory page has hardware error)",
	[ERESTARTSYS] = "ERESTARTSYS (To be restarted if SA_RESTART is set)",
	[ERESTARTNOINTR] = "ERESTARTNOINTR (To be restarted)",
	[ERESTARTNOHAND] = "ERESTARTNOHAND (To be restarted if no handler)",
	[ENOIOCTLCMD] = "ENOIOCTLCMD (No ioctl command)",
	[ERESTART_RESTARTBLOCK] = "ERESTART_RESTARTBLOCK (Restart if block)",
	[EPROBE_DEFER] = "EPROBE_DEFER (Probe deferred)",
	[EOPENSTALE] = "EOPENSTALE (Stale NFS file handle)",
	[EBADHANDLE] = "EBADHANDLE (Bad handle)",
	[ENOTSYNC] = "ENOTSYNC (State not synchronized)",
	[EBADCOOKIE] = "EBADCOOKIE (Cookie not valid)",
	[ENOTSUPP] = "ENOTSUPP (Operation not supported)",
	[ETOOSMALL] = "ETOOSMALL (Argument list too long)",
	[ESERVERFAULT] = "ESERVERFAULT (Server fault)",
	[EBADTYPE] = "EBADTYPE",
	[EJUKEBOX] = "EJUKEBOX",
	[EIOCBQUEUED] = "EIOCBQUEUED",
	[ERECALLCONFLICT] = "ERECALLCONFLICT",
};

const char *err_names[] = {
	XLAT_INDEXED(EPERM),
	XLAT_INDEXED(ENOENT),
	XLAT_INDEXED(ESRCH),
	XLAT_INDEXED(EINTR),
	XLAT_INDEXED(EIO),
	XLAT_INDEXED(ENXIO),
	XLAT_INDEXED(E2BIG),
	XLAT_INDEXED(ENOEXEC),
	XLAT_INDEXED(EBADF),
	XLAT_INDEXED(ECHILD),
	XLAT_INDEXED(EAGAIN),
	XLAT_INDEXED(ENOMEM),
	XLAT_INDEXED(EACCES),
	XLAT_INDEXED(EFAULT),
	XLAT_INDEXED(ENOTBLK),
	XLAT_INDEXED(EBUSY),
	XLAT_INDEXED(EEXIST),
	XLAT_INDEXED(EXDEV),
	XLAT_INDEXED(ENODEV),
	XLAT_INDEXED(ENOTDIR),
	XLAT_INDEXED(EISDIR),
	XLAT_INDEXED(EINVAL),
	XLAT_INDEXED(ENFILE),
	XLAT_INDEXED(EMFILE),
	XLAT_INDEXED(ENOTTY),
	XLAT_INDEXED(ETXTBSY),
	XLAT_INDEXED(EFBIG),
	XLAT_INDEXED(ENOSPC),
	XLAT_INDEXED(ESPIPE),
	XLAT_INDEXED(EROFS),
	XLAT_INDEXED(EMLINK),
	XLAT_INDEXED(EPIPE),
	XLAT_INDEXED(EDOM),
	XLAT_INDEXED(ERANGE),
	XLAT_INDEXED(EDEADLK),
	XLAT_INDEXED(ENAMETOOLONG),
	XLAT_INDEXED(ENOLCK),
	XLAT_INDEXED(ENOSYS),
	XLAT_INDEXED(ENOTEMPTY),
	XLAT_INDEXED(ELOOP),
	XLAT_INDEXED(ENOMSG),
	XLAT_INDEXED(EIDRM),
	XLAT_INDEXED(ECHRNG),
	XLAT_INDEXED(EL2NSYNC),
	XLAT_INDEXED(EL3HLT),
	XLAT_INDEXED(EL3RST),
	XLAT_INDEXED(ELNRNG),
	XLAT_INDEXED(EUNATCH),
	XLAT_INDEXED(ENOCSI),
	XLAT_INDEXED(EL2HLT),
	XLAT_INDEXED(EBADE),
	XLAT_INDEXED(EBADR),
	XLAT_INDEXED(EXFULL),
	XLAT_INDEXED(ENOANO),
	XLAT_INDEXED(EBADRQC),
	XLAT_INDEXED(EBADSLT),
	XLAT_INDEXED(EBFONT),
	XLAT_INDEXED(ENOSTR),
	XLAT_INDEXED(ENODATA),
	XLAT_INDEXED(ETIME),
	XLAT_INDEXED(ENOSR),
	XLAT_INDEXED(ENONET),
	XLAT_INDEXED(ENOPKG),
	XLAT_INDEXED(EREMOTE),
	XLAT_INDEXED(ENOLINK),
	XLAT_INDEXED(EADV),
	XLAT_INDEXED(ESRMNT),
	XLAT_INDEXED(ECOMM),
	XLAT_INDEXED(EPROTO),
	XLAT_INDEXED(EMULTIHOP),
	XLAT_INDEXED(EDOTDOT),
	XLAT_INDEXED(EBADMSG),
	XLAT_INDEXED(EOVERFLOW),
	XLAT_INDEXED(ENOTUNIQ),
	XLAT_INDEXED(EBADFD),
	XLAT_INDEXED(EREMCHG),
	XLAT_INDEXED(ELIBACC),
	XLAT_INDEXED(ELIBBAD),
	XLAT_INDEXED(ELIBSCN),
	XLAT_INDEXED(ELIBMAX),
	XLAT_INDEXED(ELIBEXEC),
	XLAT_INDEXED(EILSEQ),
	XLAT_INDEXED(ERESTART),
	XLAT_INDEXED(ESTRPIPE),
	XLAT_INDEXED(EUSERS),
	XLAT_INDEXED(ENOTSOCK),
	XLAT_INDEXED(EDESTADDRREQ),
	XLAT_INDEXED(EMSGSIZE),
	XLAT_INDEXED(EPROTOTYPE),
	XLAT_INDEXED(ENOPROTOOPT),
	XLAT_INDEXED(EPROTONOSUPPORT),
	XLAT_INDEXED(ESOCKTNOSUPPORT),
	XLAT_INDEXED(EOPNOTSUPP),
	XLAT_INDEXED(EPFNOSUPPORT),
	XLAT_INDEXED(EAFNOSUPPORT),
	XLAT_INDEXED(EADDRINUSE),
	XLAT_INDEXED(EADDRNOTAVAIL),
	XLAT_INDEXED(ENETDOWN),
	XLAT_INDEXED(ENETUNREACH),
	XLAT_INDEXED(ENETRESET),
	XLAT_INDEXED(ECONNABORTED),
	XLAT_INDEXED(ECONNRESET),
	XLAT_INDEXED(ENOBUFS),
	XLAT_INDEXED(EISCONN),
	XLAT_INDEXED(ENOTCONN),
	XLAT_INDEXED(ESHUTDOWN),
	XLAT_INDEXED(ETOOMANYREFS),
	XLAT_INDEXED(ETIMEDOUT),
	XLAT_INDEXED(ECONNREFUSED),
	XLAT_INDEXED(EHOSTDOWN),
	XLAT_INDEXED(EHOSTUNREACH),
	XLAT_INDEXED(EALREADY),
	XLAT_INDEXED(EINPROGRESS),
	XLAT_INDEXED(ESTALE),
	XLAT_INDEXED(EUCLEAN),
	XLAT_INDEXED(ENOTNAM),
	XLAT_INDEXED(ENAVAIL),
	XLAT_INDEXED(EISNAM),
	XLAT_INDEXED(EREMOTEIO),
	XLAT_INDEXED(EDQUOT),
	XLAT_INDEXED(ENOMEDIUM),
	XLAT_INDEXED(EMEDIUMTYPE),
	XLAT_INDEXED(ECANCELED),
	XLAT_INDEXED(ENOKEY),
	XLAT_INDEXED(EKEYEXPIRED),
	XLAT_INDEXED(EKEYREVOKED),
	XLAT_INDEXED(EKEYREJECTED),
	XLAT_INDEXED(EOWNERDEAD),
	XLAT_INDEXED(ENOTRECOVERABLE),
	XLAT_INDEXED(ERFKILL),
	XLAT_INDEXED(EHWPOISON),
	XLAT_INDEXED(ERESTARTSYS),
	XLAT_INDEXED(ERESTARTNOINTR),
	XLAT_INDEXED(ERESTARTNOHAND),
	XLAT_INDEXED(ENOIOCTLCMD),
	XLAT_INDEXED(ERESTART_RESTARTBLOCK),
	XLAT_INDEXED(EPROBE_DEFER),
	XLAT_INDEXED(EOPENSTALE),
	XLAT_INDEXED(EBADHANDLE),
	XLAT_INDEXED(ENOTSYNC),
	XLAT_INDEXED(EBADCOOKIE),
	XLAT_INDEXED(ENOTSUPP),
	XLAT_INDEXED(ETOOSMALL),
	XLAT_INDEXED(ESERVERFAULT),
	XLAT_INDEXED(EBADTYPE),
	XLAT_INDEXED(EJUKEBOX),
	XLAT_INDEXED(EIOCBQUEUED),
	XLAT_INDEXED(ERECALLCONFLICT),
};

const char *get_errmsg(unsigned int num)
{
	if (num >= ARRAY_SIZE(err_msgs))
		return "unknown error";
	return err_msgs[num];
}

const char *get_errname(unsigned int num)
{
	if (num >= ARRAY_SIZE(err_names))
		return NULL;
	return err_names[num];
}

bool is_error_erestart(unsigned int err)
{
	return err == ERESTARTSYS
		   || err == ERESTARTNOINTR
		   || err == ERESTARTNOHAND
		   || err == ERESTART_RESTARTBLOCK;
}