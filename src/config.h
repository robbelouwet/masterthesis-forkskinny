#ifndef CONFIG_H
#define CONFIG_H

#include <cstdio>
#include <cstdint>
#include "immintrin.h"

// @formatter:off
// -- CONFIG --
#define slice_size 32 // 8, 32, 64, 128, 256 or 512
#define AVX2_support false
#define AVX512_support false
// ------------

/* Define SKINNY_64BIT to 1 if the CPU is natively 64-bit */
#if defined(__WORDSIZE) && __WORDSIZE == 64
#define SKINNY_64BIT true
#else
#define SKINNY_64BIT false
#endif

#define AVX2_acceleration (slice_size == 64 && AVX2_support)
#define AVX512_acceleration (slice_size == 64 && AVX512_support)
#define u64 uint64_t

#define ROR64(v, i) ((v >> i) | (v << (64 - i)))
#define ROL64(v, i) ROR64(v, (64 - i))

// ----- 8-bit slices -----
#if slice_size == 8
	#define slice_t uint8_t
	#define ONE uint8_t(0xFF)
	#define ZER uint8_t(0x0)
	#define BIT(i) (uint8_t(1) << i)
	#define MASK(i) (slice_t(-1) >> (8 - i))
	#define ROR(v, i) ((v >> i) | (v << (8 - i)))
	#define XOR_SLICE(s1, s2) (s1 ^ s2)
	#define OR_SLICE(s1, s2) (s1 | s2)
	#define AND_SLICE(s1, s2) (s1 & s2)


// ----- 32-bit slices -----
#elif slice_size == 32
	#define slice_t uint32_t
	#define ONE uint32_t(0xFFFFFFFF)
	#define ZER uint32_t(0x0)
	#define BIT(i) (uint32_t(1) << i)
	#define MASK(i) (slice-t(-1) >> (32 - i))
	#define ROR(v, i) ((v >> i) | (v << (32 - i)))
	#define XOR_SLICE(s1, s2) (s1 ^ s2)
	#define OR_SLICE(s1, s2) (s1 | s2)
	#define AND_SLICE(s1, s2) (s1 & s2)


// ----- 64-bit slices -----
#elif slice_size == 64
	#define slice_t uint64_t
	#define ONE 0xFFFFFFFFFFFFFFFFULL
	#define ZER 0x0ULL
	#define BIT(i) (0x1ULL << i)
	#define MASK(i) (slice_t(-1) >> (64 - i))
	#define ROR(v, i) ((v >> i) | (v << (64 - i)))
	#define XOR_SLICE(s1, s2) (s1 ^ s2)
	#define OR_SLICE(s1, s2) (s1 | s2)
	#define AND_SLICE(s1, s2) (s1 & s2)


// ----- 128-bit slices -----
#elif slice_size == 128
	#define slice_t __m128i
	#define ONE _mm_set1_epi64x(-1)
	#define ZER _mm_setzero_si128()
	#define BIT(i) mm_rotr_si128(_mm_set_epi64x(0, 1), 128 - i)
	#define XOR_SLICE(s1, s2) _mm_xor_si128(s1, s2)
	#define OR_SLICE(s1, s2) _mm_or_si128(s1, s2)
	#define AND_SLICE(s1, s2) _mm_and_si128(s1, s2)


// ----- 256-bit slices (AVX256 registers) -----
#elif slice_size == 256
	#define slice_t __m256i
	#define ONE _mm256_set1_epi64x(-1)
	#define ZER _mm256_setzero_si256()
	#define BIT(i) mm256_rotr_si256(_mm256_set_epi64x(0, 0, 0, 1), 256 - i)
	#define XOR_SLICE(s1, s2) _mm256_xor_si256(s1, s2)
	#define OR_SLICE(s1, s2) _mm256_or_si256(s1, s2)
	#define AND_SLICE(s1, s2) _mm256_and_si256(s1, s2)


// ----- 512-bit slices (AVX512 registers) -----
#elif slice_size == 512
	#define slice_t __m512i
	#define ONE _mm512_set1_epi64(-1)
	#define ZER _mm512_setzero_si512()
	#define BIT(i) mm512_rotr_si512(_mm512_set_epi64x(0, 0, 0, 0, 0, 0, 0, 1), 512 - i)
	#define XOR_SLICE(s1, s2) _mm512_xor_si512(s1, s2)
	#define OR_SLICE(s1, s2) _mm512_or_si512(s1, s2)
	#define AND_SLICE(s1, s2) _mm512_and_si512(s1, s2)
#else
	#error "Please specify a valid configuration"
#endif
// @formatter:on
// mask the end result to get correct result for small slice on larger-register platform
#define ROL(v, i, regwidth) ROR(v, (regwidth - i))

#if slice_size == 128
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
	u64 imask = -1ULL >> (64 - shift);;
	auto masked_bits = _mm_and_si128(_mm_set_epi64x(imask, imask), v);
	masked_bits = _mm_slli_epi64(masked_bits, 64 - shift);
	masked_bits = _mm_shuffle_epi32(masked_bits, 0b01001110);
	
	auto segment = _mm_srli_epi64(v, shift);
	auto res = _mm_or_si128(segment, masked_bits);
	
	return res;
}
#define ROR mm_rotr_si128

#elif slice_size == 256
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
	u64 imask = -1ULL >> (64 - shift);
	auto masked_bits = _mm256_and_si256(_mm256_set_epi64x(imask, imask, imask, imask), v);
	masked_bits = _mm256_slli_epi64(masked_bits, 64 - shift);
	masked_bits = _mm256_permute4x64_epi64(masked_bits, 0b00111001);
	
	auto segment = _mm256_srli_epi64(v, shift);
	auto res = _mm256_or_si256(segment, masked_bits);
	
	return res;
}

#define ROR mm256_rotr_si256

#elif slice_size == 512
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
	u64 imask = -1ULL >> (64 - shift);;
	auto masked_bits = _mm512_and_si512(_mm512_set_epi64(imask, imask, imask, imask, imask, imask, imask, imask), dst);
	masked_bits = _mm512_slli_epi64(masked_bits, 64 - shift);
	
	__m512i mask_dst;
	for (int i = 7; i >= 0; --i)
		mask_dst[i] = masked_bits[(i+1) & 7];
	
	auto segment = _mm512_srli_epi64(dst, shift);
	auto res = _mm512_or_si512(segment, mask_dst);
	
	return res;
}
#define ROR mm512_rotr_si512
#endif


//#define MASK128(i) (i <= 64 ? _mm_set_epi64(0, -1ULL >> (64 - i)) : _mm_set_epi64(-1ULL >> (64 - (i & 63)), -1ULL))
//#define MASK256(i) (i <= 128 ? _mm256_setr_m128i(MASK128(i), _mm_setzero_si128()) : _mm256_setr_m128i(_mm_setzero_si128(), MASK128(i - 128)))
//#define MASK512(v, i) { \
//if (i <= 256)}
void print_block(uint8_t *block, unsigned int n) {
	for (unsigned int i = 0; i < n; i++)
		printf("%02x", block[i]);
}

#endif //CONFIG_H
