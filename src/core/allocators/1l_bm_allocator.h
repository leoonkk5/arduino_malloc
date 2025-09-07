#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * @file 1l_bm_allocator.h
 * @brief Single-level bitmap-based memory allocator.
 *
 * Provides low-level allocation/free functions for managing
 * a fixed heap. 
 */
static void *_1l_bm_allocate(size_t size);
static void  _1l_bm_free(void *ptr, size_t size);