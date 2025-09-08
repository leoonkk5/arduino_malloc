// test_malloc.c
#include "support/test_utils.h"
#include "../src/ar_malloc.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HEAP_SIZE 2048
static uint8_t heap_data[HEAP_SIZE];
uint8_t *heap = heap_data;  // allocator will use this buffer

// Reset the heap before each test (if your allocator supports it)
static void setUp(void) {
    memset(heap_data, 0, HEAP_SIZE);
}

void test_malloc_and_free(void) {
    setUp();

    void* a = ar_malloc(8);
    void* b = ar_malloc(8);
    void* c = ar_malloc(32);
    size_t a_size = ar_get_alloc_size(a);
    size_t b_size = ar_get_alloc_size(b);
    size_t c_size = ar_get_alloc_size(c);   
    ASSERT_TRUE(a_size == 8);
    ASSERT_TRUE(b_size == 8);
    ASSERT_TRUE(c_size == 32);
    ASSERT_TRUE(a != NULL);
    ASSERT_TRUE(b != NULL);
    ASSERT_TRUE(a != b);

    ar_free(a);
    ar_free(b);
    ar_free(c);

    // Check if the allocator handles zero correctly.
    void *z = ar_malloc(0);
    ASSERT_TRUE(z != NULL);
    ASSERT_TRUE(ar_get_alloc_size(z) >= 1); 
    ar_free(z);

    // Try allocating 64 bytes
    uint8_t* ptr = (uint8_t*)ar_malloc(64);
    ASSERT_TRUE(ptr != NULL);

    // Fill memory to ensure it's writable
    memset(ptr, 0xAA, 64);
    for (int i = 0; i < 64; i++) {
        ASSERT_EQUAL(ptr[i], 0xAA);
    }

    ar_free(ptr);

    void* blocks[50];
    // Allocate 50 blocks of 32 bytes
    for (int i = 0; i < 50; i++) {
        blocks[i] = ar_malloc(2);
        ASSERT_TRUE(blocks[i] != NULL);
    }

    // Free every other block
    for (int i = 0; i < 50; i += 2) {
        ar_free(blocks[i]);
    }

    // Reallocate in the freed slots
    for (int i = 0; i < 50; i += 2) {
        blocks[i] = ar_malloc(4);
        ASSERT_TRUE(blocks[i] != NULL);
    }

    // Free all blocks
    for (int i = 0; i < 50; i++) {
        ar_free(blocks[i]);
    }

    TEST_PASS("malloc/free");
}

int main(void) {
    printf("=== Running malloc/free test ===\n");
    test_malloc_and_free();
    printf("All tests passed!\n");
    return 0;
}
