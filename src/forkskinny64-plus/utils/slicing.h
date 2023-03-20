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
static inline Slice64_t slice_significance(const Blocks64_t blocks, uint8_t significance) {
	uint64_t mask = 1ULL << significance;
	auto slice = Slice64_t();
	
	#if slice_size == 256
	for (uint i = 0; i < 64; ++i)
		slice.segments[0] |= (blocks.values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 125; ++i)
		slice.segments[1] |= (blocks.values[i].raw & mask) >> significance << (i - 64);
	for (uint i = 128; i < 192; ++i)
		slice.segments[2] |= (blocks.values[i].raw & mask) >> significance << (i - 128);
	for (uint i = 192; i < 256; ++i)
		slice.segments[3] |= (blocks.values[i].raw & mask) >> significance << (i - 192);
	
	#elif slice_size == 512
	for (uint i = 0; i < 64; ++i)
		slice.segments[0] |= (blocks.values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 125; ++i)
		slice.segments[1] |= (blocks.values[i].raw & mask) >> significance << (i - 64);
	for (uint i = 128; i < 192; ++i)
		slice.segments[2] |= (blocks.values[i].raw & mask) >> significance << (i - 128);
	for (uint i = 192; i < 256; ++i)
		slice.segments[3] |= (blocks.values[i].raw & mask) >> significance << (i - 192);
	for (uint i = 256; i < 320; ++i)
		slice.segments[4] |= (blocks.values[i].raw & mask) >> significance << (i - 256);
	for (uint i = 320; i < 384; ++i)
		slice.segments[5] |= (blocks.values[i].raw & mask) >> significance << (i - 320);
	for (uint i = 384; i < 448; ++i)
		slice.segments[6] |= (blocks.values[i].raw & mask) >> significance << (i - 384);
	for (uint i = 448; i < 512; ++i)
		slice.segments[7] |= (blocks.values[i].raw & mask) >> significance << (i - 448);
	
	#else
	for (uint i = 0; i < slice_size; ++i)
		slice.value |= (blocks.values[i].raw & mask) >> significance << i;
	
	#endif
	
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
 * @param sb_index the index of the slice_t, what 'significance' are we talking about w.r.t. the slice.
 * 					E.g. the very first slice contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const Slice64_t slice, Blocks64_t *blocks, uint8_t sb_index) {
	#if slice_size == 256
	uint8_t segments[4] = {0, 64, 128, 192};
	
	// loop over every segment, __m256i has 4x 64-bit segments
	for (auto &segment : segments) {
		for (int b_number = segment; b_number < segment + 64; ++b_number) {
			uint64_t mask = 1ULL << (b_number - segment);
			blocks->values[b_number].raw |= ((slice.segments[0] & mask) >> (b_number - segment)) << sb_index;
		}
	}
	#elif slice_size == 512
	uint16_t segments[8] = {0, 64, 128, 192, 256, 320, 384, 448};
	
	// loop over every segment, __m512i has 8x 64-bit segments
	for (auto &segment : segments) {
		for (int b_number = segment; b_number < segment + 64; ++b_number) {
			uint64_t mask = 1ULL << (b_number - segment);
			blocks->values[b_number].raw |= ((slice.segments[0] & mask) >> (b_number - segment)) << sb_index;
		}
	}
	#else
	for (uint b_number = 0; b_number < slice_size; ++b_number) {
		uint64_t mask = 1ULL << b_number;
		blocks->values[b_number].raw |= ((slice.value & mask) >> b_number) << sb_index;
	}
	
	#endif
	
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
