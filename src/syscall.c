#include "ft_common.h"
#include "ft_print.h"
#include "ft_utils.h"
#include "regs.h"
#include <stdbool.h>
#include <string.h>
#include <sys/ptrace.h>

static union
{
		t_reg64 r64;
		t_reg32 r32;
} g_regs;

#define IS_ABI_32 (g_io.iov_len == sizeof(REGS_32))
#define REGS_64   g_regs.r64
#define REGS_32   g_regs.r32
#define REG_SP    (IS_ABI_32 ? (REGS_32.esp) : (REGS_64.rsp))

#ifdef DEBUG_ME
#  define UGLY_ABI_FIND(e)                                                                    \
	  (__kernel_ulong_t) e >= (__kernel_ulong_t) & sysent0[0]                                 \
			  && (__kernel_ulong_t) e < ((__kernel_ulong_t) & sysent0[0]) + sizeof(sysent0) ? \
		  "64 bit" :                                                                          \
		  "32 bit"
#  define UGLY_ABI_MARK(ent_, opr_) \
	  print_debug("Marking <%s> in %s abi for \"%s\"\n", ent_->call_name, UGLY_ABI_FIND(ent_), opr_)
#else
#  define UGLY_ABI_FIND(e)
#  define UGLY_ABI_MARK(ent_, opr_)
#endif

t_entry sysent0[] = {
#include "syscall_ent_64.h"
};
t_entry sysent1[] = {
#include "syscall_ent_32.h"
};

t_reg64 *get_reg_64(void)
{
	return &(g_regs.r64);
}

enum e_abi_sysent_size
{
	sysent_size0 = ARRAY_SIZE(sysent0),
	sysent_size1 = ARRAY_SIZE(sysent1),
};

enum e_abi_wordsize
{
	abi_wordsize0 = 8,
	abi_wordsize1 = 4,
};

enum e_abi_klongsize
{
	abi_klongsize0 = 8,
	abi_klongsize1 = 4,
};

#define ABI_STR IS_ABI_32 ? "32 bit" : "64 bit"

t_entry *sysents[] = {
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

t_entry       *sysent = sysent0;
unsigned int   sysent_size = sysent_size0;

enum e_abi     current_abi = ABI_64BIT;
unsigned int   current_wordsize = abi_wordsize0;
unsigned int   current_klongsize = abi_klongsize0;

struct iovec   g_io = {.iov_base = &g_regs};

extern bool    g_flag_trace;

static t_entry sysent_stub = {
	.call_name = "unkown",
	.logger = printargs,
	.flags = 0,
};

static inline void update_current_abi(struct s_td *td, enum e_abi abi)
{
	if (current_abi == abi)
		return;

	print_flush("[ Process PID=%d is running in %s mode ] \n", td->pid, ABI_STR);
	(td->abi) = current_abi = abi;
	sysent = sysents[current_abi];
	sysent_size = sysent_sizes[current_abi];
	current_wordsize = wordsizes[current_abi];
	current_klongsize = klongsizes[current_abi];
}

long get_reg_set(struct s_td *td)
{
	g_io.iov_len = sizeof(g_regs);
	return ptrace(PTRACE_GETREGSET, td->pid, NT_PRSTATUS, &g_io);
}

// this operation can get an error
int set_stack_pointer_to(struct s_td *td, __kernel_ulong_t *sp)
{
	if (get_reg_set(td) == -1)
		return -1;
	*sp = (__kernel_ulong_t) REG_SP;
	return 0;
}

void set_sc_no(struct s_td *td)
{
	td->sc_no = -1;
	td->entry = NULL;

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

	if (!sysent[td->sc_no].logger)
	{
		td->entry = &sysent_stub;
	}
	else
		td->entry = &sysent[td->sc_no];
}

bool entry_iter(const char *name, void (*f)(t_entry *))
{
	bool matched_any = false;
	for (size_t i = 0; i < ARRAY_SIZE(sysents); ++i)
	{
		for (size_t ii = 0; ii < sysent_sizes[i]; ii++)
		{
			t_entry *cur = &sysents[i][ii];
			if (cur->call_name && strcmp(name, cur->call_name) == 0)
			{
				f(cur);
				matched_any = true;
			}
		}
	}
	return matched_any;
}

static void entr_op_trace(t_entry *e)
{
	UGLY_ABI_MARK(e, "trace");
	e->flags |= SENFL_TRACE;
}

static void entr_op_verbose(t_entry *e)
{
	UGLY_ABI_MARK(e, "verbose");
	e->flags |= SENFL_VERBOSE;
}

bool mark_syscall_to_trace(const char *scname)
{
	return scname[0] != '\0' && entry_iter(scname, entr_op_trace);
}

bool mark_syscall_to_verbose(const char *scname)
{
	return scname[0] != '\0' && entry_iter(scname, entr_op_verbose);
}

void mark_syscall_verbose_all(void)
{
	print_debug("Marking all for \"verbose\"\n");
	for (size_t i = 0; i < ARRAY_SIZE(sysents); ++i)
	{
		for (size_t ii = 0; ii < sysent_sizes[i]; ii++)
		{
			t_entry *cur = &sysents[i][ii];
			if (cur->call_name)
				cur->flags |= SENFL_VERBOSE;
		}
	}
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

static void clear_td(struct s_td *td)
{
	td->flags = 0;
	td->sc_no = -1;
	td->entry = NULL;
	td->sc_err = 0;
	td->sc_ret = 0;
	td_free_carry(td);
}

void syscallstart(struct s_td *td)
{
	set_sc_no(td);
	get_syscall_args(td);
	if (g_flag_trace == is_traced(*td))
	{
		t_entry *ent = td->entry;
		print_syscall_enter(ent->call_name);
		td->flags |= ent->logger(td);
	}
	td->flags |= TD_INSYSCALL;
	fflush(FT_OUTFILE);
}

void syscallend(struct s_td *td)
{
	fill_trace_data_exiting(td);
	if (g_flag_trace == is_traced(*td))
	{
		t_entry *ent = td->entry;
		if (td->flags & SF_AFTER_RETURN)
		{
			print_syscall_return(td);
			print_space();
			ent->logger(td);
		}
		else
		{
			if (!(td->flags & SF_DECODE_COMPLETE))
				td->flags |= ent->logger(td);
			print_syscall_return(td);
		}
		td->flags &= ~SF_MASK;
		print_syscall_end();
		update_current_abi(td, td->abi);
	}
	td->flags &= ~TD_INSYSCALL;
	td->sc_err = 0;
	clear_td(td);
}

int syscall_noop(struct s_td *td)
{
	(void) td;
	return SF_DECODE_COMPLETE;
}