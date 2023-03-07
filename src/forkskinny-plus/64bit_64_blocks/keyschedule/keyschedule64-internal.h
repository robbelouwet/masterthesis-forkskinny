#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H

#include "../utils/skinny64_datatypes.h"

static inline void tk2_lfsr_simd(State64Sliced_t *state, uint8_t n = 8) {
	for (int i = 0; i < n; i++) {
		// 0b10010011 = 2 1 0 3 -> lanes for the simd permutation
		state->cells[i].simd_cell = _mm256_permute4x64_epi64(state->cells[i].simd_cell, 0b10010011);
		state->cells[i].slices[0] ^= state->cells[i].slices[3];
	}
}

static inline void tk3_lfsr_simd(State64Sliced_t *state, uint8_t n = 8) {
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

static inline State64Sliced_t permute(State64Sliced_t input) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xFEDCBA9876543210;
//	input = slice(test_blocks);
	
	auto output = State64Sliced_t();
	
	output.halves[1] = input.halves[0];
	
	output.cells[0] = input.cells[0xE];
	output.cells[1] = input.cells[0x8];
	output.cells[2] = input.cells[0xC];
	output.cells[3] = input.cells[0x9];
	output.cells[4] = input.cells[0xF];
	output.cells[5] = input.cells[0xB];
	output.cells[6] = input.cells[0xA];
	output.cells[7] = input.cells[0xD];
	
	// Input:   0x FEDC BA98 7654 3210
	
	// Erik:    0x 7654 3210 DABF 9C8E
	// Us:      0x 7654 3210 DABF 9C8E
	
//	auto test_output = unslice(output).values[0].raw;
	return output;
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H
