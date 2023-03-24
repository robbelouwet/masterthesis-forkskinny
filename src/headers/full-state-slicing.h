#ifndef FORKSKINNYPLUS64_FULL_STATE_SLICING_H
#define FORKSKINNYPLUS_FULL_STATE_SLICING_H

#include <cstdint>
#include <cassert>

/**
 * For every nibble, take the ith significant bit and pack those all together.
 *
 * E.g.:
 *       v    v    v
 * x = 1000 1010 0000
 * i = 1 = slice_t the 2nd LSB of every nibble (0-based index)
 * output: 010
 *
 * @param x: the input to slice_t
 * @param m: 0 <= m <= 3; the 0-based index that masks the relevant significant bit of every slices.
 * @return the input in a bit sliced_fghi manner
 */
static inline uint64_t slice_index(uint64_t x, uint64_t i) {
	// because uint4_t doesn't exist
	assert(i <= 3);
	
	// the mask for the bit, on nibble level (so a 4-bit mask that will be shifted)
	uint64_t m = 1 << i;
	
	return ((x & (m << 60)) >> 45 >> i) |
	       ((x & (m << 56)) >> 42 >> i) |
	       ((x & (m << 52)) >> 39 >> i) |
	       ((x & (m << 48)) >> 36 >> i) |
	       ((x & (m << 44)) >> 33 >> i) |
	       ((x & (m << 40)) >> 30 >> i) |
	       ((x & (m << 36)) >> 27 >> i) |
	       ((x & (m << 32)) >> 24 >> i) |
	       ((x & (m << 28)) >> 21 >> i) |
	       ((x & (m << 24)) >> 18 >> i) |
	       ((x & (m << 20)) >> 15 >> i) |
	       ((x & (m << 16)) >> 12 >> i) |
	       ((x & (m << 12)) >> 9 >> i) |
	       ((x & (m << 8)) >> 6 >> i) |
	       ((x & (m << 4)) >> 3 >> i) |
	       ((x & m) >> i);
}

/**
 * Place all the equally significant bits packed in *raw*, back in their nibble on their correct significant position.
 * I.e., every bit of *raw* gets projected to a nibble. More specifically it gets projected to the
 * ith significant bit of each nibble.
 *
 * e.g.:
 * 	raw: 0000 0000 0000 1111
 * 	i: 1 (0-based index)
 *
 * 	output: 0000 (0000)¹¹ 0010 0010 0010 0010
 * 		-> the 4 ones of *raw* each get projected to the second-LSD in a separate nibble
 *
 * @param value: the one of the 4 packed input rows of 16 bits in bit sliced_fghi representation
 * @param i: 0-based index to identify to which significant bit of every nibble needs to be projected.
 * @return
 */
static inline uint64_t unslice_index(uint16_t value, uint64_t i) {
	// because uint4_t doesn't exist
	assert(i <= 3);
	
	uint64_t x = value;
	
	return ((x & 0x1) << i)
	       | ((x & 0x2) << i << 3)
	       | ((x & 0x4) << i << 6)
	       | ((x & 0x8) << i << 9)
	       | ((x & 0x10) << i << 12)
	       | ((x & 0x20) << i << 15)
	       | ((x & 0x40) << i << 18)
	       | ((x & 0x80) << i << 21)
	       | ((x & 0x100) << i << 24)
	       | ((x & 0x200) << i << 27)
	       | ((x & 0x400) << i << 30)
	       | ((x & 0x800) << i << 33)
	       | ((x & 0x1000) << i << 36)
	       | ((x & 0x2000) << i << 39)
	       | ((x & 0x4000) << i << 42)
	       | ((x & 0x8000) << i << 45);
}

static inline uint64_t slice(uint64_t state) {
	return
			slice_index(state, 0)
			| (slice_index(state, 1) << 16)
			| (slice_index(state, 2) << 32)
			| (slice_index(state, 3) << 48);
}

static inline uint64_t unslice(uint64_t state) {
	auto s0 = unslice_index((state & 0x000000000000FFFF), 0);
	auto s1 = unslice_index(((state & 0x00000000FFFF0000) >> 16), 1);
	auto s2 = unslice_index(((state & 0x0000FFFF00000000) >> 32), 2);
	auto s3 = unslice_index(((state & 0xFFFF000000000000) >> 48), 3);
	
	return s0 | s1 | s2 | s3;
}

#endif //FORKSKINNYPLUS64_FULL_STATE_SLICING_H
