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

static inline int test_bit(const uint8_t *bytes, size_t idx) {
    return (bytes[idx / 8] >> (idx % 8)) & 1;
}

// set or clear a bit
static inline void set_bit(uint8_t *bytes, size_t idx) {
    bytes[idx / 8] |= (1 << (idx % 8));
}
static inline void clear_bit(uint8_t *bytes, size_t idx) {
    bytes[idx / 8] &= ~(1 << (idx % 8));
}
