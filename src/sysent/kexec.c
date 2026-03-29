#include "../ft_print.h"
#include "kexec.xlat.h"

int print_kexec_segment_struct(struct s_td *td, void *addr, size_t memb_size)
{
	(void) td;
	const __kernel_ulong_t *pt;
	__kernel_ulong_t        aligned[4];

	if (memb_size < sizeof(struct kexec_segment))
	{
		for (unsigned int i = 0; i < 4; i++)
			aligned[i] = ((unsigned int *) addr)[i];
		pt = aligned;
	}
	else
		pt = (const __kernel_ulong_t *) addr;

	print_struct_start();
	print_struct_member("buf");
	printaddr(pt[0]);
	print_next_struct_member("bufsz");
	PRINT_LU(pt[1]);
	print_next_struct_member("mem");
	printaddr(pt[2]);
	print_next_struct_member("memsz");
	PRINT_LU(pt[3]);
	print_struct_end();
	return PRINTARR_STATE_SEP;
}

SYS_FUNC(kexec_load)
{
	FIRST_ARG("entry");
	printaddr(td->sc_args[0]);

	NEXT_ARG("nr_segments");
	PRINT_LU(td->sc_args[1]);

	NEXT_ARG("segments");
	struct kexec_segment buf;
	printarray(td, (t_printarray_cfg) {
					   .printer = print_kexec_segment_struct,
					   .start_addr = td->sc_args[2],
					   .pt_buf_var = &buf,
					   .n_var = td->sc_args[1],
					   .var_size = sizeof(buf),
				   });

	NEXT_ARG("flags");
	__kernel_ulong_t flags = td->sc_args[3];
	printflag(kexec_archs, flags & KEXEC_ARCH_MASK, "KEXEC_ARCH_???");
	flags &= ~KEXEC_ARCH_MASK;
	if (flags)
	{
		print_or();
		printflags(kexec_flags, flags, "KEXEC_???");
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(kexec_file_load)
{
	FIRST_ARG("kernel_fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("initrd_fd");
	printfd(td->sc_args[1]);

	NEXT_ARG("cmdline_len");
	PRINT_LU(td->sc_args[2]);

	NEXT_ARG("cmdline");
	printnstr(td, td->sc_args[3], td->sc_args[2]);

	NEXT_ARG("flags");
	printflags(kexec_file_load_flags, td->sc_args[4], "KEXEC_FILE_???");

	return SF_DECODE_COMPLETE;
}