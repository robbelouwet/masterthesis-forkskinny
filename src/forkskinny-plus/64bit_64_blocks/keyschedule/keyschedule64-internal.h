#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H

#include "../utils/skinny64_datatypes.h"

static inline void tk2_lfsr_simd(State64Sliced_t *state, uint8_t n = 8) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (int i = 0; i < n; i++) {
		// 0b10010011 = 2 1 0 3 -> lanes for the simd permutation
		state->cells[i].simd_cell = _mm256_permute4x64_epi64(state->cells[i].simd_cell, 0b10010011);
		state->cells[i].slices[0] ^= state->cells[i].slices[3];
	}
}

static inline void tk3_lfsr_simd(State64Sliced_t *state, uint8_t n = 8) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (int i = 0; i < n; i++) {
		// 0b10010011 = 0 3 2 1 -> lanes for the simd permutation
		state->cells[i].simd_cell = _mm256_permute4x64_epi64(state->cells[i].simd_cell, 0b00111001);
		state->cells[i].slices[3] ^= state->cells[i].slices[0];
	}
}

/**
 * XOR's cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param stop amount of cells to xor, default = 8
 */
static inline State64Sliced_t xor_keys(State64Sliced_t a, State64Sliced_t b, uint8_t stop = 8, uint8_t start = 0) {
	auto res = State64Sliced_t();
	for (int i = start; i < stop; ++i)
		res.cells[i].simd_cell = _mm256_xor_si256(a.cells[i].simd_cell, b.cells[i].simd_cell);
	return res;
}

static inline State64Sliced_t permute(State64Sliced_t values, const uint8_t table[16]) {
	auto res = State64Sliced_t();
	for (int i = 0; i < 16; ++i)
		res.cells[i] = values.cells[table[i]];
	return res;
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H
