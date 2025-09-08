// test_heap_exhaustion.c
#include "support/test_utils.h"
#include "support/test_utils.h"
#include "../src/ar_malloc.h"

#include <stdint.h>
#include <stdio.h>
#define HEAP_SIZE 2048
static uint8_t heap_data[HEAP_SIZE];
uint8_t *heap = heap_data;  // allocator uses this buffer

// Reset the heap before each test
static void setUp(void) 
{
    for (int i = 0; i < HEAP_SIZE; i++) heap_data[i] = 0;
}

void test_heap_exhaustion(void) 
{
    setUp();

    void* blocks[256];  
    int count = 0;

    // Keep allocating 8-byte blocks until ar_malloc returns NULL
    while (1) {
        void* ptr = ar_malloc(8);
        if (!ptr) break;
        blocks[count++] = ptr;
    }

    printf("Allocated %d blocks of 8 bytes before exhaustion\n", count);
    ASSERT_TRUE(count > 0); 

    // Free all allocated blocks
    for (int i = 0; i < count; i++) 
    {
        ar_free(blocks[i]);
    }

    for (int i = 0; i < 1000; i++) 
    {
        void *ptr = ar_malloc(512);
        ASSERT_TRUE(ptr != NULL);
        ar_free(ptr);
    }

    TEST_PASS("heap_exhaustion");
}

int main(void) 
{
    printf("=== Running heap_exhaustion test ===\n");
    test_heap_exhaustion();
    printf("All tests passed!\n");
    return 0;
}
