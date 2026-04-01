#include "../ft_print.h"
#include "../ft_utils.h"
#include "landlock.xlat.h"

void printlandlock_ruleset_attr(struct s_td *td, __kernel_ulong_t addr, size_t size)
{
	struct landlock_ruleset_attr buf;

	if (size != sizeof(buf))
		return printaddr(addr);

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();
	PRINT_MEMBER_FLAGS(buf, handled_access_fs,
					   landlock_ruleset_fs_actions, "LANDLOCK_ACCESS_FS_???");
	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(buf, handled_access_net,
					   landlock_ruleset_net_actions, "LANDLOCK_ACCESS_NET_???");
// this exists on newer versions, but we are using ubuntu 22 and it does not exist.
//	print_struct_member_sep();
//	PRINT_MEMBER_FLAGS(buf, scoped,
//					   landlock_ruleset_scope_flags, "LANDLOCK_SCOPE_???");
	print_struct_end();
}

SYS_FUNC(landlock_create_ruleset)
{
	FIRST_ARG("attr");
	printlandlock_ruleset_attr(td, td->sc_args[0], td->sc_args[1]);

	NEXT_ARG("size");
	PRINT_LU(td->sc_args[1]);

	NEXT_ARG("flags");
	printflags(landlock_create_ruleset_flags, td->sc_args[2], "LANDLOCK_CREATE_RULESET_???");

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(landlock_add_rule)
{
	FIRST_ARG("ruleset_fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("rule_type");
	printflag(landlock_rule_types, td->sc_args[1], "LANDLOCK_RULE_???");

	NEXT_ARG("rule_attr");
	printaddr(td->sc_args[2]);

	NEXT_ARG("flags");
	// probably not implemented, but the call fails with EINVAL if flags is not 0
	PRINT_X(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(landlock_restrict_self)
{
	FIRST_ARG("ruleset_fd");
	printfd(td->sc_args[0]);

	NEXT_ARG("flags");
	// same as above
	PRINT_X(td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}
