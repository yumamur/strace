#include "../ft_print.h"
#include "../ft_utils.h"
#include "capability.xlat.h"

void print_user_cap_header_struct(struct s_td *td, __kernel_ulong_t addr)
{
	struct __user_cap_header_struct hdr;
	if (umovemem_or_printaddr(td, &hdr, addr, sizeof(hdr)) < 0)
		return;
	print_struct_start();
	print_struct_member("version");
	printflag(capability_version, hdr.version, "CAPABILITY_VERSION_???");
	print_struct_member_sep();
	print_struct_member("pid");
	PRINT_D(hdr.pid);
	print_struct_end();
}

void print_caps(uint32_t lo, uint32_t hi)
{
	uint64_t caps = (uint64_t) lo | ((uint64_t) hi << 32);
	printflags(capslo, caps, "CAP_???");
}

void print_user_cap_data_struct(struct s_td *td, __kernel_ulong_t addr)
{
	struct __user_cap_data_struct databuf[2];
	if (umovemem_or_printaddr(td, &databuf, addr, sizeof(databuf)) < 0)
		return;
	print_struct_start();
	print_struct_member("effective");
	print_caps(databuf[0].effective, databuf[1].effective);
	print_next_struct_member("permitted");
	print_caps(databuf[0].permitted, databuf[1].permitted);
	print_next_struct_member("inheritable");
	print_caps(databuf[0].inheritable, databuf[1].inheritable);
	print_struct_end();
}

SYS_FUNC(capget)
{
	if (entering(*td))
	{
		FIRST_ARG("hdrp");
		print_user_cap_header_struct(td, td->sc_args[0]);
	}
	else
	{
		NEXT_ARG("datap");
		if (td->sc_err)
			printaddr(td->sc_args[1]);
		else
			print_user_cap_data_struct(td, td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(capset)
{
	FIRST_ARG("hdrp");
	print_user_cap_header_struct(td, td->sc_args[0]);

	NEXT_ARG("datap");
	print_user_cap_data_struct(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}