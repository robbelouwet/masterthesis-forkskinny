#ifndef FORKSKINNYPLUS64_FIXSLICING_INTERNAL_H
#define FORKSKINNYPLUS64_FIXSLICING_INTERNAL_H

#include "../../utils/forkskinny64-datatypes.h"

static inline void fs_tk2_lfsr(State64Sliced_t *state) {
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
	for (auto &cell: state->cells) {
		auto temp = cell.slices[3];
		cell.slices[3] = cell.slices[2];
		cell.slices[2] = cell.slices[1];
		cell.slices[1] = cell.slices[0];
		cell.slices[0].value = XOR_SLICE(temp.value, cell.slices[3].value);
	}
	#endif
	
}

static inline void fs_tk3_lfsr(State64Sliced_t *state) {
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i) {
		state->pairs[i].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[i].avx512_simd_pair, 0b00111001);
		state->cells[i*2].slices[3].value = XOR_SLICE(state->cells[i*2].slices[3].value, state->cells[i*2].slices[2].value);
		state->cells[(i*2)+1].slices[3].value = XOR_SLICE(state->cells[(i*2)+1].slices[3].value, state->cells[(i*2)+1].slices[2].value);
	}
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i) {
		state->cells[i].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[i].avx2_simd_cell, 0b00111001);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, state->cells[i].slices[0].value);
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

#endif //FORKSKINNYPLUS_FIXSLICING_INTERNAL_H
