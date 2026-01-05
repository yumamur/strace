#ifndef REGS_H
#define REGS_H

#include <bits/types/struct_iovec.h>
#include <stdint.h>
#include <sys/user.h>

struct user_regs_struct_32
{
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
		uint32_t esi;
		uint32_t edi;
		uint32_t ebp;
		uint32_t eax;
		uint32_t xds;
		uint32_t xes;
		uint32_t xfs;
		uint32_t xgs;
		uint32_t orig_eax;
		uint32_t eip;
		uint32_t xcs;
		uint32_t eflags;
		uint32_t esp;
		uint32_t xss;
};

typedef struct user_regs_struct_32 t_reg32;
typedef struct user_regs_struct    t_reg64;

#endif