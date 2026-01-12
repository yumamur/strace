#include "../ft_common.h"
#include "../ft_print.h"

#include <linux/fs.h>
#include <linux/ioctl.h>

// Waiting response from 42 pedago

void printioctl_op(unsigned int op)
{
	const unsigned int dir = _IOC_DIR(op);
	const unsigned int type = _IOC_TYPE(op);
	const unsigned int nr = _IOC_NR(op);
	const unsigned int size = _IOC_SIZE(op);

	print_struct_start();

	print_struct_member("dir");
	PRINT_ULL(dir);

	print_struct_member("type");
	print_quot_char(type);

	print_next_struct_member("nr");
	PRINT_ULL(nr);

	print_next_struct_member("size");
	PRINT_ULL(size);

	print_struct_end();
}

SYS_FUNC(ioctl)
{
	FIRST_ARG("fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("op");
	printioctl_op((unsigned int) td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}