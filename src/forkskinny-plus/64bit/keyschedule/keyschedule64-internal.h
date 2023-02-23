#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H

#include "../forkskinny64-plus.h"

static inline void tk2_lfsr_simd(State64Sliced_t *state) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (auto &cell: state->cells) {
		// 0b10010011 = 2 1 0 3 -> lanes for the simd permutation
		cell.simd_cell = _mm256_permute4x64_epi64(cell.simd_cell, 0b10010011);
		cell.slices[0] ^= cell.slices[3];
	}
}

static inline void tk3_lfsr_simd(State64Sliced_t *state) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (auto &cell: state->cells) {
		// 0b10010011 = 0 3 2 1 -> lanes for the simd permutation
		cell.simd_cell = _mm256_permute4x64_epi64(cell.simd_cell, 0b00111001);
		cell.slices[3] ^= cell.slices[0];
	}
}

/**
 * XOR's the first stop cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param stop amount of cells to xor, default = 8
 */
static inline State64Sliced_t xor_key(State64Sliced_t a, State64Sliced_t b, uint8_t stop = 8, uint8_t start = 0) {
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
