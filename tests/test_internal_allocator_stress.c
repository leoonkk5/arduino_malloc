// test_internal_allocator_stress.c
#include "support/test_utils.h"
#include "../src/core/internal_allocator.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HEAP_SIZE 2048
static uint8_t heap_data[HEAP_SIZE];
uint8_t *heap = heap_data;  // allocator buffer

static void setUp(void) {
    memset(heap_data, 0, HEAP_SIZE);
}

void test_internal_allocator_stress(void) {
    setUp();

    void* blocks[50];

    // Allocate a batch of blocks
    for (int i = 0; i < 50; i++) {
        blocks[i] = _internal_allocate(32);
        ASSERT_TRUE(blocks[i] != NULL);
        memset(blocks[i], i, 32); // fill with pattern
    }

    // Free every other block to create fragmentation
    for (int i = 0; i < 50; i += 2) {
        _internal_free(blocks[i], 32);
    }

    // Reallocate into the freed slots
    for (int i = 0; i < 50; i += 2) {
        blocks[i] = _internal_allocate(32);
        ASSERT_TRUE(blocks[i] != NULL);
        memset(blocks[i], i + 1, 32);
    }

    // Repeated small allocations until exhaustion
    void* temp_blocks[128];
    int count = 0;
    while ((temp_blocks[count] = _internal_allocate(16)) != NULL && count < 128) {
        memset(temp_blocks[count], count, 16);
        count++;
    }
    printf("Allocated %d small blocks of 16 bytes before exhaustion\n", count);

    // Free temporary blocks
    for (int i = 0; i < count; i++) {
        _internal_free(temp_blocks[i], 16);
    }

    // Free all original blocks ---
    for (int i = 0; i < 50; i++) {
        _internal_free(blocks[i], 32);
    }

    TEST_PASS("internal allocator stress test");
}

int main(void) {
    printf("=== Running internal allocator stress test ===\n");
    test_internal_allocator_stress();
    printf("All tests passed!\n");
    return 0;
}
