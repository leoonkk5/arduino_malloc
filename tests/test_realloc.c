// test_realloc.c
#include "support/test_utils.h"
#include "../src/ar_malloc.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HEAP_SIZE 2048
static uint8_t heap_data[HEAP_SIZE];
uint8_t *heap = heap_data;  

static void setUp(void) {
    memset(heap_data, 0, HEAP_SIZE);
}

void test_realloc_basic(void) {
    setUp();

    // Allocate initial block
    size_t initial_size = 16;
    uint8_t* ptr = (uint8_t*)ar_malloc(initial_size);
    ASSERT_TRUE(ptr != NULL);

    // Fill it with a recognizable pattern
    for (size_t i = 0; i < initial_size; i++) {
        ptr[i] = (uint8_t)(i + 1);
    }

    // Reallocate larger
    size_t new_size = 32;
    ptr = (uint8_t*)ar_realloc(ptr, new_size);
    ASSERT_TRUE(ptr != NULL);

    // Check that old data is preserved
    for (size_t i = 0; i < initial_size; i++) {
        ASSERT_EQUAL(ptr[i], (uint8_t)(i + 1));
    }

    // Fill the rest
    for (size_t i = initial_size; i < new_size; i++) {
        ptr[i] = (uint8_t)(i + 1);
    }

    // Shrink the block
    size_t shrink_size = 8;
    ptr = (uint8_t*)ar_realloc(ptr, shrink_size);
    ASSERT_TRUE(ptr != NULL);

    // Old data should still be there up to shrink_size
    for (size_t i = 0; i < shrink_size; i++) {
        ASSERT_EQUAL(ptr[i], (uint8_t)(i + 1));
    }

    ar_free(ptr);

    // Realloc NULL should behave like malloc
    ptr = (uint8_t*)ar_realloc(NULL, 16);
    ASSERT_TRUE(ptr != NULL);
    for (size_t i = 0; i < 16; i++) {
        ptr[i] = 0xAA; // make sure we can write
    }
    ar_free(ptr);

    // Realloc to size 0 should free the block and return a valid pointer.
    ptr = (uint8_t*)ar_malloc(16);
    ASSERT_TRUE(ptr != NULL);
    ptr = (uint8_t*)ar_realloc(ptr, 0);
    ASSERT_TRUE(ptr != NULL);
    ar_free(ptr);

    TEST_PASS("realloc");
}

int main(void) {
    printf("=== Running realloc test ===\n");
    test_realloc_basic();
    printf("All tests passed!\n");
    return 0;
}
