void PRINTTIMEX_TIME_FNAME(const typeof(((STRUCT_TIMEX *)0)->time) *time)
{
	print_struct_start();
	PRINT_MEMBER_D(*time, tv_sec);
	print_struct_member_sep();
	PRINT_MEMBER_D(*time, tv_usec);
	print_struct_end();
}

void PRINTTIMEX_FNAME(struct s_td *td, __kernel_ulong_t addr)
{
	STRUCT_TIMEX buf = {};

	if (umovemem_or_printaddr(td, &buf, addr, sizeof(buf)))
		return;

	print_struct_start();

	PRINT_MEMBER_FLAGS(buf, modes, adjtimex_modes, "ADJ_???");
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, offset);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, freq);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, maxerror);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, esterror);
	print_struct_member_sep();
	PRINT_MEMBER_FLAGS(buf, status, adjtimex_status, "STA_???");
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, constant);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, precision);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, tolerance);
	print_struct_member_sep();
	PRINT_MEMBER_ADDR(buf, time, PRINTTIMEX_TIME_FNAME);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, tick);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, ppsfreq);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, jitter);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, shift);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, stabil);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, jitcnt);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, calcnt);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, errcnt);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, stbcnt);
	print_struct_member_sep();
	PRINT_MEMBER_LL(buf, tai);
	print_struct_member_sep();

	print_struct_end();
}
