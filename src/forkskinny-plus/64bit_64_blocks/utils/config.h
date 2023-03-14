#ifndef FORKSKINNYPLUS_CONFIG_H
#define FORKSKINNYPLUS_CONFIG_H

// @formatter:off
#define slice_size 64 // 32, 64, 256 or 512
#define AVX2_acceleration (slice_size == 64 && false)
#define AVX512_acceleration (slice_size == 64 && false)

// ----- 32-bit slices -----
#if slice_size == 32
#define slice_t uint32_t
#define ONE uint32_t(0xFFFFFFFF)
#define ZER uint32_t(0x0)

#define XOR_SLICE(s1, s2) (s1 ^ s2)
#define NOT_AND_SLICE(s1, s2) (~s1 & s2)
#define OR_SLICE(s1, s2) (s1 | s2)



// ----- 64-bit slices -----
#elif slice_size == 64
#define slice_t uint64_t
#define ONE uint64_t(0xFFFFFFFFFFFFFFFF)
#define ZER uint64_t(0x0)

#define XOR_SLICE(s1, s2) (s1 ^ s2)
#define NOT_AND_SLICE(s1, s2) (~s1 & s2)
#define OR_SLICE(s1, s2) (s1 | s2)



// ----- 256-bit slices (AVX256 registers) -----
#elif slice_size == 256
#define slice_t __m256i
#define ONE _mm256_set1_epi64x(-1)
#define ZER _mm256_setzero_si256()

#define XOR_SLICE(s1, s2) _mm256_xor_si256(s1, s2)
#define NOT_AND_SLICE(s1, s2) _mm256_andnot_si256(s1, s2)
#define OR_SLICE(s1, s2) _mm256_or_si256(s1, s2)



// ----- 512-bit slices (AVX512 registers) -----
#elif slice_size == 512
#define slice_t __m512i
#define ONE _mm512_set1_epi64(-1)
#define ZER _mm512_setzero()

#define XOR_SLICE(s1, s2) _mm512_xor_si512(s1, s2)
#define NOT_AND_SLICE(s1, s2) _mm512_andnot_si512(s1, s2)
#define OR_SLICE(s1, s2) _mm512_or_si512(s1, s2)
#endif

// slice_size / 8
#define bytes_per_slice (slice_size >> 3)

// @formatter:on
#endif //FORKSKINNYPLUS_CONFIG_H
