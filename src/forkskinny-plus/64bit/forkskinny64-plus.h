#ifndef FORKSKINNYPLUS_FORKSKINNY64_PLUS_H
#define FORKSKINNYPLUS_FORKSKINNY64_PLUS_H

#define FORKSKINNY64_BLOCK_SIZE 8
#define FORKSKINNY_64_192_ROUNDS_BEFORE 17
#define FORKSKINNY_64_192_ROUNDS_AFTER 23
#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_64_192_ROUNDS_BEFORE + 2*FORKSKINNY_64_192_ROUNDS_AFTER)

#include <cstdint>
#include <immintrin.h>

/** ---- SKINNY64 ---- */
// wrapper so we can return arrays from unslice()
// represent 64 unsliced states
typedef union {
	uint64_t values[64];
} Blocks64;

typedef union {
	uint64_t slices[4];
	__m256i simd_cell;
} Cell64;

typedef union {
	uint64_t raw[32];
	Cell64 cells[8];
} HalfState64Sliced_t;

typedef union {
	uint64_t raw[64];
	Cell64 cells[16];
	HalfState64Sliced_t halves[2];
} State64Sliced_t;

typedef struct {
	/** All words of the key keys */
	HalfState64Sliced_t keys[FORKSKINNY64_MAX_ROUNDS];
} KeySchedule64Sliced_t;

static void inline encrypt_round(HalfState64Sliced_t round_key, State64Sliced_t state){
	
	// S-BOX
	for (int i = 0; i < 16; ++i) {
	
	}
}
#endif //FORKSKINNYPLUS_FORKSKINNY64_PLUS_H
