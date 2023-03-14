#ifndef FORKSKINNYPLUS_SLICING_H
#define FORKSKINNYPLUS_SLICING_H

#include <cstdint>
#include "forkskinny128-plus.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline uint64_t slice_significance(const Blocks128 pt_block, uint8_t significance) {
	uint64_t mask = 1ULL << (significance % 64);
	uint64_t slice = 0ULL;
	
	// sig<64 => half = 0 = first half
	// sig>=64 => half = 1 = second half
	uint8_t half = significance >= 64;
	for (uint slice_index = 0; slice_index < 64; ++slice_index)
		slice |= (pt_block.values[slice_index][half] & mask) >> significance % 64 << slice_index;
	
	return slice;
}

static inline State128Sliced_t slice(const Blocks128 pt_blocks) {
	State128Sliced_t result = State128Sliced_t();
	for (uint i = 0; i < 128; ++i) {
		result.raw[i] = slice_significance(pt_blocks, i);
	}
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param significance the index of the slice_t, what 'significance' are we talking about w.r.t. the slice_t.
 * 					E.g. the very first slice_t contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const uint64_t slice, Blocks128 *blocks, uint8_t significance) {
	for (uint block_i = 0; block_i < 64; ++block_i) {
		
		// the index of a bit within the slice_t refers to a specific round state / block
		uint64_t mask = 1ULL << (block_i % 64);
		
		blocks->values[block_i][significance >= 64] |= (slice & mask) >> block_i << (significance % 64);
	}
}

static inline Blocks128 unslice(const State128Sliced_t state) {
	Blocks128 unsliced = Blocks128();
	for (int i = 0; i < 128; ++i)
		unslice_significance(state.raw[i], &unsliced, i);
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_H
