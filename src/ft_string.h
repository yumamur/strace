#ifndef FT_STRING
#define FT_STRING

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define iprint(buf, fmt, ...) vsnprintf(buf, sizeof(buf), fmt, __VA_ARGS__)

#endif