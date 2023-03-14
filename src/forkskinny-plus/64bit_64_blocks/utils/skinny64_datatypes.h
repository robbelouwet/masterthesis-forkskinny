#ifndef FORKSKINNYPLUS_SKINNY64_DATATYPES_H
#define FORKSKINNYPLUS_SKINNY64_DATATYPES_H

#define FORKSKINNY_ROUNDS_BEFORE 17
#define FORKSKINNY_ROUNDS_AFTER 23
#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_ROUNDS_BEFORE + 2*FORKSKINNY_ROUNDS_AFTER)

#include <cstdint>
#include "config.h"
#include "immintrin.h"

/** ---- SKINNY64 ---- */
// wrapper so we can return arrays from unslice()
// represent 64 unsliced states
typedef union {
	slice raw;
	unsigned char bytes[bytes_per_slice];
} Slice64_t;

typedef union {
	uint64_t values[slice_size];
} Blocks64_t;

typedef union {
	slice slices[4];
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
	#ifdef segment
	segment segments[segment_amount];
	#endif
	Row64_t rows[4];
	HalfState64Sliced_t halves[2];
} State64Sliced_t;

typedef union {
	/** All words of the key keys */
	HalfState64Sliced_t keys[FORKSKINNY64_MAX_ROUNDS];
} KeySchedule64Sliced_t;

typedef struct {
	State64Sliced_t C1;  // <- branch with branch constant
	State64Sliced_t C0;
	State64Sliced_t M;
} SlicedCiphertext_t;

#endif //FORKSKINNYPLUS_SKINNY64_DATATYPES_H
