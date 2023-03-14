#ifndef FORKSKINNYPLUS_SKINNY64_DATATYPES_H
#define FORKSKINNYPLUS_SKINNY64_DATATYPES_H

#define FORKSKINNY_ROUNDS_BEFORE 17
#define FORKSKINNY_ROUNDS_AFTER 23
#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_ROUNDS_BEFORE + 2*FORKSKINNY_ROUNDS_AFTER)

#include <cstdint>
#include "config.h"
#include "immintrin.h"

/** ---- SKINNY64 ---- */
typedef union {
	uint64_t raw;
	unsigned char bytes[16];
} Block64_t;

typedef union {
	Block64_t values[slice_size];
} Blocks64_t;

typedef union {
	slice_t slices[4];
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
	slice_t raw[32];
	Cell64_t cells[8];
	Pair64_t pairs[4];
} HalfState64Sliced_t;

typedef union {
	slice_t raw[64];
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
} SlicedCiphertext_t;

#endif //FORKSKINNYPLUS_SKINNY64_DATATYPES_H
