# ft_strace

A Linux syscall tracer inspired by `strace`.

`ft_strace` runs a target program under `ptrace(2)`, intercepts syscall entry/exit,
and prints decoded syscall arguments and return values.

## Features

- Traces syscall flow of a spawned process
- Decodes many syscall arguments via per-syscall handlers in `src/sysent/`
- Supports syscall filtering with `--trace=`
- Supports verbose argument naming with `-v` or selective verbose mode via `--verbose=`
- Handles ABI transitions (32-bit and 64-bit process modes)

## Repository Layout

- `src/`: tracer source code
- `src/sysent/`: syscall-specific decoders and translation tables
- `tests/`: generated syscall smoke tests (32-bit and 64-bit)

## Build

Requirements:

- Linux
- `gcc`
- `make`

From project root:

```sh
make
```

Build variants:

```sh
make de   # debug build (-g -DDEBUG_ME)
make san  # address sanitizer build
```

Clean:

```sh
make clean
make fclean
```

## Usage

General form:

```sh
./ft_strace [options] <program> [args...]
```

Examples:

```sh
./ft_strace echo hello
./ft_strace --trace=openat,read,write cat /etc/hostname
./ft_strace -v ls -l
./ft_strace --verbose=openat,execve echo hi
```

### Options

- `--trace=name1,name2,...`
  - Trace only the listed syscalls.
- `-v`
  - Enable verbose formatting for all syscalls (prints argument names where available).
- `--verbose=name1,name2,...`
  - Enable verbose formatting only for selected syscalls.
- `-c`
  - Currently parsed by CLI, but call-count summary output is not fully implemented yet.

## Notes and Limitations

- Linux-only (uses `ptrace`, register sets, and Linux syscall tables).
- The tracer launches a new process; attaching to an already-running PID is not implemented.
- Behavior and decoder coverage depend on implemented handlers in `src/sysent/`.
- Some syscall names may differ across kernels/architectures.

## Tests

Syscall smoke tests are in `tests/` and can auto-generate one test per detected syscall.

Quick run from project root:

```sh
make -C tests gen
make -C tests tests64
make -C tests run64
```

For 32-bit test binaries (requires multilib toolchain):

```sh
make -C tests tests32
make -C tests run32
```

See `tests/README.md` for full details.

## Troubleshooting

- `Cannot find executable ...`
  - Use an absolute path or make sure the binary is in `PATH`.
- `ptrace` errors (`Operation not permitted`)
  - Check ptrace restrictions (for example `kernel.yama.ptrace_scope`) and permissions.
- 32-bit test build failures
  - Install 32-bit development libraries and compiler multilib support.

## License

The Unlicense (public domain). See `LICENSE` for details.