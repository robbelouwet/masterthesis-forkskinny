#ifndef FORKSKINNYPLUS64_SLICING_H
#define FORKSKINNYPLUS64_SLICING_H

#include <cstdint>
#include "forkskinny64-datatypes.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline Slice64_t slice_significance(const Blocks64_t *blocks, uint8_t significance) {
	u64 mask = 1ULL << significance;
	auto slice = Slice64_t();
	
	#if slice_size == 128
	for (uint i = 0; i < 64; ++i)
		slice.chunks[0] |= (blocks->values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 128; ++i)
		slice.chunks[1] |= (blocks->values[i].raw & mask) >> significance << (i - 64);
	
	#elif slice_size == 256
	for (uint i = 0; i < 64; ++i)
		slice.chunks[0] |= (blocks->values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 128; ++i)
		slice.chunks[1] |= (blocks->values[i].raw & mask) >> significance << (i - 64);
	for (uint i = 128; i < 192; ++i)
		slice.chunks[2] |= (blocks->values[i].raw & mask) >> significance << (i - 128);
	for (uint i = 192; i < 256; ++i)
		slice.chunks[3] |= (blocks->values[i].raw & mask) >> significance << (i - 192);
	
	#elif slice_size == 512
	for (uint i = 0; i < 64; ++i)
		slice.chunks[0] |= (blocks->values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 128; ++i)
		slice.chunks[1] |= (blocks->values[i].raw & mask) >> significance << (i - 64);
	for (uint i = 128; i < 192; ++i)
		slice.chunks[2] |= (blocks->values[i].raw & mask) >> significance << (i - 128);
	for (uint i = 192; i < 256; ++i)
		slice.chunks[3] |= (blocks->values[i].raw & mask) >> significance << (i - 192);
	for (uint i = 256; i < 320; ++i)
		slice.chunks[4] |= (blocks->values[i].raw & mask) >> significance << (i - 256);
	for (uint i = 320; i < 384; ++i)
		slice.chunks[5] |= (blocks->values[i].raw & mask) >> significance << (i - 320);
	for (uint i = 384; i < 448; ++i)
		slice.chunks[6] |= (blocks->values[i].raw & mask) >> significance << (i - 384);
	for (uint i = 448; i < 512; ++i)
		slice.chunks[7] |= (blocks->values[i].raw & mask) >> significance << (i - 448);
	
	#else
	for (uint i = 0; i < slice_size; ++i)
		slice.value |= (blocks->values[i].raw & mask) >> significance << i;
	
	#endif
	
	return slice;
}

static inline State64Sliced_t slice_internal(const Blocks64_t *blocks,
											 const bool segment = (AVX512_acceleration || AVX2_acceleration)) {
	State64Sliced_t result = State64Sliced_t();
	for (uint i = 0; i < 64; ++i) {
		result.raw[i] = slice_significance(blocks, i);
	}
	try_segment(result.raw, &result, segment);
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param significance the index of the Slice64_t, what 'significance' are we talking about w.r.t. the slice_internal.
 * 					E.g. the very first slice_internal contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const Slice64_t *slice, Blocks64_t *blocks, uint8_t significance) {
	#if slice_size == 128
	uint8_t chunks[2] = {0, 64};
	
	// loop over every segment, __m128i has 2x 64-bit chunks
	for (int i = 0; i < 2; i++) {
		auto chunk = chunks[i];
		for (int bit_index = chunk; bit_index < chunk + 64; ++bit_index) {
			u64 chunk_mask = 1ULL << (bit_index - chunk);
			blocks->values[bit_index].raw |= ((slice->chunks[i] & chunk_mask) >> (bit_index - chunk)) << significance;
		}
	}
	
	#elif slice_size == 256
	uint8_t chunks[4] = {0, 64, 128, 192};
	
	// loop over every segment, __m256i has 4x 64-bit chunks
	for (int i = 0; i < 4; i++) {
		auto chunk = chunks[i];
		for (int b_number = chunk; b_number < chunk + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunk);
			blocks->values[b_number].raw |= ((slice->chunks[i] & mask) >> (b_number - chunk)) << significance;
		}
	}
	#elif slice_size == 512
	uint16_t chunks[8] = {0, 64, 128, 192, 256, 320, 384, 448};
	
	// loop over every chunk, __m512i has 8x 64-bit chunks
	for (int i = 0; i < 8; i++) {
		auto chunk = chunks[i];
		for (int b_number = chunk; b_number < chunk + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunk);
			blocks->values[b_number].raw |= ((slice->chunks[i] & mask) >> (b_number - chunk)) << significance;
		}
	}
	#else
	for (uint b_number = 0; b_number < slice_size; ++b_number) {
		u64 mask = 1ULL << b_number;
		blocks->values[b_number].raw |= ((slice->value & mask) >> b_number) << significance;
	}
	
	#endif
	
}

static void inline unsegmentbla(State64Sliced_t *state, const bool segmented, Slice64_t *slices) {
	if (segmented) {
		#if AVX512_acceleration
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 8; ++k) {
					unpacked[(i * 16) + j + (k * 4)].value = state.segments256[i][j][k];
				}
			}
		}
		#elif AVX2_acceleration
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					slices[(i * 16) + j + (k * 4)].value = state->segments256[i][j][k];
				}
			}
		}
		#endif
	} else
		for (int i = 0; i < 64; ++i) slices[i] = state->raw[i];
}

static inline void unslice_internal(State64Sliced_t *state, Blocks64_t *result,
										  const bool segmented = false) {
	Slice64_t slices[64];
	unsegmentbla(state, segmented, slices);
	
	for (int i = 0; i < 64; ++i)
		unslice_significance(slices + i, result, i);
}

static inline Blocks64_t unslice_internal(State64Sliced_t *state,
                                          const bool segmented = false) {
	Blocks64_t result = Blocks64_t();
	unslice_internal(state, &result, segmented);
	return result;
}


#endif //FORKSKINNYPLUS_SLICING_H
