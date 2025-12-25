#ifndef FT_STRING
#define FT_STRING

#include <stdio.h>

size_t strrspn(const char *str, const char *rmset);
char  *strtrim(const char *str, const char *rmset);
char  *stpcpy(char *dest, const char *src);

#endif