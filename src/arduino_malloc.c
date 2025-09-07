// arduino_malloc.c
#include "arduino_malloc.h"
#include <stddef.h>

typedef struct 
{
    size_t allocation_size; // Size of this allocation.
}AllocHeader;

/* API: */
void *malloc(size_t size)
{
    if(size == 0) size = 1;

    size_t total_size = size + sizeof(AllocHeader); 
    AllocHeader *header = _internal_allocate(total_size);
    if(!header) return NULL;

    header->allocation_size = total_size;
    return header + 1;  
}

void free(void *ptr)
{   
    if(!ptr) return;
    AllocHeader *header = ((AllocHeader*)ptr - 1);
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