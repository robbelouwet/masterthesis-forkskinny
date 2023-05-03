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
static inline Slice64_t slice_significance_accelerated(const Block64_t *blocks, uint16_t amount) {
	auto slice = Slice64_t();
	
	#if slice_size == 128
	for (uint i = 0; i < 64; ++i)
		slice.chunks[0] |= (blocks.values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 128; ++i)
		slice.chunks[1] |= (blocks.values[i].raw & mask) >> significance << (i - 64);
	
	#elif slice_size == 256
	for (uint i = 0; i < 64; ++i)
		slice.chunks[0] |= (blocks.values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 128; ++i)
		slice.chunks[1] |= (blocks.values[i].raw & mask) >> significance << (i - 64);
	for (uint i = 128; i < 192; ++i)
		slice.chunks[2] |= (blocks.values[i].raw & mask) >> significance << (i - 128);
	for (uint i = 192; i < 256; ++i)
		slice.chunks[3] |= (blocks.values[i].raw & mask) >> significance << (i - 192);
	
	#elif slice_size == 512
	for (uint i = 0; i < 64; ++i)
		slice.chunks[0] |= (blocks.values[i].raw & mask) >> significance << i;
	for (uint i = 64; i < 128; ++i)
		slice.chunks[1] |= (blocks.values[i].raw & mask) >> significance << (i - 64);
	for (uint i = 128; i < 192; ++i)
		slice.chunks[2] |= (blocks.values[i].raw & mask) >> significance << (i - 128);
	for (uint i = 192; i < 256; ++i)
		slice.chunks[3] |= (blocks.values[i].raw & mask) >> significance << (i - 192);
	for (uint i = 256; i < 320; ++i)
		slice.chunks[4] |= (blocks.values[i].raw & mask) >> significance << (i - 256);
	for (uint i = 320; i < 384; ++i)
		slice.chunks[5] |= (blocks.values[i].raw & mask) >> significance << (i - 320);
	for (uint i = 384; i < 448; ++i)
		slice.chunks[6] |= (blocks.values[i].raw & mask) >> significance << (i - 384);
	for (uint i = 448; i < 512; ++i)
		slice.chunks[7] |= (blocks.values[i].raw & mask) >> significance << (i - 448);
	
	#else
	for (uint i = 0; i < amount; ++i)
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
static inline State64Sliced_t slice_accelerated(Blocks64_t blocks, uint16_t const amount = slice_size) {
	auto const double_amount = amount << 1;
	Block64_t b_blocks[double_amount]; // buffer
	Slice64_t slices[64];
	auto range_mask = amount - 1;
	auto block_mask = MASK_64(64);
	
	// rotate-align the blocks & copy into double-extended buffer
	for (int i = amount; i < (amount << 1); i++){
		b_blocks[i].raw = ROL(blocks.values[i & range_mask].raw, (i & range_mask), amount);
		int appel = 1;
	}
	
	// slice the blocks
	for (int i = amount; i > 0; --i) {
		// 0xA, 0b00111, 0b00111, 0b10001, 0b110110
		// There will always be 64 slices, but a slice will be smaller if less blocks are sliced
		auto res = slice_significance_accelerated(b_blocks + i, amount).value;
		b_blocks[i - 1] = b_blocks[i + amount - 1]; // pull last block to the front and move iterator 1 back
		
		// 0b01110, 0b10011, 0b11001, 0b00110, 0b10101
		slices[amount - i].value = ROR(res, (amount - i), amount) & block_mask; // rotate back into place
	}
	
	// If AVX is enabled, pack 4 (AVX2) or 8 (AVX512) equally significant slices in the same segment
	auto result = State64Sliced_t();
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
	for (int i = 0; i < 64; result.raw[i] = slices[i++]);
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
