#include "ft_utils.h"

size_t count_set_bits(void *addr, size_t size)
{
	if (!addr)
		return 0;

	unsigned char *bytes = (unsigned char *) addr;
	size_t         count = 0;

	for (size_t i = 0; i < size; i++)
	{
		unsigned char byte = bytes[i];
		while (byte)
		{
			count += byte & 1;
			byte >>= 1;
		}
	}

	return count;
}