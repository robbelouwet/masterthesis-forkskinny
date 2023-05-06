#ifndef FORKSKINNYPLUS128_SLICING_H
#define FORKSKINNYPLUS128_SLICING_H

#include <cstdint>
#include "forkskinny128-datatypes.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline Slice128_t slice_significance(const Blocks128_t blocks, uint8_t significance, bool low) {
	u64 mask = 1ULL << significance;
	auto slice = Slice128_t();
	
	#if slice_size == 128
	for (uint i = 0; i < 64; ++i) {
		if (low) slice.chunks[0] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[0] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 64; i < 128; ++i){
		if (low) slice.chunks[1] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[1] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	#elif slice_size == 256
	for (uint i = 0; i < 64; ++i) {
		if (low) slice.chunks[0] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[0] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 64; i < 128; ++i){
		if (low) slice.chunks[1] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[1] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 128; i < 192; ++i){
		if (low) slice.chunks[2] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[2] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 192; i < 256; ++i){
		if (low) slice.chunks[3] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[3] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	
	#elif slice_size == 512
	for (uint i = 0; i < 64; ++i) {
		if (low) slice.chunks[0] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[0] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 64; i < 128; ++i) {
		if (low) slice.chunks[1] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[1] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 128; i < 192; ++i) {
		if (low) slice.chunks[2] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[2] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 192; i < 256; ++i) {
		if (low) slice.chunks[3] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[3] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 256; i < 320; ++i) {
		if (low) slice.chunks[4] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[4] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 320; i < 384; ++i) {
		if (low) slice.chunks[5] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[5] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 384; i < 448; ++i) {
		if (low) slice.chunks[6] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[6] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 448; i < 512; ++i) {
		if (low) slice.chunks[7] |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.chunks[7] |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	
	#else
	for (uint i = 0; i < slice_size; ++i) {
		if (low) slice.value |= (blocks.values[i].raw[0] & mask) >> significance << i;
		else slice.value |= (blocks.values[i].raw[1] & mask) >> significance << i;
	}
	
	#endif
	
	return slice;
}

static inline State128Sliced_t slice(const Blocks128_t blocks) {
	State128Sliced_t result = State128Sliced_t();
	for (uint i = 0; i < 128; ++i) {
		result.raw[i] = slice_significance(blocks, i ^ 64, i < 64);
	}
	
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param sb_index the index of the slice_t, what 'significance' are we talking about w.r.t. the slice.
 * 					E.g. the very first slice contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const Slice128_t slice, Blocks128_t *blocks, uint8_t sb_index, bool low) {
	#if slice_size == 128
	uint8_t chunks[4] = {0, 64};
	
	// loop over every segment, __m128i has 2x 64-bit chunks
	for (auto &segment : chunks) {
		for (int b_number = segment; b_number < segment + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - segment);
			if (low) blocks->values[b_number].raw[0] |= ((slice.chunks[0] & mask) >> (b_number - segment)) << sb_index;
			else blocks->values[b_number].raw[1] |= ((slice.chunks[0] & mask) >> (b_number - segment)) << sb_index;
		}
	}
	
	#elif slice_size == 256
	uint8_t chunks[4] = {0, 64, 128, 192};
	
	// loop over every segment, __m256i has 4x 64-bit chunks
	for (auto &segment : chunks) {
		for (int b_number = segment; b_number < segment + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - segment);
			if (low) blocks->values[b_number].raw[0] |= ((slice.chunks[0] & mask) >> (b_number - segment)) << sb_index;
			else blocks->values[b_number].raw[1] |= ((slice.chunks[0] & mask) >> (b_number - segment)) << sb_index;
		}
	}
	#elif slice_size == 512
	uint16_t chunks[8] = {0, 64, 128, 192, 256, 320, 384, 448};
	
	// loop over every segment, __m512i has 8x 64-bit chunks
	for (auto &segment: chunks) {
		for (int b_number = segment; b_number < segment + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - segment);
			if (low) blocks->values[b_number].raw[0] |= ((slice.chunks[0] & mask) >> (b_number - segment)) << sb_index;
			else blocks->values[b_number].raw[1] |= ((slice.chunks[0] & mask) >> (b_number - segment)) << sb_index;
		}
	}
	#else
	for (uint b_number = 0; b_number < slice_size; ++b_number) {
		u64 mask = 1ULL << b_number;
		if (low) blocks->values[b_number].raw[0] |= ((slice.value & mask) >> b_number) << sb_index;
		else blocks->values[b_number].raw[1] |= ((slice.value & mask) >> b_number) << sb_index;
	}
	
	#endif
	
}

static inline Blocks128_t unslice_accelerated(State128Sliced_t state) {
	
	Blocks128_t unsliced = Blocks128_t();
	for (int i = 0; i < 128; ++i) {
		unslice_significance(state.raw[i], &unsliced, i ^ 64, i < 64);
	}
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_H
