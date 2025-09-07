// arduino_malloc.c
#include "arduino_malloc.h"
#include <stddef.h>

typedef struct 
{
    size_t allocation_size; 
}AllocHeader;

void *malloc(size_t size)
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

void free(void *ptr)
{   
    if(!ptr) return;
    // Retrieve the header located just before the user memory
    AllocHeader *header = ((AllocHeader*)ptr - 1);

    // Free the entire allocation, including the header
    _internal_free((void*)header, header->allocation_size);
}

void *calloc(size_t nelems, size_t size) 
{
    return NULL; // Todo: implement
}

void *realloc(void *ptr, size_t new_size) 
{
    return NULL;  // Todo: implement
}