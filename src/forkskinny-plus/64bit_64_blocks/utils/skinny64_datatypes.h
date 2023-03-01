#ifndef FORKSKINNYPLUS_SKINNY64_DATATYPES_H
#define FORKSKINNYPLUS_SKINNY64_DATATYPES_H

#define FORKSKINNY_ROUNDS_BEFORE 17
#define FORKSKINNY_ROUNDS_AFTER 23
#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_ROUNDS_BEFORE + 2*FORKSKINNY_ROUNDS_AFTER)

#include <cstdint>
#include <immintrin.h>

/** ---- SKINNY64 ---- */
// wrapper so we can return arrays from unslice()
// represent 64 unsliced states
typedef union {
	uint64_t values[64];
} Blocks64_t;

typedef union {
	uint64_t slices[4];
	__m256i simd_cell;
} Cell64_t;

typedef union {
	Cell64_t cols[4];
} Row64_t;

typedef union {
	uint64_t raw[32];
	Cell64_t cells[8];
} HalfState64Sliced_t;

typedef union {
	uint64_t raw[64];
	Cell64_t cells[16];
	Row64_t rows[4];
	HalfState64Sliced_t halves[2];
} State64Sliced_t;

typedef union {
	/** All words of the key keys */
	HalfState64Sliced_t keys[FORKSKINNY64_MAX_ROUNDS];
} KeySchedule64Sliced_t;

typedef struct {
	State64Sliced_t left;
	State64Sliced_t right;
} SlicedCiphertext64_t;

#endif //FORKSKINNYPLUS_SKINNY64_DATATYPES_H
