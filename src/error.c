#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void __attribute__((__noreturn__, __format__(__printf__, 1, 0)))
perror_and_die(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fflush(NULL);
	vfprintf(stderr, fmt, va);
	fflush(stderr);
	va_end(va);
	exit(1);
}