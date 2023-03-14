#ifndef FORKSKINNYPLUS_CONFIG_H
#define FORKSKINNYPLUS_CONFIG_H

// @formatter:off
#define slice_size 64 // 32, 64, 256 or 512
#define AVX2_support true
#define AVX512_support true


// ----- 32-bit slices -----
#if slice_size == 32
#define slice uint32_t
#define XOR_SLICE(s1, s2) (s1 ^ s2)
#define NOT_AND_SLICE(s1, s2) (~s1 & s2)
#define OR_SLICE(s1, s2) (s1 | s2)



// ----- 64-bit slices -----
#elif slice_size == 64
#define slice uint64_t
#define XOR_SLICE(s1, s2) (s1 ^ s2)
#define NOT_AND_SLICE(s1, s2) (~s1 & s2)
#define OR_SLICE(s1, s2) (s1 | s2)

#if AVX512_support  // pack 2 cells or 8x 64-bit slices in 1x 512-bit simd lane
#define segment __m512i
#define segment_amount 8
#elif AVX2_support  // pack 1 cell or 4x 64-bit slices in 1x 256-bit simd lane
#define segment __m256i
#define segment_size 16
#endif



// ----- 256-bit slices (AVX256 registers) -----
#elif slice_size == 256
#define slice __m256i
#define XOR_SLICE(s1, s2) _mm256_xor_si256(s1, s2)
#define NOT_AND_SLICE(s1, s2) _mm256_andnot_si256(s1, s2)
#define OR_SLICE(s1, s2) _mm256_or_si256(s1, s2)



// ----- 512-bit slices (AVX512 registers) -----
#elif slice_size == 512
#define slice __m512i
#define XOR_SLICE(s1, s2) _mm512_xor_si512(s1, s2)
#define NOT_AND_SLICE(s1, s2) _mm512_andnot_si512(s1, s2)
#define OR_SLICE(s1, s2) _mm512_or_si512(s1, s2)
#endif

// slice_size / 8
#define bytes_per_slice (slice_size >> 3)

// @formatter:on
#endif //FORKSKINNYPLUS_CONFIG_H
