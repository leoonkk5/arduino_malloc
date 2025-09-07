// 1l_bm_allocator.c
#include "1l_bm_allocator.h"
#include "heap.h"
#include <stdalign.h> // for max_align_t
#include "../utils.h"

#define BLOCK_SIZE  (sizeof(max_align_t))    // aligned to max_align_t
#define BLOCK_COUNT  (HEAP_SIZE / BLOCK_SIZE)

#define BLOCKS_NEEDED(bytes) (((bytes) + (BLOCK_SIZE) - 1) / (BLOCK_SIZE))
#define BITMAP_SIZE ((BLOCK_COUNT + 7) / 8) // Round up

/*
 * Bitmap for keeping track of used blocks.
 * 0: free, can be allocated
 * 1: in use, cannot be allocated.
 * Each bit coorelates to the block in heap at : [(idx * 8 + bitpos) * BLOCK_SIZE]
 */
static uint8_t bitmap[BITMAP_SIZE];


/*  
 * Find the start index of a consecutive sequence of free blocks, 
 * atleast as long as `nblocks`
*/


/* Handle small allocations that can fit entirely within a single byte */
static inline size_t _find_small_free_index(size_t nblocks, size_t byte_idx, uint8_t b) 
{
    size_t shift = 1;
    size_t rem   = nblocks - 1;

    /*  
     * Bit-hack to find a contiguous sequence of `nblocks` free bits within the byte
     * Note: not sure if this is any faster than looping thru each bit on ARDUINO, 
     * since arduino only supports shift left/shift right by 1 bit.
     * TODO: Performance test for this VS looping thru bits.s
     */
    while(rem > 0) 
    {  
        size_t s = (shift > rem) ? shift : rem;
        b &= (b >> s);
        rem -= s;
        shift *= 2;
    }

    // Check if any sequence >= `nblocks` was found within the byte.
    if(b != 0)
    {
        return byte_idx * 8 + ctzb(b); // Return the start of the sequence found.
    }
    return (size_t)-1;
}


static inline size_t _find_free_index(size_t nblocks)
{   
    size_t seq_carry = 0;   // Consecutive free blocks carried from previous bytes
    size_t seq_start = 0;   // Index of the first block in the current candidate sequence.
    for(size_t i = 0; i < BITMAP_SIZE; i++) 
    {   
        uint8_t current  = bitmap[i];

        // Skip byte if fully occupied.
        if(current == 0xff) 
        {
            seq_carry = 0;
            seq_start = (i + 1) * 8;
            continue;
        }
        size_t entry_seq = ctzb(current);  // Count trailing zeros (consecutive free blocks at the start of this byte)

        if(entry_seq + seq_carry >= nblocks)
        {
            return seq_start;
        }

        // The entire sequence can fit in a single byte, check.
        if(nblocks <= 8) 
        {
            size_t found = _find_small_free_index(nblocks, i, current);
            if(found != (size_t)-1)
            {
                return found;
            }
        }

        // If this byte has any used blocks, the previous sequence is broken
        if(current != 0x0) 
        {
            seq_carry = 0;
            seq_start = i * 8 + (8 - clzb(current));  // index of the first zero in the carrying sequence.
        }

        // Update the carried sequence length for the next byte
        seq_carry += clzb(current);    // Add leading zeros (consecutive free blocks at the end of this byte) to the sequence carry.
    
    }

    // no blocks found.
    return (size_t)-1;
}

/*
 * Set a sequence of bits of length `count`, starting at index `idx` in the bitmap, 
 * to the value in `value`. (Either 0 or 1):
 */
static void _set_sequence(size_t idx, size_t count, uint8_t value)
{
    while (count > 0)
    {
        size_t byte_idx     = idx / 8;      // Which byte in the bitmap
        size_t bit_idx      = idx % 8;      // Which bit in that byte
        size_t bits_in_byte = 8 - bit_idx;  // How many bits we can set in this byte starting from bit_idx

        if (bits_in_byte > count) bits_in_byte = count;  // Limit to remainding count

        // Create a mask for the bits we want to set in this byte
        uint8_t mask = ((1 << bits_in_byte) - 1) << bit_idx;

        if (value) {
            bitmap[byte_idx] |= mask;       // Set bits to 1
        } else {
            bitmap[byte_idx] &= ~mask;      // Set bits to 0
        }

        // Move to the next sequence of bits
        idx   += bits_in_byte;
        count -= bits_in_byte;
    }
}

/* Allocate memory of a given size, and mark used. */
static void *_1l_bm_allocate(size_t size) 
{
    size_t nblocks = BLOCKS_NEEDED(size);

    // Fail immediately if allocation is bigger than the heap.
    if(nblocks > BLOCK_COUNT) 
    {
        return NULL;
    }

    size_t idx = _find_free_index(nblocks);

    // No free sequence found, return NULL.
    if(idx == (size_t)-1) 
    {   
        return NULL;
    }
    _set_sequence(idx, nblocks, 1);     // Mark blocks as allocated.
    return (HEAP_START + idx * BLOCK_SIZE);
}

/* free an allocation of a given size (mark free). */
static void _1l_bm_free(void *ptr, size_t size)
{
    size_t nblocks   = BLOCKS_NEEDED(size);
    size_t start_idx = ((uint8_t*)ptr - HEAP_START) / BLOCK_SIZE;
    _set_sequence(start_idx, nblocks, 0); // Mark blocks as free.
}
