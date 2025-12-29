#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

