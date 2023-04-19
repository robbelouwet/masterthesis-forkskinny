#ifndef FORKSKINNYPLUS64_FORKSKINNY_DATATYPES_H
#define FORKSKINNYPLUS64_FORKSKINNY_DATATYPES_H

#define FORKSKINNY_ROUNDS_BEFORE 17
#define FORKSKINNY_ROUNDS_AFTER 23
#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_ROUNDS_BEFORE + 2*FORKSKINNY_ROUNDS_AFTER)

#include <cstdint>
#include "../../config.h"
//#include "immintrin.h"

/** ---- SKINNY64 ---- */
typedef union {
	slice_t value;
	
	#if slice_size == 128
	uint64_t chunks[2];
	#elif slice_size == 256
	uint64_t chunks[4];
	#elif slice_size == 512
	uint64_t chunks[8];
	#endif
	
} Slice64_t;

typedef union {
	#if SKINNY_64BIT
	uint64_t raw;
	#else
	uint32_t raw[2];
	#endif
	
	unsigned char bytes[8];
} Block64_t;

typedef union {
	Block64_t values[slice_size];
} Blocks64_t;

typedef union {
	Slice64_t slices[4];
	#if AVX2_acceleration || AVX512_acceleration
	__m256i avx2_simd_cell;
	#endif
} Cell64_t;

typedef union {
	#if AVX512_acceleration
	__m512i avx512_simd_pair;
	#endif
	Cell64_t cells[2];
} Pair64_t;

typedef union {
	Cell64_t cols[4];
	#if AVX512_acceleration
	Pair64_t pairs[2];
	#endif
} Row64_t;

typedef union {
	Slice64_t raw[32];
	Cell64_t cells[8];
	Pair64_t pairs[4];
	Row64_t rows[2];
} HalfState64Sliced_t;

typedef union {
	Slice64_t raw[64];
	Cell64_t cells[16];
	Row64_t rows[4];
	Pair64_t pairs[8];
	HalfState64Sliced_t halves[2];
} State64Sliced_t;

typedef union {
	/** All words of the key keys */
	HalfState64Sliced_t keys[FORKSKINNY64_MAX_ROUNDS];
} KeySchedule64Sliced_t;

typedef struct {
	State64Sliced_t C1;  // <- branch constant
	State64Sliced_t C0;
	State64Sliced_t M;
} SlicedCiphertext64_t;

#endif //FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H
