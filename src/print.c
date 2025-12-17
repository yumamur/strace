#include "ft_print.h"
#include "ft_utils.h"

int printpath(t_td *td, __kernel_ulong_t addr)
{
	char path[4096];

	if (!addr)
		return -1;

	int null_idx = umovestr(td, path, addr, sizeof(path) - 1);
	if (null_idx < 0)
		printaddr(addr);
	else if (null_idx)
		printquotstr(path, (size_t) null_idx ?: sizeof(path));
	return null_idx;
}

int printflags(const t_xlat *xlat, uint64_t flags, const char *dflt)
{
	if (!(xlat && xlat->data && xlat->size))
		return -1;

	if (!flags)
		return 0;

	for (unsigned int i = 0; i < xlat->size; i++)
	{
		
	}
}
