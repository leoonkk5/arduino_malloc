// test_calloc.c
#include "support/test_utils.h"
#include "../src/ar_malloc.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HEAP_SIZE 2048
static uint8_t heap_data[HEAP_SIZE];
uint8_t *heap = heap_data;  

static void setUp(void) 
{
    memset(heap_data, 0xFF, HEAP_SIZE); // fill with non-zero to check zeroing
}

void test_calloc_basic(void) 
{
    setUp();

    size_t num = 1;
    size_t size = 16;
    uint8_t* ptr = (uint8_t*)ar_calloc(num, size);
    ASSERT_TRUE(ptr != NULL);
    
    // Check that it's zeroed
    for (size_t i = 0; i < num * size; i++) {
        ASSERT_EQUAL(ptr[i], 0);
    }

    // Check size reporting
    ASSERT_TRUE(ar_get_alloc_size(ptr) >= num * size);

    ar_free(ptr);

    // Allocate multiple blocks
    num = 4;
    size = 8;
    ptr = (uint8_t*)ar_calloc(num, size);
    ASSERT_TRUE(ptr != NULL);

    for (size_t i = 0; i < num * size; i++) 
    {
        ASSERT_EQUAL(ptr[i], 0);
    }

    ar_free(ptr);

    // Check calloc with zero elements
    ptr = (uint8_t*)ar_calloc(0, 16);
    ASSERT_TRUE(ptr != NULL);
    ASSERT_TRUE(ar_get_alloc_size(ptr) >= 1); // Should return some usable block
    ar_free(ptr);

    // Stress test: allocate many small blocks
    uint8_t* blocks[50];
    for (int i = 0; i < 50; i++) 
    {
        blocks[i] = (uint8_t*)ar_calloc(2, 2);
        ASSERT_TRUE(blocks[i] != NULL);

        // Verify zeroing
        for (int j = 0; j < 4; j++)
        {
            ASSERT_EQUAL(blocks[i][j], 0);
        }
    }

    // Free every other block
    for (int i = 0; i < 50; i += 2) 
    {
        ar_free(blocks[i]);
    }

    // Reallocate in freed slots
    for (int i = 0; i < 50; i += 2) 
    {
        blocks[i] = (uint8_t*)ar_calloc(1, 4);
        ASSERT_TRUE(blocks[i] != NULL);
        for (int j = 0; j < 4; j++) 
        {
            ASSERT_EQUAL(blocks[i][j], 0);
        }
    }

    // Free all
    for (int i = 0; i < 50; i++) 
    {
        ar_free(blocks[i]);
    }

    TEST_PASS("calloc");
}

int main(void) 
{
    printf("=== Running calloc test ===\n");
    test_calloc_basic();
    printf("All tests passed!\n");
    return 0;
}
