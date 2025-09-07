#pragma once
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file arduino_malloc.h
 * @brief Lightweight memory allocator for Arduino projects.
 *
 * This header provides simple memory management functions for
 * Arduino environment.
 * Designed to give programmers functional heap allocations, 
 * without the massive memory overhead made by many other allocators.
 *
 * Functions included:
 * - ar_malloc:    allocate memory
 * - ar_free:      free memory allocated by bm_malloc
 * - ar_calloc:    allocate and zero-initialize memory
 * - ar_realloc:   resize previously allocated memory
 */


/**
 * @brief Allocate memory of a given size.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 *
 * Example:
 * @code
 * uint8_t* buffer = (uint8_t*)ar_malloc(32);
 * if(buffer) { ..use buffer  }
 * @endcode
 */
void *ar_malloc(size_t size);

/**
 * @brief Free previously allocated memory.
 *
 * @param ptr Pointer to the memory block to free.
 *
 * Example:
 * @code
 * ar_free(buffer);
 * @endcode
 */
void ar_free(void *ptr);

/**
 * @brief Allocate memory for an array and initialize all bytes to zero.
 *
 * @param nelems Number of elements.
 * @param size Size of each element in bytes.
 * @return Pointer to the allocated and zero-initialized memory, or NULL if allocation fails.
 *
 * Example:
 * @code
 * uint8_t* bitmap = (uint8_t*)ar_calloc(128, sizeof(uint8_t));
 * @endcode
 */
void *ar_calloc(size_t nelems, size_t size);

/**
 * @brief Resize a previously allocated memory block.
 *
 * @param ptr Pointer to the existing memory block.
 * @param new_size New size in bytes.
 * @return Pointer to the resized memory block, or NULL if reallocation fails.
 *
 * Example:
 * @code
 * buffer = (uint8_t*)ar_realloc(buffer, 2048);
 * @endcode
 */
void *ar_realloc(void *ptr, size_t new_size);

#ifdef __cplusplus
}
#endif