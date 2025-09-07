#pragma once
#include <stdint.h>

/* count leading zeroes on an 8 bit integer, with zero input defined as 8. */
static inline uint8_t clzb(uint8_t x) 
{
    if (x == 0) return 8; // all zeros
    return __builtin_clz((unsigned int)x) - (sizeof(unsigned int)*8 - 8);
}

/* count trailing zeroes on an 8 bit integer, with zero input defined as 8. */
static inline uint8_t ctzb(uint8_t x) 
{
    if (x == 0) return 8; // all zeros
    return __builtin_ctz((unsigned int)x);
}
