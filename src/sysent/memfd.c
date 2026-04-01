#include "../ft_print.h"
#include "memfd.xlat.h"

#define MFD_HUGE_SIZE (MFD_HUGE_MASK << MFD_HUGE_SHIFT)

SYS_FUNC(memfd_create)
{
	FIRST_ARG("name");
	printnstr(td, td->sc_args[0], 250);

	NEXT_ARG("flags");
	unsigned int flags = td->sc_args[1] & ~MFD_HUGE_SIZE;
	unsigned int huge_size = (td->sc_args[1] & MFD_HUGE_SIZE) >> MFD_HUGE_SHIFT;
	printflags(memfd_create_flags, flags, "MFD_???");
	if (huge_size)
	{
		print_or();
		PRINT_U(huge_size);
		prints("<<MFD_HUGE_SIZE");
	}

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(memfd_secret)
{
	FIRST_ARG("flags");
	printflags(memfd_secret_flags, td->sc_args[0], "MFD_???");

	return SF_DECODE_COMPLETE;
}