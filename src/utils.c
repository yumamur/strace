#include "ft_utils.h"
#include <stdlib.h>

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

// struct s_map *map_create(unsigned cap, unsigned key_size, unsigned val_size)
// {
// 	if (cap < 1 || key_size > sizeof(uint64_t))
// 		return (NULL);

// 	struct s_map *m = malloc(sizeof(struct s_map));
// 	if (!m)
// 		return (NULL);

// 	m->nodes = calloc(cap, key_size + val_size);
// 	if (!m->nodes)
// 	{
// 		free(m);
// 		return (NULL);
// 	}
// 	m->cap = cap;
// 	m->size = 0;
// 	m->key_size = key_size;
// 	m->val_size = val_size;
// }

// void map_destroy(struct s_map map)
// {
// }

// bool map_has_key(struct s_map, uint64_t)
// {
// }
// struct s_map_ent *map_get_node(struct s_map, uint64_t)
// {
// }
// struct s_map_ent *map_add_node(struct s_map, uint64_t, void *, size_t n)
// {
// }
// void map_del_node(struct s_map, uint64_t)
// {
// }
