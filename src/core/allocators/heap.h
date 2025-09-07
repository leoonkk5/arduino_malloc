#pragma once
#include <stdint.h>
/**
 * @file heap.h
 * @brief This file defines the heap section of the program,
 * Separated into a single header to easily tweak heap parameters accross multiple allocator implementations.
 * 
 */

#define HEAP_SIZE   (512)
// Heap pointer: either rely on __bss_end on Arduino, or extern heap elsewhere for testing.
#ifdef ARDUINO
extern char __bss_end;
#define HEAP_START ((uint8_t *)&__bss_end)
#else
extern uint8_t *heap;   // user must provide &heap_array[0] on desktop
#define HEAP_START (heap)
#endif