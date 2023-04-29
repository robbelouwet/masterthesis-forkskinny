#ifndef FORKSKINNYPLUS_SIMD_UTILS_H
#define FORKSKINNYPLUS_SIMD_UTILS_H

#include <immintrin.h>
#include <emmintrin.h>
#include "config.h"

#if AVX2_support
static inline __m128i mm_rotr_si128(__m128i v, uint8_t shift) {
	if (shift == 0) return v;
	shift &= 127;
	
	// shift = x * 64 + y, so we can already rotate in x blocks of 64 bits
	// by performing a permutation on 2x 64-bit blocks, then only y bit-shifts remain
	if (shift >= 64) {
		v = _mm_shuffle_epi32(v, 0b01001110); // D, C, B, A -> B, A, D, C
		shift &= 0b111111;
	}
	if (shift == 0) return v;
	
	// mask & rotate the bits to be overflow, and rotate blocks 1 position to the right
	uint64_t imask = (uint64_t(0x1) << shift) - 1;
	auto masked_bits = _mm_and_si128(_mm_set_epi64x(imask, imask), v);
	masked_bits = _mm_slli_epi64(masked_bits, 64 - shift);
	masked_bits = _mm_shuffle_epi32(masked_bits, 0b01001110);
	
	auto segment = _mm_srli_epi64(v, shift);
	auto res = _mm_or_si128(segment, masked_bits);
	
	return res;
}

static inline __m256i mm256_rotr_si256(__m256i v, uint8_t shift) {
	if (shift == 0) return v;
	shift &= 255;
	
	// shift = x * 64 + y, so we can already rotate in x blocks of 64 bits
	// by performing a permutation on 4x 64-bit blocks, then only y bit-shifts remain
	if (shift >= 192) {
		// rotate-right 192 bits, x = 3, so every block rotates 3 positions
		v = _mm256_permute4x64_epi64(v, 0b10010011); // A, B, C, D -> B, C, D, A
		shift ^= 192;
	} else if (shift >= 128) {
		// rotate-right 128 bits, x = 2, every block rotates 2 positions
		v = _mm256_permute4x64_epi64(v, 0b01001110); // A, B, C, D -> C, D, A, B
		shift ^= 128;
	} else if (shift >= 64) {
		// rotate-right 64 bits, x = 1, blocks rotate 1 position
		v = _mm256_permute4x64_epi64(v, 0b00111001); // A, B, C, D -> D, A, B, C
		shift ^= 64;
	}
	
	if (shift == 0) return v;
	
	// mask & rotate the bits to be overflow, and rotate blocks 1 position to the right
	uint64_t imask = (uint64_t(0x1) << shift) - 1;
	auto masked_bits = _mm256_and_si256(_mm256_set_epi64x(imask, imask, imask, imask), v);
	masked_bits = _mm256_slli_epi64(masked_bits, 64 - shift);
	masked_bits = _mm256_permute4x64_epi64(masked_bits, 0b00111001);
	
	auto segment = _mm256_srli_epi64(v, shift);
	auto res = _mm256_or_si256(segment, masked_bits);
	
	return res;
}

#endif


#if AVX512_support


/// This one doesn't use simd operations because _mm512 operations can't move across the lower and upper 256-bit lanes.
static inline __m512i mm512_rotr_si512(__m512i v, uint16_t shift) {
	if (shift == 0) return v;
	
	shift &= 511;
	auto lanes_to_jump = shift >> 6; // shift / 64
	shift &= 63; // shift % 64
	
	__m512i dst;
	for (int i = 7; i >= 0; --i) dst[i] = v[(i + lanes_to_jump) & 7];
	if (shift == 0) return dst;
	
	// mask & rotate the bits to be overflow, and rotate blocks 1 position to the right
	uint64_t imask = (uint64_t(0x1) << shift) - 1;
	auto masked_bits = _mm512_and_si512(_mm512_set_epi64(imask, imask, imask, imask, imask, imask, imask, imask), dst);
	masked_bits = _mm512_slli_epi64(masked_bits, 64 - shift);
	
	__m512i mask_dst;
	for (int i = 7; i >= 0; --i)
		mask_dst[i] = masked_bits[(i+1) & 7];
	
	auto segment = _mm512_srli_epi64(dst, shift);
	auto res = _mm512_or_si512(segment, mask_dst);
	
	return res;
}

#endif

#endif //FORKSKINNYPLUS_SIMD_UTILS_H
