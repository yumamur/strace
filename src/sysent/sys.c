#include "../ft_print.h"
#include "../ft_utils.h"
#include <sys/utsname.h>

SYS_FUNC(newuname)
{
	struct utsname un;

	if (entering(*td))
		return 0;

	FIRST_ARG("buf");
	if (umovemem(td, &un, td->sc_args[0], sizeof(un)) < 0)
		printaddr(td->sc_args[0]);
	else
	{
		print_struct_start();
		PRINT_MEMBER_STRQ(un, sysname);
		print_struct_member_sep();
		PRINT_MEMBER_STRQ(un, nodename);
		print_struct_member_sep();
		PRINT_MEMBER_STRQ(un, release);
		print_struct_member_sep();
		PRINT_MEMBER_STRQ(un, version);
		print_struct_member_sep();
		PRINT_MEMBER_STRQ(un, machine);
		print_struct_member_sep();

#ifdef __USE_GNU
		PRINT_MEMBER_STRQ(un, domainname);
#else
		print_struct_member("domainname");
		putquotstr(un.__domainname, sizeof(un.__domainname));
#endif

		print_struct_end();
	}
	return SF_DECODE_COMPLETE;
}