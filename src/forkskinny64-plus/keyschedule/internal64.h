#ifndef FORKSKINNYPLUS64_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS64_KEYSCHEDULE_INTERNAL_H

//#include <immintrin.h>
#include "../utils/forkskinny64-datatypes.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "../common.h"

static inline void tk2_lfsr(State64Sliced_t *state) {
	// 2 1 0 (3+2)
	#if AVX512_acceleration
	for (int i = 0; i < 4; ++i) {
		state->pairs[i].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b10010011);
		state->cells[2*i].slices[0].value = XOR_SLICE(state->cells[2*i].slices[0].value, state->cells[2*i].slices[3].value);
		state->cells[(2*i)+1].slices[0].value = XOR_SLICE(state->cells[(2*i)+1].slices[0].value, state->cells[(2*i)+1].slices[3].value);
	}
	
	#elif AVX2_acceleration
	for (int i = 0; i < 8; ++i) {
		state->cells[i].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[i].avx2_simd_cell, 0b10010011);
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, state->cells[i].slices[3].value);
	}
	
	#else
	for (int i = 0; i < 8; i++) {
		auto temp = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[0];
		state->cells[i].slices[0].value = XOR_SLICE(temp.value, state->cells[i].slices[3].value);
	}
	#endif
	
}


static inline void tk2_lfsr_full(State64Sliced_t *state) {
	// 2 1 0 (3+2)
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i) {
		state->pairs[i].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[i].avx512_simd_pair, 0b10010011);
		state->cells[i*2].slices[0].value = XOR_SLICE(state->cells[i*2].slices[0].value, state->cells[i*2].slices[3].value);
		state->cells[(i*2)+1].slices[0].value = XOR_SLICE(state->cells[(i*2)+1].slices[0].value, state->cells[(i*2)+1].slices[3].value);
	}
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i) {
		state->cells[i].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[i].avx2_simd_cell, 0b10010011);
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, state->cells[i].slices[3].value);
	}
	
	#else
	for (int i = 0; i < 16; ++i) {
		auto temp = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[0];
		state->cells[i].slices[0].value = XOR_SLICE(temp.value, state->cells[i].slices[3].value);
	}
	#endif
	
}

static inline void tk3_lfsr(State64Sliced_t *state) {
	#if AVX512_acceleration
	
	for (int i = 0; i < 4; ++i) {
		state->pairs[i].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[i].avx512_simd_pair, 0b00111001);
		state->cells[i*2].slices[3].value = XOR_SLICE(state->cells[i*2].slices[3].value, state->cells[i*2].slices[2].value);
		state->cells[(i*2)+1].slices[3].value = XOR_SLICE(state->cells[(i*2)+1].slices[3].value, state->cells[(i*2)+1].slices[2].value);
	}
	
	#elif AVX2_acceleration
	for (int i = 0; i < 8; ++i) {
		state->cells[i].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[i].avx2_simd_cell, 0b00111001);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, state->cells[i].slices[2].value);
	}
	
	#else
	for (int i = 0; i < 8; i++) {
		// 0b00111001 = 0 3 2 1 -> lanes for the simd permutation
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3].value = XOR_SLICE(temp.value, state->cells[i].slices[2].value);
	}
	#endif
}

static inline void tk3_lfsr_full(State64Sliced_t *state) {
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i) {
		state->pairs[i].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[i].avx512_simd_pair, 0b00111001);
		state->cells[i*2].slices[3].value = XOR_SLICE(state->cells[i*2].slices[3].value, state->cells[i*2].slices[2].value);
		state->cells[(i*2)+1].slices[3].value = XOR_SLICE(state->cells[(i*2)+1].slices[3].value, state->cells[(i*2)+1].slices[2].value);
	}
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i) {
		state->cells[i].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[i].avx2_simd_cell, 0b00111001);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, state->cells[i].slices[2].value);
	}
	
	
	#else
	for (int i = 0; i < 8; i++) {
		// 0b00111001 = 0 3 2 1 -> lanes for the simd permutation
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3].value = XOR_SLICE(temp.value, state->cells[i].slices[2].value);
	}
	#endif
}

static inline State64Sliced_t permute(State64Sliced_t input) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xEFCDAB8967452301;
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

/**
 * XOR_AVX2's cells of top 2 rows of 2 keys together, and stores them in res
 * @param a
 * @param b
 */
static inline HalfState64Sliced_t xor_half_keys(HalfState64Sliced_t a, HalfState64Sliced_t b) {
	return {.rows={
			xor_row(a.rows[0], b.rows[0]),
			xor_row(a.rows[1], b.rows[1]),
	}};
}

/**
 * XOR_AVX2's cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param stop amount of cells to xor
 */
static inline State64Sliced_t xor_keys(State64Sliced_t a, State64Sliced_t b) {
	return {.rows={
			xor_row(a.rows[0], b.rows[0]),
			xor_row(a.rows[1], b.rows[1]),
			xor_row(a.rows[2], b.rows[2]),
			xor_row(a.rows[3], b.rows[3]),
	}};
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE_INTERNAL_H
