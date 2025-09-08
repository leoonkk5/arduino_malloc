# Compiler
CC      := gcc
CFLAGS  := -Wall -Wextra -g -O0

# Library source files
SRC     := src/ar_malloc.c src/core/allocators/1l_bm_allocator.c

# Test source files (each has its own main)
TEST_SRC := $(wildcard tests/*.c)

# Build directory
BUILD   := build

# Convert each test .c file into a binary name in build/
TEST_BIN := $(patsubst tests/%.c,$(BUILD)/%,$(TEST_SRC))

# Default target: build all tests
all: $(TEST_BIN)

# Pattern rule: compile each test file into its own binary
$(BUILD)/%: tests/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $(SRC) $<

# Run all tests
# Run all tests with summary
run: all
	@passed=0; failed=0; \
	for test in $(TEST_BIN); do \
		echo "Running $$test..."; \
		./$$test; \
		if [ $$? -eq 0 ]; then \
			passed=$$((passed+1)); \
		else \
			failed=$$((failed+1)); \
		fi; \
		echo ""; \
	done; \
	echo "=== Test Summary ==="; \
	echo "Passed: $$passed"; \
	echo "Failed: $$failed"; \
	if [ $$failed -ne 0 ]; then exit 1; fi


# Address sanitizer build
asan: CFLAGS += -fsanitize=address
asan: clean all

# Clean build
clean:
	rm -rf $(BUILD)
