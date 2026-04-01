#include "../ft_print.h"
#include "../ft_utils.h"
#include "sys.xlat.h"
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <sys/utsname.h>

SYS_FUNC(uname)
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

SYS_FUNC(umask)
{
	FIRST_ARG("mask");
	printumode(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

void printrlimit(struct s_td *td, __kernel_ulong_t addr)
{
	struct rlimit buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
		printaddr(addr);
	else
	{
		print_struct_start();
		PRINT_MEMBER(buf, rlim_cur, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, rlim_max, PRINT_LU);
		print_struct_end();
	}
}

SYS_FUNC(getrlimit)
{
	if (entering(*td))
	{
		FIRST_ARG("resource");
		printflag_indexed(rlimit_resources, td->sc_args[0], "RLIMIT_???");

		return 0;
	}
	else
	{
		NEXT_ARG("rlim");
		if (td->sc_err)
			printaddr(td->sc_args[1]);
		else
			printrlimit(td, td->sc_args[1]);

		return SF_DECODE_COMPLETE;
	}
}

SYS_FUNC(getrusage)
{
	if (entering(*td))
	{
		FIRST_ARG("who");
		printflag(rusage_who, td->sc_args[0], "RUSAGE_???");

		return 0;
	}
	else
	{
		NEXT_ARG("rusage");
		if (td->sc_err)
			printaddr(td->sc_args[1]);
		else
			printrusage(td, td->sc_args[1]);

		return SF_DECODE_COMPLETE;
	}
}

void printsysinfo(struct s_td *td, __kernel_ulong_t addr)
{
	struct sysinfo buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
		printaddr(addr);
	else
	{
		print_struct_start();
		PRINT_MEMBER(buf, uptime, PRINT_L);
		print_next_struct_member("loads");
		print_arr_start();
		PRINT_LLU(buf.loads[0]);
		print_comment("1min");
		print_arg_sep();
		PRINT_LLU(buf.loads[1]);
		print_comment("5min");
		print_arg_sep();
		PRINT_LLU(buf.loads[2]);
		print_comment("15min");
		print_arr_end();
		print_struct_member_sep();
		PRINT_MEMBER(buf, totalram, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, freeram, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, sharedram, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, bufferram, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, totalswap, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, freeswap, PRINT_LU);
		print_struct_member_sep();
		PRINT_MEMBER(buf, procs, PRINT_U);
		print_struct_end();
	}
}

SYS_FUNC(sysinfo)
{
	if (exiting(*td))
	{
		FIRST_ARG("info");
		printsysinfo(td, td->sc_args[0]);
	}

	return 0;
}

void printtms(struct s_td *td, __kernel_ulong_t addr)
{
	struct tms buf = {};
	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
		printaddr(addr);
	else
	{
		print_struct_start();
		PRINT_MEMBER(buf, tms_utime, PRINT_L);
		print_struct_member_sep();
		PRINT_MEMBER(buf, tms_stime, PRINT_L);
		print_struct_member_sep();
		PRINT_MEMBER(buf, tms_cutime, PRINT_L);
		print_struct_member_sep();
		PRINT_MEMBER(buf, tms_cstime, PRINT_L);
		print_struct_end();
	}
}

SYS_FUNC(times)
{
	if (exiting(*td))
	{
		FIRST_ARG("tms");
		printtms(td, td->sc_args[0]);
	}

	return 0;
}

SYS_FUNC(setuid16)
{
	FIRST_ARG("uid");
	PRINT_ID((unsigned short) td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setuid)
{
	FIRST_ARG("uid");
	PRINT_ID(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setpgid)
{
	FIRST_ARG("pid");
	PRINT_ID(td->sc_args[0]);

	FIRST_ARG("pgid");
	PRINT_ID(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setreuid16)
{
	FIRST_ARG("ruid");
	PRINT_ID((unsigned short) td->sc_args[0]);

	NEXT_ARG("euid");
	PRINT_ID((unsigned short) td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setreuid)
{
	FIRST_ARG("ruid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("euid");
	PRINT_ID(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setresuid16)
{
	FIRST_ARG("ruid");
	PRINT_ID((unsigned short) td->sc_args[0]);

	NEXT_ARG("euid");
	PRINT_ID((unsigned short) td->sc_args[1]);

	NEXT_ARG("suid");
	PRINT_ID((unsigned short) td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setresuid)
{
	FIRST_ARG("ruid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("euid");
	PRINT_ID(td->sc_args[1]);

	NEXT_ARG("suid");
	PRINT_ID(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

void umove_print_id(struct s_td *td, __kernel_ulong_t addr, size_t id_t_size)
{
	__uid_t buf = 0;

	if (umovemem(td, &buf, addr, sizeof(buf)) < 0)
		return printaddr(addr);
	print_arg_start();
	if (id_t_size == sizeof(short))
	{
		PRINT_ID((unsigned short) buf);
	}
	else
		PRINT_ID(buf);
	print_arg_end();
}

void do_getresuid(struct s_td *td, size_t id_t_size, bool is_gid)
{
	is_gid ? FIRST_ARG("rgidp") : FIRST_ARG("ruidp");
	umove_print_id(td, td->sc_args[0], id_t_size);

	is_gid ? NEXT_ARG("egidp") : NEXT_ARG("euidp");
	umove_print_id(td, td->sc_args[1], id_t_size);

	is_gid ? NEXT_ARG("sgidp") : NEXT_ARG("suidp");
	umove_print_id(td, td->sc_args[2], id_t_size);
}

SYS_FUNC(getresuid16)
{
	if (exiting(*td))
		do_getresuid(td, sizeof(__kernel_old_uid_t), false);
	return 0;
}

SYS_FUNC(getresuid)
{
	if (exiting(*td))
		do_getresuid(td, sizeof(__kernel_uid_t), false);
	return 0;
}

SYS_FUNC(setresgid16)
{
	FIRST_ARG("rgid");
	PRINT_ID((__kernel_old_gid_t) td->sc_args[0]);

	NEXT_ARG("egid");
	PRINT_ID((__kernel_old_gid_t) td->sc_args[1]);

	NEXT_ARG("sgid");
	PRINT_ID((__kernel_old_gid_t) td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setresgid)
{
	FIRST_ARG("rgid");
	PRINT_ID(td->sc_args[0]);

	NEXT_ARG("egid");
	PRINT_ID(td->sc_args[1]);

	NEXT_ARG("sgid");
	PRINT_ID(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(getresgid16)
{
	if (exiting(*td))
		do_getresuid(td, sizeof(__kernel_old_uid_t), false);
	return 0;
}

SYS_FUNC(getresgid)
{
	if (exiting(*td))
		do_getresuid(td, sizeof(__kernel_uid_t), true);
	return 0;
}

SYS_FUNC(getpgid)
{
	FIRST_ARG("pid");
	PRINT_D(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setfsuid16)
{
	FIRST_ARG("uid");
	PRINT_ID((unsigned short) td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setfsuid)
{
	FIRST_ARG("uid");
	PRINT_ID(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setfsgid16)
{
	FIRST_ARG("gid");
	PRINT_ID((unsigned short) td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setfsgid)
{
	FIRST_ARG("gid");
	PRINT_ID(td->sc_args[0]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(getpriority)
{
	FIRST_ARG("which");
	printflag_indexed(priorities, td->sc_args[0], "PRIO_???");

	NEXT_ARG("who");
	PRINT_D(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setpriority)
{
	FIRST_ARG("which");
	printflag_indexed(priorities, td->sc_args[0], "PRIO_???");

	NEXT_ARG("who");
	PRINT_D(td->sc_args[1]);

	NEXT_ARG("niceval");
	PRINT_D(td->sc_args[2]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(prctl)
{
	FIRST_ARG("option");
	printflag(prctl_options, td->sc_args[0], "PR_???");

	// tons of unnecessary if else logging, skipped
	NEXT_ARG("arg2");
	printaddr(td->sc_args[1]);
	NEXT_ARG("arg3");
	printaddr(td->sc_args[2]);
	NEXT_ARG("arg4");
	printaddr(td->sc_args[3]);
	NEXT_ARG("arg5");
	printaddr(td->sc_args[4]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(setrlimit)
{
	FIRST_ARG("resource");
	printflag_indexed(rlimit_resources, td->sc_args[0], "RLIMIT_???");

	NEXT_ARG("rlim");
	printrlimit(td, td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(sethostname)
{
	FIRST_ARG("name");
	printnstr(td, td->sc_args[0], td->sc_args[1]);

	NEXT_ARG("len");
	PRINT_U(td->sc_args[1]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(gethostname)
{
	if (exiting(*td))
	{
		FIRST_ARG("name");
		printnstr(td, td->sc_args[0], td->sc_args[1]);

		NEXT_ARG("len");
		PRINT_U(td->sc_args[1]);
	}

	return 0;
}

SYS_FUNC(prlimit64)
{
	FIRST_ARG("pid");
	PRINT_D(td->sc_args[0]);

	NEXT_ARG("resource");
	printflag_indexed(rlimit_resources, td->sc_args[1], "RLIMIT_???");

	NEXT_ARG("new_limit");
	printrlimit(td, td->sc_args[2]);

	NEXT_ARG("old_limit");
	printrlimit(td, td->sc_args[3]);

	return SF_DECODE_COMPLETE;
}

SYS_FUNC(getcpu)
{
	if (exiting(*td))
	{
		FIRST_ARG("cpu");
		printnum_addr_uint32(td, td->sc_args[0]);

		NEXT_ARG("node");
		printnum_addr_uint32(td, td->sc_args[1]);

		NEXT_ARG("tcache");
		printaddr(td->sc_args[2]);
	}

	return 0;
}