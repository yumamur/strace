CC := gcc
CFLAGS_COMMON := -std=c11 -O0 -g -Wall -Wextra -Werror
CFLAGS_64 := -m64
CFLAGS_32 := -m32

GEN_SCRIPT := ./gen_syscall_tests.sh
GEN_DIR := ./generated
BIN_DIR := ./bin

LIST_64 := ./syscalls_64.list
LIST_32 := ./syscalls_32.list

TIMEOUT_CMD ?= timeout
TIMEOUT_SEC ?= 2

.PHONY: all gen clean fclean tests64 tests32 run64 run32 run list

all: tests64 tests32

gen: $(GEN_SCRIPT) ../src/syscall_ent_64.h ../src/syscall_ent_32.h
	@$(GEN_SCRIPT)

list: gen
	@echo "64-bit syscall count: $$(wc -l < $(LIST_64))"
	@echo "32-bit syscall count: $$(wc -l < $(LIST_32))"
	@echo "64-bit only: $$(wc -l < ./syscalls_64_only.list)"
	@echo "32-bit only: $$(wc -l < ./syscalls_32_only.list)"

tests64: gen
	@mkdir -p $(BIN_DIR)/64
	@set -e; \
	for src in $(GEN_DIR)/64/*.c; do \
		[ -e "$$src" ] || continue; \
		out="$(BIN_DIR)/64/$$(basename "$$src" .c)"; \
		$(CC) $(CFLAGS_COMMON) $(CFLAGS_64) "$$src" -o "$$out"; \
	done
	@echo "built 64-bit syscall tests in $(BIN_DIR)/64"

tests32: gen
	@mkdir -p $(BIN_DIR)/32
	@set -e; \
	for src in $(GEN_DIR)/32/*.c; do \
		[ -e "$$src" ] || continue; \
		out="$(BIN_DIR)/32/$$(basename "$$src" .c)"; \
		$(CC) $(CFLAGS_COMMON) $(CFLAGS_32) "$$src" -o "$$out"; \
	done
	@echo "built 32-bit syscall tests in $(BIN_DIR)/32"

run64: tests64
	@set -e; \
	pass=0; skip=0; fail=0; total=0; \
	for bin in $(BIN_DIR)/64/*; do \
		[ -x "$$bin" ] || continue; \
		total=$$((total + 1)); \
		if $(TIMEOUT_CMD) $(TIMEOUT_SEC) "$$bin" >/dev/null 2>&1; then \
			pass=$$((pass + 1)); \
		else \
			ec=$$?; \
			if [ $$ec -eq 77 ]; then \
				skip=$$((skip + 1)); \
			else \
				fail=$$((fail + 1)); \
				echo "FAIL(64): $$bin (exit=$$ec)"; \
			fi; \
		fi; \
	done; \
	echo "64-bit results: total=$$total pass=$$pass skip=$$skip fail=$$fail"; \
	[ $$fail -eq 0 ]

run32: tests32
	@set -e; \
	pass=0; skip=0; fail=0; total=0; \
	for bin in $(BIN_DIR)/32/*; do \
		[ -x "$$bin" ] || continue; \
		total=$$((total + 1)); \
		if $(TIMEOUT_CMD) $(TIMEOUT_SEC) "$$bin" >/dev/null 2>&1; then \
			pass=$$((pass + 1)); \
		else \
			ec=$$?; \
			if [ $$ec -eq 77 ]; then \
				skip=$$((skip + 1)); \
			else \
				fail=$$((fail + 1)); \
				echo "FAIL(32): $$bin (exit=$$ec)"; \
			fi; \
		fi; \
	done; \
	echo "32-bit results: total=$$total pass=$$pass skip=$$skip fail=$$fail"; \
	[ $$fail -eq 0 ]

run: run64 run32

clean:
	$(RM) -r $(GEN_DIR) $(BIN_DIR)
	$(RM) $(LIST_64) $(LIST_32) ./syscalls_common.list ./syscalls_64_only.list ./syscalls_32_only.list

fclean: clean
