#ifndef FORKSKINNYPLUS64_SLICING_ACCELERATED_H
#define FORKSKINNYPLUS64_SLICING_ACCELERATED_H

#include <cstdint>
#include <x86intrin.h>
#include "forkskinny64-datatypes.h"
#include "../../constants.h"
#include "slicing64.h"

/**
 * Performs the back-rotate iteration, in other words:
 * [0 | 0 | 0 | A | B | C] --> [0 | 0 | C | A | B | 0]
 *
 * If the slices are SIMD registers (so more than 64 blocks), this rotation is applied on their individual 64-bit lanes.
 * @param blocks ptr to beginning of an Block64_t array. Its length is assumed to be the value of the 'slice_t'.
 */
static inline void back_rotate(Block64_t *b_blocks){
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
	lane_t slice = ZER;
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
								blocks[i + 448].raw,
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

static inline State64Sliced_t slice_accelerated(Blocks64_t blocks) {
	auto result = State64Sliced_t();
	Slice64_t slices[64];
	
	/* Buffer that holds all blocks AND the 64 spots that we use for the back-rotation */
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
		b_blocks[i].raw = ROL64(blocks.values[i & range_mask].raw, (i & 63));
	
	
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
		
		/* Now re-align the slice by rotating back and put it in the slices buffer
		 * (assign to result in 1 go if SIMD segmentation isn't needed) */
		slices[i].value = ROR_LANES(res, i);
		int appel = 1;
	}
	
	#if AVX512_support
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.segments512[i][j] = _mm512_set_epi64(
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
	#elif AVX2_support
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.segments256[i][j] = _mm256_set_epi64x(
					slices[(i << 4) + j + 12].value,
					slices[(i << 4) + j + 8].value,
					slices[(i << 4) + j + 4].value,
					slices[(i << 4) + j].value
					);
		}
	}
	#else
	for (int i = 0; i < 64; i++) result.raw[i] = slices[i];
	#endif
	
	
	
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param significance the index of the Slice64_t, what 'significance' are we talking about w.r.t. the slice.
 * 					E.g. the very first slice contains the *least* significant bits of 64 states
 */
static inline void unslice_significance_accelerated(const Slice64_t slice, Blocks64_t *blocks, uint8_t significance) {
	// TODO
}

static inline Blocks64_t unslice_accelerated(State64Sliced_t state) {
	Blocks64_t unsliced = Blocks64_t();
	Slice64_t unpacked[64];
	#if AVX512_support
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 8; ++k) {
				unpacked[(i * 16) + j + (k * 4)].value = state.segments256[i][j][k];
			}
		}
	}
	#elif AVX2_support
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				unpacked[(i * 16) + j + (k * 4)].value = state.segments256[i][j][k];
			}
		}
	}
	
	#else
	
	#endif
	for (int i = 0; i < 64; ++i)
		unslice_significance(unpacked[i], &unsliced, i);
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_ACCELERATED_H
