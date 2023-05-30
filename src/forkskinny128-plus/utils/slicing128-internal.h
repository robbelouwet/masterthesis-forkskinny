#ifndef FORKSKINNYPLUS128_SLICING_H
#define FORKSKINNYPLUS128_SLICING_H

#include <cstdint>
#include "forkskinny128-datatypes.h"

static inline void slice_significance(const Blocks128_t *blocks, Slice128_t *slice,
                                            uint8_t significance, bool low) {
	u64 mask = 1ULL << significance;
	
	#if slice_size == 128
	for (uint i = 0; i < 64; ++i) {
		if (low) slice->chunks[0] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[0] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 64; i < 128; ++i){
		if (low) slice->chunks[1] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[1] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	#elif slice_size == 256
	for (uint i = 0; i < 64; ++i) {
		if (low) slice->chunks[0] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[0] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 64; i < 128; ++i){
		if (low) slice->chunks[1] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[1] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 128; i < 192; ++i){
		if (low) slice->chunks[2] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[2] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 192; i < 256; ++i){
		if (low) slice->chunks[3] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[3] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	
	#elif slice_size == 512
	for (uint i = 0; i < 64; ++i) {
		if (low) slice->chunks[0] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[0] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 64; i < 128; ++i) {
		if (low) slice->chunks[1] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[1] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 128; i < 192; ++i) {
		if (low) slice->chunks[2] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[2] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 192; i < 256; ++i) {
		if (low) slice->chunks[3] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[3] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 256; i < 320; ++i) {
		if (low) slice->chunks[4] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[4] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 320; i < 384; ++i) {
		if (low) slice->chunks[5] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[5] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 384; i < 448; ++i) {
		if (low) slice->chunks[6] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[6] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	for (uint i = 448; i < 512; ++i) {
		if (low) slice->chunks[7] |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->chunks[7] |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	
	#else
	for (uint i = 0; i < slice_size; ++i) {
		if (low) slice->value |= (blocks->values[i].raw[0] & mask) >> significance << i;
		else slice->value |= (blocks->values[i].raw[1] & mask) >> significance << i;
	}
	#endif
}

static inline void slice_internal(const Blocks128_t *blocks, State128Sliced_t *res, const bool segment = AVX2_acceleration) {
	State128Sliced_t unsegmented = {};
	for (uint i = 0; i < 128; ++i) {
		slice_significance(blocks, &(unsegmented.raw[i]), i & 63, i < 64);
	}
	
	try_segment128(&unsegmented, res, segment);
}

static inline State128Sliced_t slice_internal(const Blocks128_t *blocks, const bool segment = AVX2_acceleration) {
	State128Sliced_t res;
	slice_internal(blocks, &res, segment);
	return res;
}

/**
 *
 * @param slice
 * @param blocks
 * @param sb_index the index of the slice_t, what 'significance' are we talking about w.r.t. the slice_internal.
 * 					E.g. the very first slice_internal contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const Slice128_t *slice, Blocks128_t *blocks, uint8_t sb_index, bool low) {
	#if slice_size == 128
	uint8_t chunks[4] = {0, 64};
	
	// loop over every segment, __m128i has 2x 64-bit chunks
	for (int i = 0; i < 2; i++) {
		for (int b_number = chunks[i]; b_number < chunks[i] + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunks[i]);
			if (low) blocks->values[b_number].raw[0] |= ((slice->chunks[i] & mask) >> (b_number - chunks[i])) << sb_index;
			else blocks->values[b_number].raw[1] |= ((slice->chunks[i] & mask) >> (b_number - chunks[i])) << sb_index;
		}
	}
	#elif slice_size == 256
	uint8_t chunks[4] = {0, 64, 128, 192};
	
	// loop over every segment, __m256i has 4x 64-bit chunks
	for (int i = 0; i < 4; i++) {
		for (int b_number = chunks[i]; b_number < chunks[i] + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunks[i]);
			if (low) blocks->values[b_number].raw[0] |= ((slice->chunks[i] & mask) >> (b_number - chunks[i])) << sb_index;
			else blocks->values[b_number].raw[1] |= ((slice->chunks[i] & mask) >> (b_number - chunks[i])) << sb_index;
		}
	}
	#elif slice_size == 512
	uint16_t chunks[8] = {0, 64, 128, 192, 256, 320, 384, 448};
	
	// loop over every segment, __m512i has 8x 64-bit chunks
	for (int i = 0; i < 8; i++) {
		for (int b_number = chunks[i]; b_number < chunks[i] + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunks[i]);
			if (low) blocks->values[b_number].raw[0] |= ((slice->chunks[i] & mask) >> (b_number - chunks[i])) << sb_index;
			else blocks->values[b_number].raw[1] |= ((slice->chunks[i] & mask) >> (b_number - chunks[i])) << sb_index;
		}
	}
	#else
	for (uint b_number = 0; b_number < slice_size; ++b_number) {
		u64 mask = 1ULL << b_number;
		if (low) blocks->values[b_number].raw[0] |= ((slice->value & mask) >> b_number) << sb_index;
		else blocks->values[b_number].raw[1] |= ((slice->value & mask) >> b_number) << sb_index;
	}
	#endif
}

static inline void unslice_internal(State128Sliced_t *state, Blocks128_t *result,
                                    const bool segmented = AVX2_acceleration) {
	State128Sliced_t unsegmented;
	try_unsegment128(state, &unsegmented, segmented);
	
	for (int i = 0; i < 128; ++i)
		unslice_significance(&(unsegmented.raw[i]), result, i & 63, i < 64);
}

static inline Blocks128_t unslice_internal(State128Sliced_t *state,
                                           const bool segmented = AVX2_acceleration) {
	Blocks128_t result = Blocks128_t();
	unslice_internal(state, &result, segmented);
	return result;
}

#endif //FORKSKINNYPLUS_SLICING_H
