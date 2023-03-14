#ifndef FORKSKINNYPLUS_SLICING_H
#define FORKSKINNYPLUS_SLICING_H

#include <cstdint>
#include "skinny64_datatypes.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline slice_t slice_significance(const Blocks64_t blocks, uint8_t significance) {
	uint64_t mask = 1ULL << significance;
	uint64_t slice = 0ULL;
	
	for (uint i = 0; i < slice_size; ++i)
		slice |= (blocks.values[i].raw & mask) >> significance << (i);
	
	return slice;
}

static inline State64Sliced_t slice(const Blocks64_t blocks, bool reverse = true) {
	State64Sliced_t result = State64Sliced_t();
	for (uint i = 0; i < 64; ++i) {
		result.raw[i] = slice_significance(blocks, i);
	}
	
	
	// reverse the cells (!= 'just' reversing the raw slices array, their order within a cell needs to be maintained!)
//	if (reverse)
//		for (int i = 0; i < 8; ++i) {
//			auto temp = result.cells[15 - i];
//			result.cells[15 - i] = result.cells[i];
//			result.cells[i] = temp;
//		}
	
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param sb_index the index of the slice_t, what 'significance' are we talking about w.r.t. the slice_t.
 * 					E.g. the very first slice_t contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const uint64_t slice, Blocks64_t *blocks, uint8_t sb_index) {
	for (uint b_number = 0; b_number < slice_size; ++b_number) {
		uint64_t mask = 1ULL << b_number;
		
		uint64_t temp = slice & mask;
		temp >>= b_number;
		temp <<= sb_index;
		blocks->values[b_number].raw |= temp;
		//blocks->values[b_number] |= (slice_t & mask) >> b_number << sb_index;
	}
}

static inline Blocks64_t unslice(State64Sliced_t state, bool reverse_back = true) {
	// reverse the cells (!= 'just' reversing the raw slices array, their order within a cell needs to be maintained!)
//	if (reverse_back)
//		for (int i = 0; i < 8; ++i) {
//			auto temp = state.cells[15 - i];
//			state.cells[15 - i] = state.cells[i];
//			state.cells[i] = temp;
//		}
	
	Blocks64_t unsliced = Blocks64_t();
	for (int i = 0; i < 64; ++i) {
		unslice_significance(state.raw[i], &unsliced, i);
	}
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_H
