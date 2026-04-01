void PRINTTIMESPEC_STRUCT(void *raw_pt)
{
	STRUCT_TIMESPEC *pt = (STRUCT_TIMESPEC *) raw_pt;
	print_struct_start();
	PRINT_MEMBER(*pt, tv_sec, PRINT_LL);
	print_struct_member_sep();
	print_next_struct_member(TO_STRING(PRECISION_FIELD));
	PRINT_LL(pt->PRECISION_FIELD);
	print_struct_end();

	printtime(pt->tv_sec, pt->PRECISION_FIELD);
}

const char *SPRINTTIMESPEC_STRUCT(void *raw_pt)
{
	STRUCT_TIMESPEC *pt = (STRUCT_TIMESPEC *) raw_pt;
	static char buf[sizeof("{tv_sec=18446744073709551615, tv_usec=18446744073709551615}")];
	inprint(buf, "{tv_sec=%lld, " TO_STRING(PRECISION_FIELD) "=%lld}", (long long) pt->tv_sec, (long long) pt->PRECISION_FIELD);
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

void PRINTITIMERSPEC(struct s_td *td, __kernel_ulong_t addr)
{
	STRUCT_ITIMERSPEC buf = {};
	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;
	print_struct_start();
	PRINT_MEMBER_ADDR(buf, it_interval, PRINTTIMESPEC_STRUCT);
	print_struct_member_sep();
	PRINT_MEMBER_ADDR(buf, it_value, PRINTTIMESPEC_STRUCT);
	print_struct_end();
}

void PRINTTIMESPEC_UTIMES(struct s_td *td, __kernel_ulong_t addr)
{
	STRUCT_TIMESPEC buf[2] = {};
	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;
	print_arr_start();
	PRINTTIMESPEC_STRUCT(&buf[0]);
	print_arr_sep();
	PRINTTIMESPEC_STRUCT(&buf[1]);
	print_arr_end();
}