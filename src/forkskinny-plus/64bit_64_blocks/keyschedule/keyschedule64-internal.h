#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_INTERNAL_H

#include <immintrin.h>
#include "../utils/skinny64_datatypes.h"

static inline void tk2_lfsr(State64Sliced_t *state) {
	// 2 1 0 (3+2)
	#if AVX512_acceleration
	state->pairs[0].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b10010011);
	state->cells[0].slices[0] = XOR_SLICE(state->cells[0].slices[0], state->cells[0].slices[3]);
	state->cells[1].slices[0] = XOR_SLICE(state->cells[1].slices[0], state->cells[1].slices[3]);
	
	state->pairs[1].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b10010011);
	state->cells[2].slices[0] = XOR_SLICE(state->cells[2].slices[0], state->cells[2].slices[3]);
	state->cells[3].slices[0] = XOR_SLICE(state->cells[3].slices[0], state->cells[3].slices[3]);
	
	state->pairs[2].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b10010011);
	state->cells[4].slices[0] = XOR_SLICE(state->cells[4].slices[0], state->cells[4].slices[3]);
	state->cells[5].slices[0] = XOR_SLICE(state->cells[5].slices[0], state->cells[5].slices[3]);
	
	state->pairs[3].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b10010011);
	state->cells[6].slices[0] = XOR_SLICE(state->cells[6].slices[0], state->cells[6].slices[3]);
	state->cells[7].slices[0] = XOR_SLICE(state->cells[7].slices[0], state->cells[7].slices[3]);
	
	#elif AVX2_acceleration
	state->cells[0].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b10010011);
	state->cells[0].slices[0].value = XOR_SLICE(state->cells[0].slices[0].value, state->cells[0].slices[3].value);
	
	state->cells[1].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[1].avx2_simd_cell, 0b10010011);
	state->cells[1].slices[0].value = XOR_SLICE(state->cells[1].slices[0].value, state->cells[1].slices[3].value);

	state->cells[2].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[2].avx2_simd_cell, 0b10010011);
	state->cells[2].slices[0].value = XOR_SLICE(state->cells[2].slices[0].value, state->cells[2].slices[3].value);

	state->cells[3].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[3].avx2_simd_cell, 0b10010011);
	state->cells[3].slices[0].value = XOR_SLICE(state->cells[3].slices[0].value, state->cells[3].slices[3].value);

	state->cells[4].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[4].avx2_simd_cell, 0b10010011);
	state->cells[4].slices[0].value = XOR_SLICE(state->cells[4].slices[0].value, state->cells[4].slices[3].value);

	state->cells[5].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[5].avx2_simd_cell, 0b10010011);
	state->cells[5].slices[0].value = XOR_SLICE(state->cells[5].slices[0].value, state->cells[5].slices[3].value);

	state->cells[6].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[6].avx2_simd_cell, 0b10010011);
	state->cells[6].slices[0].value = XOR_SLICE(state->cells[6].slices[0].value, state->cells[6].slices[3].value);

	state->cells[7].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[7].avx2_simd_cell, 0b10010011);
	state->cells[7].slices[0].value = XOR_SLICE(state->cells[7].slices[0].value, state->cells[7].slices[3].value);
	
	
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

static inline void tk3_lfsr(State64Sliced_t *state, uint8_t n = 8) {
	#if AVX512_acceleration
	state->pairs[0].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[0].slices[3] = XOR_SLICE(state->cells[0].slices[3], state->cells[0].slices[2]);
	state->cells[1].slices[3] = XOR_SLICE(state->cells[1].slices[3], state->cells[1].slices[2]);
	
	state->pairs[1].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[2].slices[3] = XOR_SLICE(state->cells[2].slices[3], state->cells[2].slices[2]);
	state->cells[3].slices[3] = XOR_SLICE(state->cells[3].slices[3], state->cells[3].slices[2]);
	
	state->pairs[2].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[4].slices[3] = XOR_SLICE(state->cells[4].slices[3], state->cells[4].slices[2]);
	state->cells[5].slices[3] = XOR_SLICE(state->cells[5].slices[3], state->cells[5].slices[2]);
	
	state->pairs[3].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[6].slices[3] = XOR_SLICE(state->cells[6].slices[3], state->cells[6].slices[2]);
	state->cells[7].slices[3] = XOR_SLICE(state->cells[7].slices[3], state->cells[7].slices[2]);
	
	#elif AVX2_acceleration
	state->cells[0].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[0].slices[3].value = XOR_SLICE(state->cells[0].slices[3].value, state->cells[0].slices[0].value);
	
	state->cells[1].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[1].slices[3].value = XOR_SLICE(state->cells[1].slices[3].value, state->cells[1].slices[0].value);
	
	state->cells[2].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[2].slices[3].value = XOR_SLICE(state->cells[2].slices[3].value, state->cells[2].slices[0].value);
	
	state->cells[3].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[3].slices[3].value = XOR_SLICE(state->cells[3].slices[3].value, state->cells[3].slices[0].value);
	
	state->cells[4].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[4].slices[3].value = XOR_SLICE(state->cells[4].slices[3].value, state->cells[4].slices[0].value);
	
	state->cells[5].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[5].slices[3].value = XOR_SLICE(state->cells[5].slices[3].value, state->cells[5].slices[0].value);
	
	state->cells[6].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[6].slices[3].value = XOR_SLICE(state->cells[6].slices[3].value, state->cells[6].slices[0].value);
	
	state->cells[7].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[7].slices[3].value = XOR_SLICE(state->cells[7].slices[3].value, state->cells[7].slices[0].value);
	
	#else
	for (int i = 0; i < n; i++) {
		// 0b00111001 = 0 3 2 1 -> lanes for the simd permutation
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3].value = XOR_SLICE(temp.value, state->cells[i].slices[2].value);
	}
	#endif
}

/**
 * XOR's cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param stop amount of cells to xor, default = 8
 */
static inline State64Sliced_t xor_keys(State64Sliced_t a, State64Sliced_t b) {
	auto res = State64Sliced_t();
	
	#if AVX512_acceleration
	for (int i = 0; i < 4; ++i)
		res.pairs[i].avx512_simd_pair = _mm512_xor_si512(a.pairs[i].avx512_simd_pair, b.pairs[i].avx512_simd_pair);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 8; ++i)
		res.cells[i].avx2_simd_cell = _mm256_xor_si256(a.cells[i].avx2_simd_cell, b.cells[i].avx2_simd_cell);
		
	#else
	for (int i = 0; i < 8; ++i) {
		res.cells[i].slices[0].value = XOR_SLICE(a.cells[i].slices[0].value, b.cells[i].slices[0].value);
		res.cells[i].slices[1].value = XOR_SLICE(a.cells[i].slices[1].value, b.cells[i].slices[1].value);
		res.cells[i].slices[2].value = XOR_SLICE(a.cells[i].slices[2].value, b.cells[i].slices[2].value);
		res.cells[i].slices[3].value = XOR_SLICE(a.cells[i].slices[3].value, b.cells[i].slices[3].value);
	}
	#endif
	
	return res;
}

static inline State64Sliced_t permute(State64Sliced_t input) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xFEDCBA9876543210;
//	input = slice_t(test_blocks);
	
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
