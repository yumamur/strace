#include "../ft_print.h"
#include "../ft_utils.h"
#include "namespace.xlat.h"

SYS_FUNC(mount)
{
	FIRST_ARG("source");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("target");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("filesystemtype");
	if (td->sc_args[3]
		& (MS_REMOUNT | MS_BIND | MS_MOVE | MS_UNBINDABLE | MS_PRIVATE | MS_SLAVE | MS_SHARED))
		printstr(td, td->sc_args[2]);
	else
		printaddr(td->sc_args[2]);

	NEXT_ARG("mountflags");
	printflags(mount_flags, td->sc_args[3], "MS_???");

	NEXT_ARG("data");
	printaddr(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(umount2)
{
	FIRST_ARG("target");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("flags");
	printflags(umount_flags, td->sc_args[1], "MNT_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(open_tree)
{
	FIRST_ARG("dirfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("filename");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(open_tree_flags, td->sc_args[2], "OPEN_TREE_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(move_mount)
{
	FIRST_ARG("from_dfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("from_pathname");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("to_dfd");
	printdirfd(td, td->sc_args[2]);

	NEXT_ARG("to_pathname");
	printpath(td, td->sc_args[3]);

	NEXT_ARG("flags");
	printflags(move_mount_flags, td->sc_args[2], "MOVE_MOUNT_???");

	return SF_DECODE_COMPLETE;
}

void printmount_attr(struct s_td *td, __kernel_ulong_t addr, size_t size)
{
	struct mount_attr buf;

	if (size < sizeof(buf))
		return printaddr(addr);

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_FLAGS(buf, attr_set, mount_attr_attributes, "MOUNT_ATTR_???");
	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(buf, attr_clr, mount_attr_attributes, "MOUNT_ATTR_???");
	print_struct_member_sep();
	PRINT_MEMBER_FLAG(buf, propagation, mount_attr_propagation_types, "MS_???");
	print_struct_member_sep();
	PRINT_MEMBER(buf, userns_fd, printfd);
	print_struct_end();
}

SYS_FUNC(mount_setattr)
{
	FIRST_ARG("dfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("path");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(mount_setattr_flags, td->sc_args[2], "AT_???");

	NEXT_ARG("uattr");
	printmount_attr(td, td->sc_args[3], td->sc_args[4]);

	NEXT_ARG("usize");
	PRINT_LU(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(open_tree_attr)
{
	FIRST_ARG("dfd");
	printdirfd(td, td->sc_args[0]);

	NEXT_ARG("path");
	printpath(td, td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(open_tree_flags, td->sc_args[2], "OPEN_TREE_???");

	NEXT_ARG("uattr");
	printmount_attr(td, td->sc_args[3], td->sc_args[4]);

	NEXT_ARG("usize");
	PRINT_LU(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(statmount)
{
	FIRST_ARG("req");
	printaddr(td->sc_args[0]);

	NEXT_ARG("buf");
	printaddr(td->sc_args[1]);
	if (is_verbose(*td))
		print_comment("Sorry, can't");

	NEXT_ARG("bufsize");
	PRINT_LU(td->sc_args[2]);

	NEXT_ARG("flags");
	// not implemented yet
	PRINT_X(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

int print_uint64(struct s_td *td, void *pt, size_t size)
{
	(void) td;
	(void) size;
	PRINT_LLU(*(unsigned long long *) pt);
	return PRINTARR_STATE_SEP;
}

SYS_FUNC(listmount)
{
	FIRST_ARG("req");
	printaddr(td->sc_args[0]);

	NEXT_ARG("mnt_ids");
	uint64_t buf;
	printarray(td, (t_printarray_cfg){
					   .printer = print_uint64,
					   .start_addr = td->sc_args[1],
					   .n_var = td->sc_args[2],
					   .pt_buf_var = &buf,
					   .var_size = sizeof(buf),
				   });

	NEXT_ARG("nr_mnt_ids");
	PRINT_LU(td->sc_args[2]);

	NEXT_ARG("flags");
	// not implemented yet
	PRINT_X(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}