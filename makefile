# Makefile for running unit test on the allocator.

# Compiler
CC      := gcc
CFLAGS  := -Wall -Wextra -g -O0

# Source files
SRC     := src/arduino_malloc.c src/core/allocators/1l_bm_allocator.c # add custom backends here
TEST_SRC := tests/test_allocator.c

# Output binary
OUT := build/test_allocator

# Build unit test.
test: $(OUT)

$(OUT): $(SRC) $(TEST_SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

run: $(OUT)
	./$(OUT)

asan: CFLAGS += -fsanitize=address
asan: clean $(OUT)

clean:
	rm -f $(OUT) *.o
