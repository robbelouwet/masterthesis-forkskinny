#ifndef FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H
#define FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H

#define FORKSKINNY_ROUNDS_BEFORE 17
#define FORKSKINNY_ROUNDS_AFTER 23
#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_ROUNDS_BEFORE + 2*FORKSKINNY_ROUNDS_AFTER)

#include <cstdint>
#include "config.h"
#include "immintrin.h"

/** ---- SKINNY64 ---- */
typedef union {
	slice_t value;
	
	#if slice_size == 256
	uint64_t segments[4];
	#elif slice_size == 512
	uint64_t segments[8];
	#endif
	
} Slice_t;

typedef union {
	#if SKINNY_64BIT
	uint64_t raw;
	#else
	uint32_t raw[2];
	#endif
	
	unsigned char bytes[8];
} Block_t;

typedef union {
	Block_t values[slice_size];
} Blocks_t;

typedef union {
	Slice_t slices[4];
	#if AVX2_acceleration || AVX512_acceleration
	__m256i avx2_simd_cell;
	#endif
} Cell_t;

typedef union {
	#if AVX512_acceleration
	__m512i avx512_simd_pair;
	#endif
	Cell_t cells[2];
} Pair_t;

typedef union {
	Cell_t cols[4];
	#if AVX512_acceleration
	Pair_t pairs[2];
	#endif
} Row_t;

typedef union {
	Slice_t raw[32];
	Cell_t cells[8];
	Pair_t pairs[4];
} HalfStateSliced_t;

typedef union {
	Slice_t raw[64];
	Cell_t cells[16];
	Row_t rows[4];
	Pair_t pairs[8];
	HalfStateSliced_t halves[2];
} StateSliced_t;

typedef union {
	/** All words of the key keys */
	HalfStateSliced_t keys[FORKSKINNY64_MAX_ROUNDS];
} KeyScheduleSliced_t;

typedef struct {
	StateSliced_t C1;  // <- branch constant
	StateSliced_t C0;
	StateSliced_t M;
} SlicedCiphertext_t;

#endif //FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H
