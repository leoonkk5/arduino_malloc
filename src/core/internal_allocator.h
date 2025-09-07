#pragma once
#include "allocators/1l_bm_allocator.h"  // one-level bitmap allocator backend

/**
 * @file internal_allocator.h
 * @brief Dispatch layer for backend memory allocation in Arduino projects.
 *
 * This header provides an abstraction layer for the actual memory allocation
 * implementation. By using `_internal_allocate` and `_internal_free`, the
 * frontend allocator (e.g., `malloc`, `free`) can remain unchanged while
 * using different backend implementations (bitmap, pool, etc.).
 *
 * Currently uses:
 * - One-level bitmap allocator (`_1l_bm_allocate` / `_1l_bm_free`)
 *
 * Future expansions can switch allocator implementation dynamically.
 */

/**
 * @brief Allocate memory using the current backend allocator.
 *
 * This function dispatches the request to the selected allocator backend.
 * Currently implemented using the one-level bitmap allocator.
 *
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if allocation fails
 *
 * @note This is an internal function; user code should call `malloc` instead.
 */
static inline void* _internal_allocate(size_t size)
{
    return _1l_bm_allocate(size);
}

/**
 * @brief Free memory previously allocated by `_internal_allocate`.
 *
 * This function dispatches the request to the selected allocator backend.
 * Currently implemented using the one-level bitmap allocator.
 *
 * @param ptr Pointer to memory to free
 * @param size Size of the memory block that was allocated
 *
 * @note This is an internal function; user code should call `free` instead.
 */
static inline void _internal_free(void* ptr, size_t size)
{
    _1l_bm_free(ptr, size);
}
