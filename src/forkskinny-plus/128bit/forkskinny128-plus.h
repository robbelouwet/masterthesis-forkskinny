#ifndef FORKSKINNYPLUS_FORKSKINNY128_PLUS_H
#define FORKSKINNYPLUS_FORKSKINNY128_PLUS_H

#define FORKSKINNY_128_256_ROUNDS_BEFORE 21
#define FORKSKINNY_128_256_ROUNDS_AFTER 27
#define FORKSKINNY_128_384_ROUNDS_BEFORE 25
#define FORKSKINNY_128_384_ROUNDS_AFTER 31
#define FORKSKINNY128_MAX_ROUNDS (FORKSKINNY_128_384_ROUNDS_BEFORE + 2*FORKSKINNY_128_384_ROUNDS_AFTER)

#include <cstdint>
#include <immintrin.h>

/** ---- SKINNY128 ---- */
typedef union {
	// a 128bit block represented as 2x 64bit integers
	// values[i][0] is less significant than values[i][1]
	uint64_t values[64][2];
} Blocks128;

typedef union {
	uint64_t slices[8];
	__m256i simd_cells[2];
} Cell128;

typedef union {
	uint64_t raw[64];
	Cell128 cells[8];
} HalfState128Sliced_t;

typedef union {
	uint64_t raw[128];
	Cell128 cells[16];
	HalfState128Sliced_t halves[2];
} State128Sliced_t;

typedef struct {
	/** All words of the key keys */
	HalfState128Sliced_t keys[FORKSKINNY128_MAX_ROUNDS];
} KeySchedule128Sliced_t;

#endif //FORKSKINNYPLUS_FORKSKINNY128_PLUS_H
