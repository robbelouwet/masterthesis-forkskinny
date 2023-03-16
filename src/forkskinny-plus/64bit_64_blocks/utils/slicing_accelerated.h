#ifndef FORKSKINNYPLUS_ACCELERATED_SLICING_H
#define FORKSKINNYPLUS_ACCELERATED_SLICING_H

#include <cstdint>
#include <x86intrin.h>
#include "skinny64_datatypes.h"

/**
 * Perform a rotate-right bit rotation on a 256-bit __m256i value.
 * @param v
 * @param shift amount of bits to rotate.
 * @return
 */
static inline __m256i mm256_rotr_si256(__m256i v, uint8_t shift) {
	
	// shift = x * 64 + y, so we can already rotate in x blocks of 64 bits
	// by performing a permutation on 4x 64-bit blocks, then only y bit-shifts remain
	if (shift >= 192) {
		// rotate-right 192 bits, x = 3, so every block rotates 3 positions
		v = _mm256_permute4x64_epi64(v, 0b10010011); // A, B, C, D -> B, C, D, A
		shift ^= 192;
	}
	else if (shift >= 128) {
		// rotate-right 128 bits, x = 2, every block rotates 2 positions
		v = _mm256_permute4x64_epi64(v, 0b00111001); // A, B, C, D -> C, D, A, B
		shift ^= 128;
	}
	else if (shift >= 64) {
		// rotate-right 64 bits, x = 1, blocks rotate 1 position
		v = _mm256_permute4x64_epi64(v, 0b00111001); // A, B, C, D -> D, A, B, C
		shift ^= 64;
	}
	
	if (shift == 0) return v;
	
	// mask & rotate the bits to be overflow, and rotate blocks 1 position to the right
	uint64_t imask = (((uint64_t(0x1) << shift) - 1) << 1) | 1;
	auto masked_bits = _mm256_and_si256(_mm256_set_epi64x(imask, imask, imask, imask), v);
	masked_bits = _mm256_slli_epi64(masked_bits, 64 - shift);
	masked_bits = _mm256_permute4x64_epi64(masked_bits, 0b00111001);
	
	auto segment = _mm256_srli_epi64(v, shift);
	auto res = _mm256_or_si256(segment, masked_bits);
	
	return res;
}

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline Slice64_t slice_significance_accelerated(const Blocks64_t blocks, uint8_t significance) {
	uint64_t mask = 1ULL << significance;
	auto slice = Slice64_t();
	
	#if slice_size == 256
	
	
	#elif slice_size == 512
	
	#else
	
	#endif
	
	return slice;
}

static inline State64Sliced_t *slice_accelerated(Blocks64_t blocks) {
	// make it 32-byte aligned for fast memory access
	auto *result = (State64Sliced_t *) aligned_alloc(32, sizeof(State64Sliced_t));
	
	// align the blocks
	for (uint i = 1; i < slice_size; ++i) {
		#if SKINNY_64BIT
		blocks.values[i].raw = _rotr(blocks.values[i].raw, i);
		#else
		// TODO
		#endif
	}
	
	for (int i = 0; i < 64; ++i) {
		auto slice = Slice64_t();
		
		#if slice_size == 64
		uint64_t mask = 0x8000000000000000ULL >> i;
		int j = 0;
		for (; j < slice_size; ++j) {
			slice |= (blocks.values[i].raw & mask);
			mask = _rotr(mask, 1);
		}
		result->raw[j].value = _rotl(slice, i);
		#elif slice_size == 256
		uint64_t mask = 0x8000000000000000ULL >> i;
		int j = 0;
		for (; j < 64; ++j) {
			slice.segments[0] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		for (; j < 128; ++j) {
			slice.segments[1] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		for (; j < 192; ++j) {
			slice.segments[2] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		for (; j < 256; ++j) {
			slice.segments[3] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		
		#else
		#endif
		
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
static inline void unslice_significance_accelerated(const Slice64_t slice, Blocks64_t *blocks, uint8_t sb_index) {
#if slice_size == 256

#elif slice_size == 512

#else

#endif
	
}

static inline Blocks64_t unslice_accelerated(State64Sliced_t state) {
	Blocks64_t unsliced = Blocks64_t();
	for (int i = 0; i < 64; ++i) {
		unslice_significance(state.raw[i], &unsliced, i);
	}
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_ACCELERATED_SLICING_H
