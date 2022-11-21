#ifndef FORKSKINNYPLUS_UTILS_H
#define FORKSKINNYPLUS_UTILS_H

#include "headers/forkskinny64-cipher.h"

typedef unsigned long ulong;

/**
 * for every nibble in the input x, take the ith bit and pack those all together
 *
 * E.g.:
 *       v    v    v
 * x = 1000 1010 0000
 * i = 0010 -> mask to sample the 2nd LSD
 * output: 010
 *
 * @param x the input to slice
 * @param m the 4-bit(!) mask that masks the significant bit of all the cells to be packed. Flagging more than 1 bit is undefined.
 * @return
 */
static inline uint64_t slice_index(uint64_t x, uint64_t i) {
	uint64_t m = 1 << i; // the mask for the bit, on nibble level (so a 4-bit mask that will be shifted)
	uint64_t res = ((x & (m << 60)) >> 45 >> i) |
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
	
	return res;
}

/// http://programming.sirrida.de/calcperm.php
/// input: 0 8 16 24 1 9 17 25 2 10 18 26 3 11 19 27 4 12 20 28 5 13 21 29 6 14 22 30 7 15 23 31
static inline uint64_t bit_permute_step(uint64_t x, uint64_t m, uint64_t shift) {
	uint64_t t;
	t = ((x >> shift) ^ x) & m;
	x = (x ^ t) ^ (t << shift);
	return x;
}

static inline State64Sliced_t slice(State64_t state) {
	auto sliced = State64Sliced_t();
	
	slice_index(0xffffffffffffffff, 1);
	
	sliced.state.internal_state =
			slice_index(state.llrow, 0)
			| (slice_index(state.llrow, 1) << 16)
			| (slice_index(state.llrow, 2) << 32)
			| (slice_index(state.llrow, 3) << 48);
	
	sliced.slice0 = &(sliced.state.slices[0]);
	sliced.slice1 = &(sliced.state.slices[1]);
	sliced.slice2 = &(sliced.state.slices[2]);
	sliced.slice3 = &(sliced.state.slices[3]);
	
	return sliced;
}

static inline State64_t unslice(State64Sliced_t state) {

}

#endif //FORKSKINNYPLUS_UTILS_H
