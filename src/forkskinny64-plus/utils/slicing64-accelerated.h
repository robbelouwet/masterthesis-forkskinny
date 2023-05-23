#ifndef FORKSKINNYPLUS64_SLICING_ACCELERATED_H
#define FORKSKINNYPLUS64_SLICING_ACCELERATED_H

#include <cstdint>
#include <x86intrin.h>
#include "forkskinny64-datatypes.h"
#include "../../constants.h"

static void inline try_segment(Slice64_t *slices, State64Sliced_t *result, const bool segment){
	if (segment) {
		#if AVAVX2_acceleration
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				result->segments512[i][j] = _mm512_set_epi64(
						slices[(i << 5) + j + 28].value,
						slices[(i << 5) + j + 24].value,
						slices[(i << 5) + j + 20].value,
						slices[(i << 5) + j + 16].value,
						slices[(i << 5) + j + 12].value,
						slices[(i << 5) + j + 8].value,
						slices[(i << 5) + j + 4].value,
						slices[(i << 5) + j].value
						);
			}
		}
		#elif AVX2_acceleration
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result->segments256[i][j] = _mm256_set_epi64x(
						slices[(i << 4) + j + 12].value,
						slices[(i << 4) + j + 8].value,
						slices[(i << 4) + j + 4].value,
						slices[(i << 4) + j].value
				);
			}
		}
		#endif
	} else
		for (int i = 0; i < 64; i++)
			result->raw[i].value = slices[i].value;
}

/**
 * Performs the back-rotate iteration, in other words:
 * [0 | 0 | 0 | A | B | C] --> [0 | 0 | C | A | B | 0]
 *
 * If the slices are SIMD registers (so more than 64 blocks), this rotation is applied on their individual 64-bit lanes.
 * @param blocks ptr to beginning of an Block64_t array. Its length is assumed to be the value of the 'slice_t'.
 */
static inline void back_rotate(Block64_t *b_blocks) {
	#if slice_size > 64
	auto iterations = (slice_size >> 6);
	for (int i = 0; i < iterations; ++i)
		*(b_blocks - 1 + (64 * i)) = *(b_blocks - 1 + (64 * (i+1)));
	#else
	*(b_blocks - 1) = *(b_blocks + 63);
	#endif
}

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline lane_t slice_significance_accelerated_64(const Block64_t *blocks) {
	lane_t slice = slice_ZER;
	#if slice_size == 128
	for (uint i = 0; i < 64; ++i)
		slice = _mm_or_si128(
				_mm_and_si128(
						_mm_set_epi64x(
								blocks[i].raw,
								blocks[i + 64].raw
								
						), _mm_set1_epi64x(bit_masks[i])
				), slice
		);
	
	#elif slice_size == 256
	for (uint i = 0; i < 64; ++i)
		slice = _mm256_or_si256(
				_mm256_and_si256(
						_mm256_set_epi64x(
								blocks[i + 128].raw,
								blocks[i + 64].raw,
								blocks[i].raw,
								blocks[i + 192].raw
								
						), _mm256_set1_epi64x(bit_masks[i])
				), slice
		);
	
	#elif slice_size == 512
	for (uint i = 0; i < 64; ++i)
		slice = _mm512_or_si512(
				_mm512_and_si512(
						_mm512_set_epi64(
								blocks[i].raw,
								blocks[i + 64].raw,
								blocks[i + 128].raw,
								blocks[i + 192].raw,
								blocks[i + 256].raw,
								blocks[i + 320].raw,
								blocks[i + 384].raw,
								blocks[i + 448].raw
						), _mm512_set1_epi64(bit_masks[i])
				), slice
		);
	
	#else
	for (int i = 0; i < 64; ++i) {
		auto val = blocks[i].raw & bit_masks[i];
		slice |= val;
	}
	#endif
	
	return slice;
}

/**
 *
 * @param blocks
 * @param amount
 * @return
 */

