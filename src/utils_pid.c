// #include "ft_print.h"

// #define __USE_MISC
// #include <dirent.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <limits.h>
// #include <stdlib.h>
// #include <sys/stat.h>

// static int strace_self_pid;

// int        pid_init(void)
// {
// 	int fd = open("/proc/self/ns/pid", O_RDONLY);

// 	if (fd < 0)
// 	{
// 		print_debug("where is my pid???");
// 		return (-1);
// 	}
// 	struct stat st;
// 	if (!fstat(fd, &st))
// 	{
// 		strace_self_pid = st.st_ino;
// 		return (0);
// 	}
// 	else
// 	{
// 		perror_and_cont(errno, "fstat");
// 		close(fd);
// 		return (-1);
// 	}
// }

// int translate_pid(struct s_td *td, const char *path, int pid)
// {
// 	DIR *pd_proc = opendir("/proc");

// 	if (!pd_proc)
// 		return;

// 	while (1)
// 	{
// 		errno = 0;
// 		struct dirent *d_proc = readdir(pd_proc);
// 		if (!d_proc)
// 		{
// 			if (errno)
// 				perror_and_cont(errno, "readdir");
// 			break;
// 		}

// 		if (d_proc->d_type != DT_DIR)
// 			continue;

// 		errno = 0;
// 		long proc_p = strtol(d_proc->d_name, NULL, 10);
// 		if (proc_p > INT_MAX || proc_p < 1 || errno)
// 			continue;
// 	}

// 	closedir(pd_proc);
// }
