#include "ft_print.h"
#include "ft_syscall.h"
#include "ft_utils.h"
#include "regs.h"
#include "trace.h"
#include <stdbool.h>
#include <string.h>

static union
{
		t_reg64 r64;
		t_reg32 r32;
} g_regs;

#define REGS_64 g_regs.r64
#define REGS_32 g_regs.r32

#define IS_ABI_32 (g_io.iov_len == sizeof(REGS_32))

t_entry sysent0[] = {
#include "syscall_ent_64.h"
};
t_entry sysent1[] = {
#include "syscall_ent_32.h"
};
t_entry sysent2[] = {
#include "syscall_ent_32.h"
};

enum e_abi_sysent_size
{
	sysent_size0 = ARRAY_SIZE(sysent0),
	sysent_size1 = ARRAY_SIZE(sysent1),
	sysent_size2 = ARRAY_SIZE(sysent2)
};

enum e_abi_wordsize
{
	abi_wordsize0 = 8,
	abi_wordsize1 = 4,
	abi_wordsize2 = 4
};

enum e_abi_klongsize
{
	abi_klongsize0 = 8,
	abi_klongsize1 = 4,
	abi_klongsize2 = 8
};

const char *abi_names[] = {"64 bit", "32 bit"};

t_entry    *sysents[] = {
    [ABI_64BIT] = sysent0,
    [ABI_32BIT] = sysent1,
};

static const unsigned int sysent_sizes[] = {
	[ABI_64BIT] = ARRAY_SIZE(sysent0),
	[ABI_32BIT] = ARRAY_SIZE(sysent1),
};

static const unsigned int wordsizes[] = {
	[ABI_64BIT] = abi_wordsize0,
	[ABI_32BIT] = abi_wordsize1,
};

static const unsigned int klongsizes[] = {
	[ABI_64BIT] = abi_klongsize0,
	[ABI_32BIT] = abi_klongsize1,
};

t_entry           *sysent = sysent0;
unsigned int       sysent_size = sysent_size0;

enum e_abi         current_abi = ABI_64BIT;
unsigned int       current_wordsize = abi_wordsize0;
unsigned int       current_klongsize = abi_klongsize0;

struct iovec       g_io = {.iov_base = &g_regs};

extern bool        g_flag_trace;

static inline void update_current_abi(struct s_td *td, enum e_abi abi)
{
	if (current_abi == abi)
		return;

	print_flush("[ Process PID=%d is running in %s mode ] \n", td->pid, abi_names[abi]);
	(td->abi) = current_abi = abi;
	sysent = sysents[current_abi];
	sysent_size = sysent_sizes[current_abi];
	current_wordsize = wordsizes[current_abi];
	current_klongsize = klongsizes[current_abi];
}

void set_sc_no(struct s_td *td)
{
	if (IS_ABI_32)
	{
		td->sc_no = REGS_32.orig_eax;
		update_current_abi(td, ABI_32BIT);
	}
	else
	{
		td->sc_no = REGS_64.orig_rax;
		update_current_abi(td, ABI_64BIT);
	}
}

// TODO: Find any other way than this
int mark_syscall_to_trace(const char *scname)
{
	int matched_any = 0;
	for (size_t i = 0; i < ARRAY_SIZE(sysents); ++i)
	{
		for (size_t ii = 0; ii < sysent_sizes[i]; ii++)
		{
			t_entry *cur = &sysents[i][ii];
			if (cur->call_name && strcmp(scname, cur->call_name) == 0)
			{
				print_debug("Marking %ld numbered syscall (%s) to trace in %s mode\n",
							ii,
							cur->call_name,
							abi_names[i]);
				matched_any = cur->traced = 1;
			}
		}
	}
	return matched_any;
}

void get_syscall_args(struct s_td *td)
{
	if (IS_ABI_32)
	{
		td->sc_args[0] = (uint32_t) REGS_32.ebx;
		td->sc_args[1] = (uint32_t) REGS_32.ecx;
		td->sc_args[2] = (uint32_t) REGS_32.edx;
		td->sc_args[3] = (uint32_t) REGS_32.esi;
		td->sc_args[4] = (uint32_t) REGS_32.edi;
		td->sc_args[5] = (uint32_t) REGS_32.ebp;
	}
	// else if (IS_ABI_X32)
	// {
	// 	td->sc_args[0] = (uint32_t) REGS_64.rdi;
	// 	td->sc_args[1] = (uint32_t) REGS_64.rsi;
	// 	td->sc_args[2] = (uint32_t) REGS_64.rdx;
	// 	td->sc_args[3] = (uint32_t) REGS_64.r10;
	// 	td->sc_args[4] = (uint32_t) REGS_64.r8;
	// 	td->sc_args[5] = (uint32_t) REGS_64.r9;
	// }
	else
	{
		td->sc_args[0] = REGS_64.rdi;
		td->sc_args[1] = REGS_64.rsi;
		td->sc_args[2] = REGS_64.rdx;
		td->sc_args[3] = REGS_64.r10;
		td->sc_args[4] = REGS_64.r8;
		td->sc_args[5] = REGS_64.r9;
	}
}

bool is_rax_error(__kernel_ulong_t rax)
{
	unsigned long max = -(long) MAX_ERRNO_VAL;
	if (current_klongsize < sizeof(rax))
	{
		rax = (uint32_t) rax;
		max = (uint32_t) max;
	}
	return rax >= max;
}

void fill_trace_data_exiting(struct s_td *td)
{
	long long rax;

	bool      is_32 = IS_ABI_32;
	if (is_32)
		rax = (int32_t) REGS_32.eax;
	else
		rax = REGS_64.rax;

	if (is_rax_error(rax))
	{
		td->sc_ret = -1;
		td->sc_err = (uint32_t) rax;
	}
	else
	{
		if (is_32)
			td->sc_ret = (uint32_t) rax;
		else
			td->sc_ret = rax;
	}
}

void syscallstart(struct s_td *td)
{
	set_sc_no(td);
	get_syscall_args(td);
	t_entry *ent = &sysent[td->sc_no];
	if (g_flag_trace == ent->traced)
	{
		print_syscall_enter(ent->call_name);
		td->flags |= ent->logger(td);
	}
	td->flags |= TD_INSYSCALL;
}

void syscallend(struct s_td *td)
{
	fill_trace_data_exiting(td);
	t_entry *ent = &sysent[td->sc_no];
	if (g_flag_trace == ent->traced)
	{
		if (td->flags & SC_AFTER_RETURN)
		{
			print_syscall_return(td);
			print_space();
			ent->logger(td);
		}
		else
		{
			if (!(td->flags & SC_DECODE_COMPLETE))
				td->flags |= ent->logger(td);
			print_syscall_return(td);
		}
		td->flags &= ~SC_MASK;
		print_syscall_end();
		update_current_abi(td, td->abi);
	}
	td->flags &= ~TD_INSYSCALL;
	td->sc_err = 0;
}