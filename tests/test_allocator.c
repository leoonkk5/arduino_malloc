#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "../src/arduino_malloc.h"

#define HEAP_SIZE 512
uint8_t heap_data[HEAP_SIZE];  
uint8_t *heap = heap_data;

void test_malloc_free() {
    printf("=== Test: malloc & free ===\n");

    void* a = ar_malloc(8);
    void* b = ar_malloc(8);

    assert(a != NULL && b != NULL);
    assert(a != b); 
    ar_free(a);
    ar_free(b);

    // Try allocating 64 bytes
    uint8_t* ptr = (uint8_t*)ar_malloc(64);
    assert(ptr != NULL);  // If this fails.
    printf("Allocated 64 bytes at %p\n", ptr);

    // Fill memory with something fun
    memset(ptr, 0xAA, 64);

    // Free it
    ar_free(ptr);
    printf("Freed 64 bytes at %p\n", ptr);
}

void test_calloc() {
    printf("=== Test: calloc ===\n");

    uint8_t* ptr = (uint8_t*) ar_calloc(10, sizeof(uint8_t));
    assert(ptr != NULL);

    // Check that memory is zeroed
    for (int i = 0; i < 10; i++) {
        assert(ptr[i] == 0); 
    }

    printf("calloc allocated and zeroed 10 bytes at %p\n", ptr);
    ar_free(ptr);
}

void test_realloc() {
    printf("=== Test: realloc ===\n");

    uint8_t* ptr = (uint8_t*) ar_malloc(16);
    assert(ptr != NULL);

    // Fill memory with pattern
    memset(ptr, 0x55, 16);

    // Reallocate to bigger size (let's see if we survive)
    uint8_t* new_ptr = (uint8_t*) ar_realloc(ptr, 32);
    assert(new_ptr != NULL);

    // Check old data is preserved
    for (int i = 0; i < 16; i++) {
        assert(new_ptr[i] == 0x55);
    }

    printf("Reallocated 16 bytes -> 32 bytes at %p (old was %p)\n", new_ptr, ptr);
    ar_free(new_ptr);
}

void test_heap_exhaustion() {
    printf("=== Test: heap exhaustion ===\n");

    // Keep allocating small blocks until heap gives up
    void* blocks[256];  // 8-byte chunks
    int count = 0;

    while (1) {
        void* ptr = ar_malloc(8);
        if (!ptr) break; 
        blocks[count++] = ptr;
    }

    printf("Allocated %d blocks of 8 bytes before exhaustion\n", count);

    // Free all blocks
    for (int i = 0; i < count; i++) {
        ar_free(blocks[i]);
    }

    printf("Freed all blocks after exhaustion test\n");
}

int main() {
    printf("Starting Arduino lightweight allocator tests...\n\n");

    test_malloc_free();
    test_calloc();
    test_realloc();
    test_heap_exhaustion();

    printf("\nAll tests passed!\n");
    return 0;
}
