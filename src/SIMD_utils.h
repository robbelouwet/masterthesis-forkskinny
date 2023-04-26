#ifndef FORKSKINNYPLUS_SIMD_UTILS_H
#define FORKSKINNYPLUS_SIMD_UTILS_H

#include <immintrin.h>
#include "config.h"

#if AVX2_support
// Shift left by a specified number of bits
// Shift left by a specified number of bits
static inline __m128i shift_left_128(__m128i input, int shift) {
	__m128i shifted = _mm_or_si128(_mm_slli_si128(input, shift / 8),
	                               _mm_srli_epi64(input, (64 - shift % 64) % 64));
	return shifted;
}

// Rotate left by a specified number of bits
static inline __m128i rotate_left_128(__m128i input, int shift) {
	const int sub_shift = shift & 127; // equivalent to shift % 128
	__m128i rotated = shift_left_128(input, sub_shift);
	rotated = _mm_or_si128(rotated, shift_left_128(input, sub_shift - 128));
	return rotated;
}

// Shift left by a specified number of bits
static inline __m256i shift_left_256(__m256i input, int shift) {
	__m256i shifted = _mm256_bslli_epi128(input, shift / 128);
	shifted = _mm256_or_si256(shifted, _mm256_slli_epi64(input, shift % 64));
	return shifted;
}

// Rotate left by a specified number of bits
static inline __m256i rotate_left_256(__m256i input, int shift) {
	const int sub_shift = shift & 255; // equivalent to shift % 256
	__m256i rotated = shift_left_256(input, sub_shift);
	rotated = _mm256_or_si256(rotated, shift_left_256(input, sub_shift - 256));
	return rotated;
}
#endif
#if AVX512_support
// Shift left by a specified number of bits
static inline __m512i shift_left_512(__m512i input, int shift) {
    __m512i shifted = _mm512_bslli_epi128(input, shift / 128);
    shifted = _mm512_or_si512(shifted, _mm512_slli_epi64(input, shift % 64));
    return shifted;
}

// Rotate left by a specified number of bits
static inline __m512i rotate_left_512(__m512i input, int shift) {
    const int sub_shift = shift & 511; // equivalent to shift % 512
    __m512i rotated = shift_left_512(input, sub_shift);
    rotated = _mm512_or_si512(rotated, shift_left_512(input, sub_shift - 512));
    return rotated;
}
#endif

#endif //FORKSKINNYPLUS_SIMD_UTILS_H
