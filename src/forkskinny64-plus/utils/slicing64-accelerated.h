#ifndef FORKSKINNYPLUS64_SLICING_ACCELERATED_H
#define FORKSKINNYPLUS64_SLICING_ACCELERATED_H

#include <cstdint>
#include <x86intrin.h>
#include "forkskinny64-datatypes.h"
#include "../../constants.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline Slice64_t slice_significance_accelerated_64(const Block64_t *blocks) {
	Slice64_t slice = Slice64_t();
	#if slice_size == 128
	slice.value = _mm_setzero_si128();
	for (uint i = 0; i < 64; ++i)
		slice.value = _mm_or_si128(
				_mm_and_si128(
						_mm_set_epi64x(
								blocks[i + 64].raw,
								blocks[i].raw
						), _mm_set1_epi64x(bit_masks[i])
				), slice.value
		);
	
	#elif slice_size == 256
	slice.value = _mm256_setzero_si256();
	for (uint i = 0; i < 64; ++i)
		slice.value = _mm256_or_si256(
				_mm256_and_si256(
						_mm256_set_epi64x(
								blocks[i + 192].raw,
								blocks[i + 128].raw,
								blocks[i + 64].raw,
								blocks[i].raw
						), _mm256_set1_epi64x(bit_masks[i])
				), slice.value
		);
	
	#elif slice_size == 512
	slice.value = _mm512_setzero_si512();
	for (uint i = 0; i < 64; ++i)
		slice.value = _mm512_or_si512(
				_mm512_and_si512(
						_mm512_set_epi64(
								blocks[i + 448].raw,
								blocks[i + 384].raw,
								blocks[i + 320].raw,
								blocks[i + 256].raw,
								blocks[i + 192].raw,
								blocks[i + 128].raw,
								blocks[i + 64].raw,
								blocks[i].raw
						), _mm512_set1_epi64(bit_masks[i])
				), slice.value
		);
	
	#elif slice_size == 64
	for (int i = 0; i < 64; ++i) {
		auto val = blocks[i].raw & bit_masks[i];
		slice.value |= val;
	}
	#endif
	
	return slice;
}

static inline Slice64_t slice_significance_accelerated_small(const Block64_t *blocks, uint16_t mask_index){
	auto slice = Slice64_t();
	auto bound = slice_size - mask_index;
	int i = 0;
	for (; i < bound; ++i) slice.value |= blocks[i].raw & bit_masks[mask_index];
	mask_index &= 63;
	for (; i < slice_size; ++i) slice.value |= blocks[i].raw & bit_masks[mask_index];
}

/**
 *
 * @param blocks
 * @param amount
 * @return
 */
static inline void slice_accelerated_64(Blocks64_t blocks, Slice64_t *out) {
	
	/* Buffer that holds all blocks AND the 64 blocks that we iterate to the front */
	/* this push-and-rotate technique doesn't work if slice_size < block size */
	#if slice_size < 64
	auto len = 128;
	Block64_t b_blocks[128] = {};
	#else
	auto len = 64 + slice_size;
	Block64_t b_blocks[len];
	#endif
	
	auto range_mask = slice_size - 1;
	
	/* rotate-align the blocks & copy into extended buffer at the upper half of the buffer */
	auto stop = slice_size < 64 ? 64 + slice_size : len;
	for (int i = 64; i < stop; i++) {
		b_blocks[i].raw =  ROL64(blocks.values[i & range_mask].raw, (i & range_mask));
	}
	
	/* iterate over every 'significance' level the pt blocks have, which are 64 for forkskinny64 */
	/* when iterating, slowly rotate the blocks back towards the beginning of the buffer on every iteration,
	 * aligning the next significance every time */
	for (int i = 0; i < 64; i++) {
		/* The b_blocks iterator */
		auto ind = 64 - i;
		
		/* Construct the slice for this significance level */
		auto res = slice_significance_accelerated_64(b_blocks + ind).value;
		
		/* Move the last block in the buffer to the front*/
		b_blocks[ind - 1] = b_blocks[len - 1 - i];
		
		/* Now re-align the slice by rotating back and put it in the slices buffer */
		out[i].value = ROR(res, i); // rotate slices back into place
		int appel = 1;
	}
}

static inline void slice_accelerated_small(Blocks64_t blocks, Slice64_t *out){
	
	/* Buffer that holds all blocks AND the 64 blocks that we iterate to the front */
	/* this push-and-rotate technique doesn't work if slice_size < block size */
	auto len = 64 + slice_size;
	Block64_t b_blocks[len];
	
	auto range_mask = slice_size - 1;
	
	/* rotate-align the blocks & copy into extended buffer at the upper half of the buffer */
	auto stop = slice_size < 64 ? 64 + slice_size : len;
	for (int i = 64; i < stop; i++) {
		b_blocks[i].raw =  ROL64(blocks.values[i & range_mask].raw, (i & range_mask));
	}
	
	/* iterate over every 'significance' level the pt blocks have, which are 64 for forkskinny64 */
	/* when iterating, slowly rotate the blocks back towards the beginning of the buffer on every iteration,
	 * aligning the next significance every time */
	for (int i = 0; i < 64; i++) {
		/* The b_blocks iterator */
		auto ind = 64 - i;
		
		/* Construct the slice for this significance level */
		auto res = slice_significance_accelerated_small(b_blocks + 64, i).value;
		
		/* Move the last block in the buffer to the front*/
		b_blocks[ind - 1] = b_blocks[len - 1 - i];
		
		/* Now re-align the slice by rotating back and put it in the slices buffer */
		out[i].value = ROR(res, i); // rotate slices back into place
		int appel = 1;
	}
}

static inline State64Sliced_t slice_accelerated(Blocks64_t blocks){
	auto result = State64Sliced_t();
	Slice64_t slices[64];
	#if slice_size >=64
	slice_accelerated_64(blocks, slices);
	#else
	slice_accelerated_small(blocks, slices);
	#endif
	
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
	for (int i = 0; i < 64; i++)
		result.raw[i].value = slices[i].value;
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

	
}

static inline Blocks64_t unslice_accelerated(State64Sliced_t state) {

}

#endif //FORKSKINNYPLUS_SLICING_ACCELERATED_H
