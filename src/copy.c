#include "ft_common.h"
#include <string.h>
#include <sys/uio.h>
#include <syscall.h>
#include <unistd.h>

static ssize_t strace_process_vm_readv(pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags)
{
	return syscall(__NR_process_vm_readv, (long) pid, lvec, liovcnt, rvec, riovcnt, flags);
}
#define process_vm_readv strace_process_vm_readv

static ssize_t process_read_mem(const pid_t pid, void *const laddr, void *const raddr, const size_t len)
{
	const struct iovec local = {.iov_base = laddr, .iov_len = len};
	const struct iovec remote = {.iov_base = raddr, .iov_len = len};

	return process_vm_readv(pid, &local, 1, &remote, 1, 0);
}

int umovestr(t_td *const td, char *laddr, __kernel_ulong_t taddr, size_t len)
{
	const int    pid = td->pid;
	const size_t page_size = getpagesize();
	const size_t mask = page_size - 1;

	size_t       total = 0;

	if (!len)
		return 0;

	while (len)
	{
		/* man process_vm_readv(2) notes that if data overflows the page
		boundary, the call may fail or result in partial reading. So we do this */
		size_t chunk_len = len > page_size ? page_size : len;
		size_t end_in_page = (taddr + chunk_len) & mask;

		if (chunk_len > end_in_page)  /* would cross into next page */
			chunk_len -= end_in_page; /* pull back to page boundary */

		ssize_t r = process_read_mem(pid, laddr, (void *) taddr, chunk_len);
		if (r <= 0)
			return -1;

		char *nul = memchr(laddr, '\0', (size_t) r);
		if (nul)
		{
			total += (size_t) (nul - laddr) + 1;
			return (int) total;
		}

		taddr += (unsigned long) r;
		laddr += r;
		total += (size_t) r;
		len -= (size_t) r;
	}

	return 0;
}