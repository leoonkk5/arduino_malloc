/**
 * @file heap.h
 * @brief Defines the heap section of the program.
 *
 * This header provides a flexible way to define heap memory for
 * different platforms (e.g., Arduino, desktop testing). Users
 * can tweak HEAP_SIZE according to the available RAM.
 */
#pragma once
#include <stdint.h>


/**
 * @brief Size of the heap in bytes.
 *
 * If HEAP_SIZE is not defined elsewhere, defaults are set
 * depending on the platform:
 * - ATmega328P (Arduino Uno): 1024 bytes
 * - ATmega2560 (Arduino Mega): 4096 bytes
 * - SAMD21 (Arduino Zero): 16384 bytes
 * - Fallback default: 2048 bytes
 */
#ifndef HEAP_SIZE
    #ifdef ARDUINO_AVR_UNO
        #define HEAP_SIZE  1024   // ATmega328P, ~2KB SRAM, leave some for stack
    #elif defined(ARDUINO_AVR_MEGA2560)
        #define HEAP_SIZE  4096   // ATmega2560, 8KB SRAM, half for heap
    #elif defined(ARDUINO_SAMD_ZERO)
        #define HEAP_SIZE  16384  // SAMD21, 32KB SRAM, half for heap
    #else
        #define HEAP_SIZE  512   // Fallback default
    #endif
#endif

/** 
 * @brief Pointer to the start of the heap.
 *
 * On Arduino, this is typically the end of the .bss segment.
 * On desktop/testing, the user must provide a static heap array.
 */
#ifdef ARDUINO
    extern char __bss_end;            /**< End of .bss segment */
    #define HEAP_START ((uint8_t *)&__bss_end)
#else
    extern uint8_t *heap;             /**< User-defined heap array: uint8_t heap[HEAP_SIZE]; */
    #define HEAP_START (heap)
#endif
