// arduino_malloc.c
#include "arduino_malloc.h"
#include "core/internal_allocator.h"
#include <stddef.h>
#include <string.h> // For memset.

typedef struct 
{
    size_t allocation_size; 
}AllocHeader;

size_t ar_get_alloc_size(void *ptr) 
{
    if(!ptr) return 0;
    AllocHeader *header = ((AllocHeader*)ptr - 1);
    return header->allocation_size - sizeof(AllocHeader);
}

void *ar_malloc(size_t size)
{
    if(size == 0) size = 1;  // Ensure we allocate at least 1 byte

    // Compute total size: user-requested size + header to track allocation
    size_t total_size = size + sizeof(AllocHeader); 

    // Allocate memory via internal backend
    AllocHeader *header = _internal_allocate(total_size);
    if(!header) return NULL;

    header->allocation_size = total_size;
    return header + 1; // Return a pointer to the memory just after the header (user memory)
}

void ar_free(void *ptr)
{   
    if(!ptr) return;
    // Retrieve the header located just before the user memory
    AllocHeader *header = ((AllocHeader*)ptr - 1);

    // Free the entire allocation, including the header
    _internal_free((void*)header, header->allocation_size);
}

void *ar_calloc(size_t nelems, size_t size) 
{
    size_t bytes = nelems * size;
    uint8_t *ptr = ar_malloc(bytes);
    memset(ptr, 0, bytes);
    return ptr;
}

void *ar_realloc(void *ptr, size_t new_size) 
{
    if(!ptr) return ar_malloc(new_size);
    AllocHeader *header = (AllocHeader*)ptr - 1;
    if(header->allocation_size == new_size) return ptr;

    uint8_t *new_ptr = ar_malloc(new_size);
    if(!new_ptr) return NULL;

    size_t copy_size = header->allocation_size < new_size ? header->allocation_size : new_size;
    memcpy(new_ptr, ptr, copy_size);

    ar_free(ptr);
    return new_ptr;
}