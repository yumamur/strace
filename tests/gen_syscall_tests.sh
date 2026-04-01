#!/usr/bin/env sh
set -eu

SELF_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SELF_DIR/.." && pwd)

SRC64="$ROOT_DIR/src/syscall_ent_64.h"
SRC32="$ROOT_DIR/src/syscall_ent_32.h"
LIST64="$SELF_DIR/syscalls_64.list"
LIST32="$SELF_DIR/syscalls_32.list"
LIST_COMMON="$SELF_DIR/syscalls_common.list"
LIST_64_ONLY="$SELF_DIR/syscalls_64_only.list"
LIST_32_ONLY="$SELF_DIR/syscalls_32_only.list"
GEN64_DIR="$SELF_DIR/generated/64"
GEN32_DIR="$SELF_DIR/generated/32"

if [ ! -f "$SRC64" ] || [ ! -f "$SRC32" ]; then
	echo "error: syscall entry headers were not found" >&2
	exit 1
fi

extract_list() {
	awk '
		/^[[:space:]]*\/\// { next }
		/^[[:space:]]*#/ { next }
		{
			if ($0 ~ /^[[:space:]]*\[[0-9]/ && match($0, /"[^"]+"/)) {
				name = substr($0, RSTART + 1, RLENGTH - 2)
				if (name != "")
					print name
			}
		}
	' "$1" | LC_ALL=C sort -u
}

is_skip_syscall() {
	case "$1" in
		exit|exit_group|fork|vfork|clone|clone3|pause|rt_sigsuspend|sigsuspend|rt_sigreturn|sigreturn|uretprobe)
			return 0
			;;
		*)
			return 1
			;;
	esac
}

generate_source() {
	arch="$1"
	sysname="$2"
	out_dir="$SELF_DIR/generated/$arch"
	out_file="$out_dir/$sysname.c"

	mkdir -p "$out_dir"

	if is_skip_syscall "$sysname"; then
		cat > "$out_file" <<EOF
#define _GNU_SOURCE
#include <stdlib.h>

int main(void)
{
	/* Skip known dangerous lifecycle syscalls in smoke tests. */
	return 77;
}
EOF
		return
	fi

	cat > "$out_file" <<EOF
#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

static void on_alarm(int sig)
{
	(void) sig;
}

int main(void)
{
#ifdef __NR_$sysname
	errno = 0;
	signal(SIGALRM, on_alarm);
	alarm(1);
	long rc = syscall(__NR_$sysname, -1L, -1L, -1L, -1L, -1L, -1L);
	(void) rc;
	alarm(0);
	return errno == ENOSYS ? 77 : 0;
#else
	return 77;
#endif
}
EOF
}

extract_list "$SRC64" > "$LIST64"
extract_list "$SRC32" > "$LIST32"

LC_ALL=C comm -12 "$LIST64" "$LIST32" > "$LIST_COMMON"
LC_ALL=C comm -23 "$LIST64" "$LIST32" > "$LIST_64_ONLY"
LC_ALL=C comm -13 "$LIST64" "$LIST32" > "$LIST_32_ONLY"

rm -rf "$GEN64_DIR" "$GEN32_DIR"
mkdir -p "$GEN64_DIR" "$GEN32_DIR"

while IFS= read -r name; do
	[ -n "$name" ] || continue
	generate_source "64" "$name"
done < "$LIST64"

while IFS= read -r name; do
	[ -n "$name" ] || continue
	generate_source "32" "$name"
done < "$LIST32"

printf 'generated %s 64-bit syscall tests\n' "$(wc -l < "$LIST64")"
printf 'generated %s 32-bit syscall tests\n' "$(wc -l < "$LIST32")"
printf 'common syscalls: %s\n' "$(wc -l < "$LIST_COMMON")"
printf '64-only syscalls: %s\n' "$(wc -l < "$LIST_64_ONLY")"
printf '32-only syscalls: %s\n' "$(wc -l < "$LIST_32_ONLY")"
