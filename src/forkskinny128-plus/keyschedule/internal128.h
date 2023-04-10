#ifndef FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H

#include <immintrin.h>
#include "../utils/forkskinny128-datatypes.h"

static inline void tk2_lfsr(State128Sliced_t *state) {
	// 2 1 0 (3+2)
	
	for (int i = 0; i < 8; i++) {
		auto temp = state->cells[i].slices[7];
		state->cells[i].slices[7] = state->cells[i].slices[6];
		state->cells[i].slices[6] = state->cells[i].slices[5];
		state->cells[i].slices[5] = state->cells[i].slices[4];
		state->cells[i].slices[4] = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[0];
		state->cells[i].slices[0].value = XOR_SLICE(temp.value, state->cells[i].slices[6].value);
	}
}

static inline void tk2_lfsr_full(State128Sliced_t *state) {
	for (int i = 0; i < 16; i++) {
		auto temp = state->cells[i].slices[7];
		state->cells[i].slices[7] = state->cells[i].slices[6];
		state->cells[i].slices[6] = state->cells[i].slices[5];
		state->cells[i].slices[5] = state->cells[i].slices[4];
		state->cells[i].slices[4] = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[0];
		state->cells[i].slices[0].value = XOR_SLICE(temp.value, state->cells[i].slices[6].value);
	}
}

static inline void tk3_lfsr(State128Sliced_t *state) {
	for (int i = 0; i < 8; i++) {
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[4];
		state->cells[i].slices[4] = state->cells[i].slices[5];
		state->cells[i].slices[5] = state->cells[i].slices[6];
		state->cells[i].slices[6] = state->cells[i].slices[7];
		state->cells[i].slices[7].value = XOR_SLICE(temp.value, state->cells[i].slices[5].value);
	}
}

static inline void tk3_lfsr_full(State128Sliced_t *state) {
	for (int i = 0; i < 16; i++) {
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[4];
		state->cells[i].slices[4] = state->cells[i].slices[5];
		state->cells[i].slices[5] = state->cells[i].slices[6];
		state->cells[i].slices[6] = state->cells[i].slices[7];
		state->cells[i].slices[7].value = XOR_SLICE(temp.value, state->cells[i].slices[5].value);
	}
}

static inline State128Sliced_t permute(State128Sliced_t input) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw[0] = 0x7766554433221100;
//	test_blocks.values[0].raw[1] = 0xffeeddccbbaa9988;
//	input = slice(test_blocks);
	
	auto output = State128Sliced_t();
	
	output.halves[1] = input.halves[0];
	
	output.cells[0] = input.cells[0x9];
	output.cells[1] = input.cells[0xF];
	output.cells[2] = input.cells[0x8];
	output.cells[3] = input.cells[0xD];
	output.cells[4] = input.cells[0xA];
	output.cells[5] = input.cells[0xE];
	output.cells[6] = input.cells[0xC];
	output.cells[7] = input.cells[0xB];
	
	// Input:   0x FEDC BA98 7654 3210
	
	// Erik:    0x 7654 3210 DABF 9C8E
	// Us:      0x 7654 3210 DABF 9C8E

//	auto res = unslice(output).values[0];
//	auto test_output0 = res.raw[0];
//	auto test_output1 = res.raw[1];
	return output;
}

/**
 * XOR's cells of top 2 rows of 2 keys together, and stores them in res
 * @param a
 * @param b
 */
static inline HalfState128Sliced_t xor_half_keys(HalfState128Sliced_t a, HalfState128Sliced_t b) {
	auto res = HalfState128Sliced_t();
	
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i)
		res.cells[i].avx512_simd_cell = _mm512_xor_si512(a.cells[i].avx512_simd_cell, b.cells[i].avx512_simd_cell);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 8; ++i){
		res.cells[i].avx2_simd_cells[0] = _mm256_xor_si256(a.cells[i].avx2_simd_cells[0], b.cells[i].avx2_simd_cells[0]);
		res.cells[i].avx2_simd_cells[1] = _mm256_xor_si256(a.cells[i].avx2_simd_cells[1], b.cells[i].avx2_simd_cells[1]);
	}
	
	#else
	for (int i = 0; i < 8; ++i) {
		res.cells[i].slices[0].value = XOR_SLICE(a.cells[i].slices[0].value, b.cells[i].slices[0].value);
		res.cells[i].slices[1].value = XOR_SLICE(a.cells[i].slices[1].value, b.cells[i].slices[1].value);
		res.cells[i].slices[2].value = XOR_SLICE(a.cells[i].slices[2].value, b.cells[i].slices[2].value);
		res.cells[i].slices[3].value = XOR_SLICE(a.cells[i].slices[3].value, b.cells[i].slices[3].value);
		res.cells[i].slices[4].value = XOR_SLICE(a.cells[i].slices[4].value, b.cells[i].slices[4].value);
		res.cells[i].slices[5].value = XOR_SLICE(a.cells[i].slices[5].value, b.cells[i].slices[5].value);
		res.cells[i].slices[6].value = XOR_SLICE(a.cells[i].slices[6].value, b.cells[i].slices[6].value);
		res.cells[i].slices[7].value = XOR_SLICE(a.cells[i].slices[7].value, b.cells[i].slices[7].value);
	}
	#endif
	
	return res;
}

/**
 * XOR's cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param stop amount of cells to xor
 */
static inline State128Sliced_t xor_keys(State128Sliced_t a, State128Sliced_t b) {
	auto res = State128Sliced_t();
	
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i)
		res.cells[i].avx512_simd_cell = _mm512_xor_si512(a.cells[i].avx512_simd_cell, b.cells[i].avx512_simd_cell);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i){
		res.cells[i].avx2_simd_cells[0] = _mm256_xor_si256(a.cells[i].avx2_simd_cells[0], b.cells[i].avx2_simd_cells[0]);
		res.cells[i].avx2_simd_cells[1] = _mm256_xor_si256(a.cells[i].avx2_simd_cells[1], b.cells[i].avx2_simd_cells[1]);
		
	}
	
	#else
	for (int i = 0; i < 16; ++i) {
		res.cells[i].slices[0].value = XOR_SLICE(a.cells[i].slices[0].value, b.cells[i].slices[0].value);
		res.cells[i].slices[1].value = XOR_SLICE(a.cells[i].slices[1].value, b.cells[i].slices[1].value);
		res.cells[i].slices[2].value = XOR_SLICE(a.cells[i].slices[2].value, b.cells[i].slices[2].value);
		res.cells[i].slices[3].value = XOR_SLICE(a.cells[i].slices[3].value, b.cells[i].slices[3].value);
		res.cells[i].slices[4].value = XOR_SLICE(a.cells[i].slices[4].value, b.cells[i].slices[4].value);
		res.cells[i].slices[5].value = XOR_SLICE(a.cells[i].slices[5].value, b.cells[i].slices[5].value);
		res.cells[i].slices[6].value = XOR_SLICE(a.cells[i].slices[6].value, b.cells[i].slices[6].value);
		res.cells[i].slices[7].value = XOR_SLICE(a.cells[i].slices[7].value, b.cells[i].slices[7].value);
	}
	#endif
	
	return res;
}
	
	#endif //FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H
