#ifndef FORKSKINNYPLUS128_FORKSKINNY_DATATYPES_H
#define FORKSKINNYPLUS128_FORKSKINNY_DATATYPES_H

#define FORKSKINNY_128_256_ROUNDS_BEFORE 21
#define FORKSKINNY_128_256_ROUNDS_AFTER 27
#define FORKSKINNY_128_384_ROUNDS_BEFORE 25
#define FORKSKINNY_128_384_ROUNDS_AFTER 31
#define FORKSKINNY128_MAX_ROUNDS (FORKSKINNY_128_384_ROUNDS_BEFORE + 2*FORKSKINNY_128_384_ROUNDS_AFTER)

#include <cstdint>
//#include "immintrin.h"
#include "../../config.h"

/** ---- SKINNY128 ---- */
typedef union {
	slice_t value;
	
	#if slice_size == 128
	u64 chunks[2];
	#elif slice_size == 256
	u64 chunks[4];
	#elif slice_size == 512
	u64 chunks[8];
	#endif
	
} Slice128_t;

typedef union {
	#if AVX512_acceleration
	__m512i __attribute__((aligned(32))) lane;
	u64 segments[8];
	#elif AVX2_acceleration
	__m256i __attribute__((aligned(32))) lane;
	u64 segments[4];
	#endif
} Segment128_t;

typedef union {
	#if SKINNY_64BIT
	u64 raw[2];
	#else
	uint32_t raw[4];
	#endif
	unsigned char bytes[16];
} Block128_t;

typedef union {
	Block128_t values[slice_size];
} Blocks128_t;

typedef union {
	Slice128_t slices[8];
	#if AVX512_acceleration
	__m512i __attribute__((aligned(32))) avx512_simd_cell;
	#endif
	#if AVX2_acceleration | AVX512_acceleration
	__m256i __attribute__((aligned(32))) avx2_simd_cells[2];
	#endif
} Cell128_t;

typedef union {
	Cell128_t cols[4];
} Row128_t;

typedef union {
	Slice128_t raw[64];
	Cell128_t cells[8];
	Row128_t rows[2];
} HalfState128Sliced_t;

typedef union {
	Slice128_t raw[128];
	Cell128_t cells[16];
	Row128_t rows[4];
	HalfState128Sliced_t halves[2];
} State128Sliced_t;

typedef union {
	/** All words of the key keys */
	HalfState128Sliced_t keys[FORKSKINNY128_MAX_ROUNDS];
} KeySchedule128Sliced_t;

typedef struct {
	State128Sliced_t C1;  // <- branch constant
	State128Sliced_t C0;
	State128Sliced_t M;
} SlicedCiphertext128_t;

#endif //FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H
