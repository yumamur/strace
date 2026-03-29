#include "../ft_print.h"
#include "../ft_utils.h"
#include "perf_event.xlat.h"

void printperf_event_attr(struct s_td *td, __kernel_ulong_t addr)
{
	struct perf_event_attr buf;

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_FLAG(buf, type, perf_event_types, "PERF_TYPE_???");
	print_struct_member_sep();
	PRINT_MEMBER_FLAG(buf, size, perf_attr_size_versions, "PERF_ATTR_SIZE_???");
	print_struct_member_sep();
	// this is parsed conditionally, so I'll just hex print it
	PRINT_MEMBER_LLU(buf, config);
	print_struct_member_sep();
	// this struct also holds 39 flags (as of today), unions, configs,
	// fields per versions... Let's just say:
	print_has_more();
	print_struct_end();
}

SYS_FUNC(perf_event_open)
{
	if (exiting(*td))
	{
		FIRST_ARG("attr");
		printaddr(td->sc_args[0]);

		NEXT_ARG("pid");
		PRINT_D(td->sc_args[1]);

		NEXT_ARG("cpu");
		PRINT_D(td->sc_args[2]);

		NEXT_ARG("groupfd");
		printfd(td->sc_args[3]);

		NEXT_ARG("flags");
		printflags(perf_flags, td->sc_args[4], "PERF_FLAG_???");
	}
	return 0;
}