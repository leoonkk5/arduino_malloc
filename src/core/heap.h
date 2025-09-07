#pragma once
#include <stdint.h>

/**
 * @file heap.h
 * @brief Defines the heap section of the program.
 * 
 * This header provides a flexible way to define heap memory for
 * different platforms (e.g., Arduino, desktop testing).
 * Users can tweak HEAP_SIZE according to the available RAM.
 */

/* ===================== Heap Size Configuration ===================== */
/* Default heap size if not defined elsewhere */
#ifndef HEAP_SIZE
    #ifdef ARDUINO_AVR_UNO
        #define HEAP_SIZE  1024   // ATmega328P, ~2KB SRAM, leave some for stack
    #elif defined(ARDUINO_AVR_MEGA2560)
        #define HEAP_SIZE  8192   // ATmega2560, 8KB SRAM, safe default
    #elif defined(ARDUINO_SAMD_ZERO)
        #define HEAP_SIZE  16384  // SAMD21, 32KB SRAM, half for heap
    #else
        #define HEAP_SIZE  2048   // Fallback default
    #endif
#endif

/* ===================== Heap Pointer ===================== */
/**
 * HEAP_START points to the start of our heap memory.
 * On Arduino, we can use the end of the .bss segment.
 * On desktop/testing, user must provide a static heap array.
 */
#ifdef ARDUINO
    extern char __bss_end;
    #define HEAP_START ((uint8_t *)&__bss_end)
#else
    extern uint8_t *heap;  // User must define: uint8_t heap[HEAP_SIZE];
    #define HEAP_START (heap)
#endif
