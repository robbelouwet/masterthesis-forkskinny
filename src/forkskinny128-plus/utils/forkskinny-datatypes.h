#ifndef FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H
#define FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H

#define FORKSKINNY_128_256_ROUNDS_BEFORE 21
#define FORKSKINNY_128_256_ROUNDS_AFTER 27
#define FORKSKINNY_128_384_ROUNDS_BEFORE 25
#define FORKSKINNY_128_384_ROUNDS_AFTER 31
#define FORKSKINNY128_MAX_ROUNDS (FORKSKINNY_128_384_ROUNDS_BEFORE + 2*FORKSKINNY_128_384_ROUNDS_AFTER)

#include <cstdint>
#include "config.h"
#include "immintrin.h"

/** ---- SKINNY128 ---- */
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
	uint64_t raw[2];
	#else
	uint32_t raw[4];
	#endif
	unsigned char bytes[16];
} Block_t;

typedef union {
	Block_t values[slice_size];
} Blocks_t;

typedef union {
	Slice_t slices[8];
	#if AVX512_acceleration
	__m512i avx512_simd_cell;
	#elif AVX2_acceleration
	__m256i avx2_simd_cells[2];
	#endif
} Cell_t;

typedef union {
	Cell_t cols[4];
} Row_t;

typedef union {
	Slice_t raw[64];
	Cell_t cells[8];
} HalfStateSliced_t;

typedef union {
	Slice_t raw[128];
	Cell_t cells[16];
	Row_t rows[4];
	HalfStateSliced_t halves[2];
} StateSliced_t;

typedef union {
	/** All words of the key keys */
	HalfStateSliced_t keys[FORKSKINNY128_MAX_ROUNDS];
} KeyScheduleSliced_t;

typedef struct {
	StateSliced_t C1;  // <- branch constant
	StateSliced_t C0;
	StateSliced_t M;
} SlicedCiphertext_t;

#endif //FORKSKINNYPLUS_FORKSKINNY_DATATYPES_H
