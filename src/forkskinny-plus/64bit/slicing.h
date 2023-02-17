#ifndef FORKSKINNYPLUS_SLICING_H
#define FORKSKINNYPLUS_SLICING_H

#include <cstdint>
#include "forkskinny64-plus.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline uint64_t slice_significance(const Blocks64 blocks, uint8_t significance) {
	uint64_t mask = 1ULL << significance;
	uint64_t slice = 0ULL;
	
	for (uint i = 0; i < 64; ++i)
		slice |= (blocks.values[i] & mask) >> significance << i;
	
	return slice;
}

static inline State64Sliced_t slice(const Blocks64 blocks) {
	State64Sliced_t result = State64Sliced_t();
	for (uint i = 0; i < 64; ++i) {
		result.raw[i] = slice_significance(blocks, i);
	}
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param sb_index the index of the slice, what 'significance' are we talking about w.r.t. the slice.
 * 					E.g. the very first slice contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const uint64_t slice, Blocks64 *blocks, uint8_t sb_index) {
	for (uint b_number = 0; b_number < 64; ++b_number) {
		uint64_t mask = 1ULL << b_number;
		
		uint64_t temp = slice & mask;
		temp >>= b_number;
		temp <<= sb_index;
		blocks->values[b_number] |= temp;
		//blocks->values[b_number] |= (slice & mask) >> b_number << sb_index;
	}
}

static inline Blocks64 unslice(const State64Sliced_t state) {
	Blocks64 unsliced = Blocks64();
	for (int i = 0; i < 64; ++i) {
		unslice_significance(state.raw[i], &unsliced, i);
	}
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_H
