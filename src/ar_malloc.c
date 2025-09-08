// ar_malloc.c 
#include "ar_malloc.h"
#include "core/internal_allocator.h"
#include <stddef.h>
#include <string.h> // For memset

/* Header stored before each user allocation to keep track of the total allocation size */
typedef struct 
{
    size_t allocation_size; 
}AllocHeader;

/* Returns the size of memory the user actually requested (excluding the header) */
size_t ar_get_alloc_size(void *ptr) 
{
    if(!ptr) return 0;
    // Retrieve the header located just before the user memory
    AllocHeader *header = ((AllocHeader*)ptr - 1);

    // Return the user-allocated size.
    return header->allocation_size - sizeof(AllocHeader);
}

/* Allocate memory of a given size with space for the header */
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

/* Free memory previously allocated by ar_malloc/ar_calloc/ar_realloc */
void ar_free(void *ptr)
{   
    if(!ptr) return;
    // Retrieve the header located just before the user memory
    AllocHeader *header = ((AllocHeader*)ptr - 1);

    // Free the entire allocation, including the header
    _internal_free((void*)header, header->allocation_size);
}

/* Allocate and zero-initialize memory for an array */
void *ar_calloc(size_t nelems, size_t size) 
{
    size_t bytes = nelems * size;
    uint8_t *ptr = ar_malloc(bytes);
    memset(ptr, 0, bytes);
    return ptr;
}

/* Resize a previously allocated memory block */
void *ar_realloc(void *ptr, size_t new_size) 
{
    if(!ptr) return ar_malloc(new_size); // realloc(NULL, size) is malloc(size)

    AllocHeader *header = (AllocHeader*)ptr - 1;
    if(ar_get_alloc_size(ptr) == new_size) return ptr;

    uint8_t *new_ptr = ar_malloc(new_size);
    if(!new_ptr) return NULL;

    // min(old_size, new_size)
    size_t copy_size = header->allocation_size < new_size ? header->allocation_size : new_size;
    memcpy(new_ptr, ptr, copy_size);

    ar_free(ptr);
    return new_ptr;
}