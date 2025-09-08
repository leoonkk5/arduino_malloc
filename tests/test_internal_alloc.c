// test_internal_alloc.c
#include "support/test_utils.h"
#include "../src/core/internal_allocator.h"

#include <stdint.h>
#include <stdio.h>

#define HEAP_SIZE 2048
static uint8_t heap_data[HEAP_SIZE];
uint8_t *heap = heap_data;

static void setUp(void) {
    for (int i = 0; i < HEAP_SIZE; i++) heap_data[i] = 0;
}

void test_internal_alloc(void) {
    setUp();

    void* ptr1 = _internal_allocate(16);
    void* ptr2 = _internal_allocate(32);

    ASSERT_TRUE(ptr1 != NULL);
    ASSERT_TRUE(ptr2 != NULL);
    ASSERT_TRUE(ptr1 != ptr2);

    _internal_free(ptr1, 16);
    _internal_free(ptr2, 32);

    TEST_PASS("internal allocator");
}

int main(void) {
    printf("=== Running internal allocator test ===\n");
    test_internal_alloc();
    printf("All tests passed!\n");
    return 0;
}