static inline void slice_accelerated(Blocks64_t *blocks,
                                     State64Sliced_t *result,
                                     bool const segment = (AVX512_acceleration || AVX2_acceleration)) {
	Slice64_t slices[64];
	
	/* Buffer that holds all blocks (minimum 64) AND the 64 spots that we use for the back-rotation */
	#if slice_size >= 64
	auto len = 64 + slice_size;
	Block64_t b_blocks[len];
	#else
	auto len = 128;
	Block64_t b_blocks[128] = {};
	#endif
	
	auto range_mask = slice_size - 1;
	
	/* rotate-align the blocks & copy into extended buffer at the upper half of the buffer */
	auto stop = slice_size < 64 ? 64 + slice_size : len;
	for (int i = 64; i < stop; i++)
		b_blocks[i].raw = ROL64(blocks->values[i & range_mask].raw, (i & 63));
	
	
	/* Iterate over every 'significance' level the pt blocks have, which are 64 for forkskinny64 */
	/* When iterating, rotate 1 block from the back towards the front on every iteration,
	 * aligning the next significance every time */
	for (int i = 0; i < 64; i++) {
		/* The relative pointer pointing to the beginning of the queue */
		auto ind = 64 - i;
		
		/* Construct the slice for this significance level */
		auto res = slice_significance_accelerated_64(b_blocks + ind);
		
		/* back-rotate the last block in the buffer to the front */
		back_rotate(b_blocks + ind);
		
		/* Now re-align the slice by rotating back and put it in the slices buffer */
		slices[i].value = ROR_LANES(res, i);
		int appel = 1;
	}
	
	try_segment(slices, result, segment);
	
	int appel = 1;
}

static inline State64Sliced_t slice_accelerated(Blocks64_t *blocks,
                                                bool const segment = (AVX512_acceleration || AVX2_acceleration)) {
	State64Sliced_t res;
	slice_accelerated(blocks, &res, segment);
	return res;
}

static void inline unsegment(State64Sliced_t *state, const bool segmented, lane_t *slices) {
	if (segmented) {
		#if AVX512_acceleration
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 8; ++k) {
					unpacked[(i * 16) + j + (k * 4)] = state.segments256[i][j][k];
				}
			}
		}
		#elif AVX2_acceleration
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					slices[(i * 16) + j + (k * 4)] = state->segments256[i][j][k];
				}
			}
		}
		#endif
	} else
		for (int i = 0; i < 64; ++i) slices[i] = state->raw[i].value;
}

/**
 *
 * @param slice
 * @param blocks
 * @param significance the index of the Slice64_t, what 'significance' are we talking about w.r.t. the slice.
 * 					E.g. the very first slice contains the *least* significant bits of 64 states
 */
static inline lane_t unslice_significance_accelerated(lane_t *slices) {
	lane_t block = slice_ZER;
	
	#if slice_size == 512
	for (int i = 0; i < 64; ++i) {
		auto val = AND512(slices[i], _mm512_set1_epi64(bit_masks[i]));
		block = OR512(block, val);
	}
	#elif slice_size == 256
	for (int i = 0; i < 64; ++i) {
		auto val = AND256(slices[i], _mm256_set1_epi64x(bit_masks[i]));
		block = OR256(block, val);
	}
	#elif slice_size == 128
	for (int i = 0; i < 64; ++i) {
		auto val = AND128(slices[i], _mm_set1_epi64x(bit_masks[i]));
		block = OR128(block, val);
	}
	#else
	for (int i = 0; i < 64; ++i) {
		lane_t val = slices[i] & bit_masks[i];
		block |= val;
	}
	#endif

	return block;
}

static inline void unslice_accelerated(State64Sliced_t *state,
                                       Blocks64_t *result,
                                       bool const segmented = (AVX2_acceleration || AVX512_acceleration)) {
	lane_t slices[128] = {};
	unsegment(state, segmented, slices + 64);
	

	for (int i = 64; i < 128; ++i)
		slices[i] = ROL_LANES((lane_t(slices[i])), (i & 63));
	
	auto stop = slice_size < 64 ? 64 + slice_size : 128;
	for (int i = 64; i < stop; ++i) {
		auto ind = i - 64;
		auto rotated_block = unslice_significance_accelerated(slices + 64 - ind);
		slices[63 - ind] = slices[127 - ind];
		
		auto packed_blocks = ROR_LANES(rotated_block, ind);
		
		// after unslicing and rotating back, the slice contains the value of the block
		// or multiple blocks are contained in the lanes if a slice is a SIMD variable
		#if slice_size > 64
		for (int j = 0; j < (slice_size >> 6); ++j)
			result->values[j * 64 + ind].raw = packed_blocks[j];
		#else
		result->values[ind].raw = packed_blocks;
		#endif
		int appel = 1;
	}
}

static inline Blocks64_t unslice_accelerated(State64Sliced_t state,
                                             bool const segmented = (AVX2_acceleration || AVX512_acceleration)) {
	Blocks64_t res = Blocks64_t();
	unslice_accelerated(&state, &res, segmented);
	return res;
}

#endif //FORKSKINNYPLUS_SLICING_ACCELERATED_H
