#include "../ft_print.h"
#include "userfaultfd.xlat.h"

SYS_FUNC(userfaultfd)
{
	FIRST_ARG("flags");
	printflags(uffd_flags, td->sc_args[0], "UFFD_??");

	return SF_DECODE_COMPLETE;
}