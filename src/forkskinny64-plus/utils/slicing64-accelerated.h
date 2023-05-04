#ifndef FORKSKINNYPLUS64_SLICING_ACCELERATED_H
#define FORKSKINNYPLUS64_SLICING_ACCELERATED_H

#include <cstdint>
#include "forkskinny64-datatypes.h"
#include "../../constants.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline Slice64_t slice_significance_accelerated(const Block64_t *blocks) {
	auto slice = Slice64_t();
	
	#if slice_size == 128
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
	
		#else
	for (uint i = 0; i < slice_size; ++i)
		slice.value |= blocks[i].raw & bit_masks[i];
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
	
	/* we iterate & re-queue 64 times because we have 64 bits per block.
	 * If there are less blocks, make sure we still have room */
	auto len = slice_size <= 64 ? 64 : slice_size;
	Block64_t b_blocks[len << 1]; // double-length blocks buffer
	Slice64_t slices[64];
	auto range_mask = slice_size - 1;
	
	///////////////// IETS MET DE BUFFER, ALS KLEINE SLICESIZE, NI GENOEG RUILTE VOOR 64 ITERATIES
	
	/* rotate-align the blocks & copy into double-extended buffer at the upper half of the buffer */
	for (int i = slice_size; i < (slice_size << 1); i++)
		b_blocks[i].raw = ROL64(blocks.values[i & range_mask].raw, (i & range_mask));
	
	/* iterate over every 'significance' level the pt blocks have, which are 64 */
	/* when iterating, slowly rotate the blocks towards the beginning on every iteration, aligning the next significance every time */
	for (int i = 64; i > 0; --i) {
		auto ind = 64 - i;
		
		/* construct the slice for this significance level */
		auto res = slice_significance_accelerated(b_blocks + i).value;
		
		/* move the last block in the buffer to the front and decrement the blocks iterator by 1 */
		b_blocks[i - 1] = b_blocks[double_len - 1 - ind];
		
		/* Now re-align the slice by rotating back and put it in the buffer */
		slices[ind].value = ROR(res, ind); // rotate slices back into place
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
	for (int i = 0; i < 64; i++) result.raw[i].value = slices[i].value;
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
	#if slice_size == 128
	uint8_t chunks[2] = {0, 64};
	
	// loop over every segment, __m128i has 2x 64-bit chunks
	for (int i = 0; i < 2; i++) {
		auto chunk = chunks[i];
		for (int bit_index = chunk; bit_index < chunk + 64; ++bit_index) {
			u64 chunk_mask = 1ULL << (bit_index - chunk);
			blocks->values[bit_index].raw |= ((slice.chunks[i] & chunk_mask) >> (bit_index - chunk)) << significance;
		}
	}
	
	#elif slice_size == 256
	uint8_t chunks[4] = {0, 64, 128, 192};
	
	// loop over every segment, __m256i has 4x 64-bit chunks
	for (int i = 0; i < 4; i++) {
		auto chunk = chunks[i];
		for (int b_number = chunk; b_number < chunk + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunk);
			blocks->values[b_number].raw |= ((slice.chunks[i] & mask) >> (b_number - chunk)) << significance;
		}
	}
	#elif slice_size == 512
	uint16_t chunks[8] = {0, 64, 128, 192, 256, 320, 384, 448};
	
	// loop over every chunk, __m512i has 8x 64-bit chunks
	for (int i = 0; i < 8; i++) {
		auto chunk = chunks[i];
		for (int b_number = chunk; b_number < chunk + 64; ++b_number) {
			u64 mask = 1ULL << (b_number - chunk);
			blocks->values[b_number].raw |= ((slice.chunks[i] & mask) >> (b_number - chunk)) << significance;
		}
	}
	#else
	for (uint b_number = 0; b_number < slice_size; ++b_number) {
		u64 mask = 1ULL << b_number;
		blocks->values[b_number].raw |= ((slice.value & mask) >> b_number) << significance;
	}
	
	#endif
	
}

static inline Blocks64_t unslice_accelerated(State64Sliced_t state) {
	
	Blocks64_t unsliced = Blocks64_t();
	for (int i = 0; i < 64; ++i)
		unslice_significance(state.raw[i], &unsliced, i);
	
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_ACCELERATED_H
