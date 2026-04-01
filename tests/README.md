# Syscall Smoke Tests

This folder auto-detects implemented syscalls from:

- `src/syscall_ent_64.h`
- `src/syscall_ent_32.h`

and generates one small test program per syscall for each ABI.

## What is generated

- `generated/64/<syscall>.c`
- `generated/32/<syscall>.c`
- `syscalls_64.list`
- `syscalls_32.list`
- `syscalls_common.list`
- `syscalls_64_only.list`
- `syscalls_32_only.list`

Each test performs a direct `syscall(2)` with intentionally invalid arguments.
The test is considered successful when the syscall exists (anything except `ENOSYS`).

Exit codes:

- `0`: syscall exists and was callable
- `77`: test skipped (`ENOSYS`, dangerous syscall, or unsupported `__NR_*` macro)

## Build and run

From project root:

```sh
make -C tests gen
make -C tests tests64
make -C tests tests32
make -C tests run64
make -C tests run32
```

Build both ABIs:

```sh
make -C tests all
```

Run both ABIs:

```sh
make -C tests run
```

## Notes

- `tests32` requires 32-bit toolchain and multilib development packages.
- Some lifecycle syscalls are intentionally skipped to avoid destabilizing test runs.
