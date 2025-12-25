
#include "ft_string.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

char *stpcpy(char *dest, const char *src)
{
	while ((*dest++ = *src++))
		;
	return dest - 1;
}

size_t strrspn(const char *str, const char *rmset)
{
	_Bool hash[256] = {0};

	if (!(str && *str && rmset && *rmset))
		return 0;

	for (int i = 0; rmset[i]; ++i)
		hash[(unsigned char) rmset[i]] = true;

	const size_t size = strlen(str);
	size_t       ri = size;

	while (ri)
		if (!hash[(unsigned char) str[--ri]])
			return size - ri - 1;

	return size;
}

char *strtrim(const char *str, const char *rmset)
{
	if (!str || !*str || !rmset || !*rmset)
		return NULL;
	str += strspn(str, rmset);
	return strndup(str, strlen(str) - strrspn(str, rmset));
}
