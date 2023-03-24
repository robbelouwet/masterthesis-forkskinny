#ifndef FORKSKINNYPLUS_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS_KEYSCHEDULE_INTERNAL_H

#include <immintrin.h>
#include "../utils/forkskinny-datatypes.h"

static inline void tk2_lfsr(StateSliced_t *state) {
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
	#endif
	
}

static inline void tk3_lfsr(StateSliced_t *state) {
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
	#endif
}

static inline StateSliced_t permute(StateSliced_t input) {
//	auto test_blocks = Blocks_t();
//	test_blocks.values[0].raw = 0xFEDCBA9876543210;
//	input = slice_t(test_blocks);
	
	auto output = StateSliced_t();
	
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

#endif //FORKSKINNYPLUS_KEYSCHEDULE_INTERNAL_H
