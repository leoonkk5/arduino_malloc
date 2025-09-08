/**
 * @file 1l_bm_allocator.h
 * @brief Single-level bitmap-based memory allocator.
 *
 * This header provides low-level allocation and free functions for
 * managing a fixed-size heap using a single-level bitmap.
 * Used internally by ar_malloc/ar_free.
 */
#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Allocate a memory block from the fixed heap.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 *
 * @note This allocator does not include metadata headers; 
 *       it directly manages raw heap memory using a bitmap.
 * @warning This is a low-level internal function; higher-level allocators
 *          (e.g., `ar_malloc`) should be used by user code.
 */
void *_1l_bm_allocate(size_t size);

/**
 * @brief Free a memory block previously allocated with `_1l_bm_allocate`.
 *
 * @param ptr Pointer to the memory block to free.
 * @param size Size of the memory block in bytes.
 *
 * @note This function relies on the size being exactly the same as 
 *       what was passed to `_1l_bm_allocate`.
 * @warning This is a low-level internal function; user code should
 *          use higher-level free functions like `ar_free`.
 */
void  _1l_bm_free(void *ptr, size_t size);