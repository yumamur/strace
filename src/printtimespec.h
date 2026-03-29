void PRINTTIMESPEC_STRUCT(STRUCT_TIMESPEC *pt)
{
	print_struct_start();
	PRINT_MEMBER(*pt, tv_sec, PRINT_LL);
	print_struct_member_sep();
	print_next_struct_member(TO_STRING(PRECISION_FIELD));
	PRINT_LL(pt->PRECISION_FIELD);
	print_struct_end();

	printtime(pt->tv_sec, pt->PRECISION_FIELD);
}

const char *SPRINTTIMESPEC_STRUCT(STRUCT_TIMESPEC *pt)
{
	static char buf[sizeof("{tv_sec=18446744073709551615, tv_usec=18446744073709551615}")];
	inprint(buf, "{tv_sec=%lld, " TO_STRING(PRECISION_FIELD) "=%lld}", (long long) pt->PRECISION_FIELD, (long long) pt->tv_nsec);
	return buf;
}

void PRINTTIMESPEC(struct s_td *td, __kernel_ulong_t addr)
{
	STRUCT_TIMESPEC buf = {};
	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;
	PRINTTIMESPEC_STRUCT(&buf);
}

const char *SPRINTTIMESPEC(struct s_td *td, __kernel_ulong_t addr)
{
	STRUCT_TIMESPEC buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) != sizeof(buf))
		return sprintaddr(addr);
	return SPRINTTIMESPEC_STRUCT(&buf);
}
