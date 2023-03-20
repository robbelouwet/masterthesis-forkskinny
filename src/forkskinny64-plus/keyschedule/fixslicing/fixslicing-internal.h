#ifndef FORKSKINNYPLUS_FIXSLICING_INTERNAL_H
#define FORKSKINNYPLUS_FIXSLICING_INTERNAL_H

#include "../../utils/skinny64_datatypes.h"

static inline void tk2_lfsr(State64Sliced_t *state) {
	// 2 1 0 (3+2)
	#if AVX512_acceleration
	state->pairs[0].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b10010011);
	state->cells[0].slices[0].value = XOR_SLICE(state->cells[0].slices[0].value, state->cells[0].slices[3].value);
	state->cells[1].slices[0].value = XOR_SLICE(state->cells[1].slices[0].value, state->cells[1].slices[3].value);
	
	state->pairs[1].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[1].avx512_simd_pair, 0b10010011);
	state->cells[2].slices[0].value = XOR_SLICE(state->cells[2].slices[0].value, state->cells[2].slices[3].value);
	state->cells[3].slices[0].value = XOR_SLICE(state->cells[3].slices[0].value, state->cells[3].slices[3].value);
	
	state->pairs[2].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[2].avx512_simd_pair, 0b10010011);
	state->cells[4].slices[0].value = XOR_SLICE(state->cells[4].slices[0].value, state->cells[4].slices[3].value);
	state->cells[5].slices[0].value = XOR_SLICE(state->cells[5].slices[0].value, state->cells[5].slices[3].value);
	
	state->pairs[3].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[3].avx512_simd_pair, 0b10010011);
	state->cells[6].slices[0].value = XOR_SLICE(state->cells[6].slices[0].value, state->cells[6].slices[3].value);
	state->cells[7].slices[0].value = XOR_SLICE(state->cells[7].slices[0].value, state->cells[7].slices[3].value),
	
	state->pairs[4].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[4].avx512_simd_pair, 0b10010011);
	state->cells[8].slices[0].value = XOR_SLICE(state->cells[8].slices[0].value, state->cells[8].slices[3].value);
	state->cells[9].slices[0].value = XOR_SLICE(state->cells[9].slices[0].value, state->cells[9].slices[3].value);
	
	state->pairs[5].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[5].avx512_simd_pair, 0b10010011);
	state->cells[0xa].slices[0].value = XOR_SLICE(state->cells[0xa].slices[0].value, state->cells[0xa].slices[3].value);
	state->cells[0xb].slices[0].value = XOR_SLICE(state->cells[0xb].slices[0].value, state->cells[0xb].slices[3].value);
	
	state->pairs[6].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[6].avx512_simd_pair, 0b10010011);
	state->cells[0xc].slices[0].value = XOR_SLICE(state->cells[0xc].slices[0].value, state->cells[0xc].slices[3].value);
	state->cells[0xd].slices[0].value = XOR_SLICE(state->cells[0xd].slices[0].value, state->cells[0xd].slices[3].value);
	
	state->pairs[7].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[7].avx512_simd_pair, 0b10010011);
	state->cells[0xe].slices[0].value = XOR_SLICE(state->cells[0xe].slices[0].value, state->cells[0xe].slices[3].value);
	state->cells[0xf].slices[0].value = XOR_SLICE(state->cells[0xf].slices[0].value, state->cells[0xf].slices[3].value);
	
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
	
	state->cells[8].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[8].avx2_simd_cell, 0b10010011);
	state->cells[8].slices[0].value = XOR_SLICE(state->cells[8].slices[0].value, state->cells[8].slices[3].value);
	
	state->cells[9].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[9].avx2_simd_cell, 0b10010011);
	state->cells[9].slices[0].value = XOR_SLICE(state->cells[9].slices[0].value, state->cells[9].slices[3].value);

	state->cells[0xa].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xa].avx2_simd_cell, 0b10010011);
	state->cells[0xa].slices[0].value = XOR_SLICE(state->cells[0xa].slices[0].value, state->cells[0xa].slices[3].value);

	state->cells[0xb].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xb].avx2_simd_cell, 0b10010011);
	state->cells[0xb].slices[0].value = XOR_SLICE(state->cells[0xb].slices[0].value, state->cells[0xb].slices[3].value);

	state->cells[0xc].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xc].avx2_simd_cell, 0b10010011);
	state->cells[0xc].slices[0].value = XOR_SLICE(state->cells[0xc].slices[0].value, state->cells[0xc].slices[3].value);

	state->cells[0xd].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xd].avx2_simd_cell, 0b10010011);
	state->cells[0xd].slices[0].value = XOR_SLICE(state->cells[0xd].slices[0].value, state->cells[0xd].slices[3].value);

	state->cells[0xe].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xe].avx2_simd_cell, 0b10010011);
	state->cells[0xe].slices[0].value = XOR_SLICE(state->cells[0xe].slices[0].value, state->cells[0xe].slices[3].value);

	state->cells[0xf].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xf].avx2_simd_cell, 0b10010011);
	state->cells[0xf].slices[0].value = XOR_SLICE(state->cells[0xf].slices[0].value, state->cells[0xf].slices[3].value);
	
	
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

static inline void tk3_lfsr(State64Sliced_t *state) {
	#if AVX512_acceleration
	state->pairs[0].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[0].slices[3].value = XOR_SLICE(state->cells[0].slices[3].value, state->cells[0].slices[2].value);
	state->cells[1].slices[3].value = XOR_SLICE(state->cells[1].slices[3].value, state->cells[1].slices[2].value);
	
	state->pairs[1].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[2].slices[3].value = XOR_SLICE(state->cells[2].slices[3].value, state->cells[2].slices[2].value);
	state->cells[3].slices[3].value = XOR_SLICE(state->cells[3].slices[3].value, state->cells[3].slices[2].value);
	
	state->pairs[2].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[4].slices[3].value = XOR_SLICE(state->cells[4].slices[3].value, state->cells[4].slices[2].value);
	state->cells[5].slices[3].value = XOR_SLICE(state->cells[5].slices[3].value, state->cells[5].slices[2].value);
	
	state->pairs[3].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[0].avx512_simd_pair, 0b00111001);
	state->cells[6].slices[3].value = XOR_SLICE(state->cells[6].slices[3].value, state->cells[6].slices[2].value);
	state->cells[7].slices[3].value = XOR_SLICE(state->cells[7].slices[3].value, state->cells[7].slices[2].value);
	
	state->pairs[4].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[4].avx512_simd_pair, 0b00111001);
	state->cells[8].slices[3].value = XOR_SLICE(state->cells[8].slices[3].value, state->cells[8].slices[2].value);
	state->cells[9].slices[3].value = XOR_SLICE(state->cells[9].slices[3].value, state->cells[9].slices[2].value);
	
	state->pairs[5].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[5].avx512_simd_pair, 0b00111001);
	state->cells[0xa].slices[3].value = XOR_SLICE(state->cells[0xa].slices[3].value, state->cells[0xa].slices[2].value);
	state->cells[0xb].slices[3].value = XOR_SLICE(state->cells[0xb].slices[3].value, state->cells[0xb].slices[2].value);
	
	state->pairs[6].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[6].avx512_simd_pair, 0b00111001);
	state->cells[0xc].slices[3].value = XOR_SLICE(state->cells[0xc].slices[3].value, state->cells[0xc].slices[2].value);
	state->cells[0xd].slices[3].value = XOR_SLICE(state->cells[0xd].slices[3].value, state->cells[0xd].slices[2].value);
	
	state->pairs[7].avx512_simd_pair = _mm512_permutex_epi64(state->pairs[7].avx512_simd_pair, 0b00111001);
	state->cells[0xe].slices[3].value = XOR_SLICE(state->cells[0xe].slices[3].value, state->cells[0xe].slices[2].value);
	state->cells[0xf].slices[3].value = XOR_SLICE(state->cells[0xf].slices[3].value, state->cells[0xf].slices[2].value);
	
	#elif AVX2_acceleration
	state->cells[0].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0].avx2_simd_cell, 0b00111001);
	state->cells[0].slices[3].value = XOR_SLICE(state->cells[0].slices[3].value, state->cells[0].slices[0].value);
	
	state->cells[1].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[1].avx2_simd_cell, 0b00111001);
	state->cells[1].slices[3].value = XOR_SLICE(state->cells[1].slices[3].value, state->cells[1].slices[0].value);
	
	state->cells[2].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[2].avx2_simd_cell, 0b00111001);
	state->cells[2].slices[3].value = XOR_SLICE(state->cells[2].slices[3].value, state->cells[2].slices[0].value);
	
	state->cells[3].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[3].avx2_simd_cell, 0b00111001);
	state->cells[3].slices[3].value = XOR_SLICE(state->cells[3].slices[3].value, state->cells[3].slices[0].value);
	
	state->cells[4].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[4].avx2_simd_cell, 0b00111001);
	state->cells[4].slices[3].value = XOR_SLICE(state->cells[4].slices[3].value, state->cells[4].slices[0].value);
	
	state->cells[5].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[5].avx2_simd_cell, 0b00111001);
	state->cells[5].slices[3].value = XOR_SLICE(state->cells[5].slices[3].value, state->cells[5].slices[0].value);
	
	state->cells[6].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[6].avx2_simd_cell, 0b00111001);
	state->cells[6].slices[3].value = XOR_SLICE(state->cells[6].slices[3].value, state->cells[6].slices[0].value);
	
	state->cells[7].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[7].avx2_simd_cell, 0b00111001);
	state->cells[7].slices[3].value = XOR_SLICE(state->cells[7].slices[3].value, state->cells[7].slices[0].value);
	
	state->cells[8].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[8].avx2_simd_cell, 0b00111001);
	state->cells[8].slices[3].value = XOR_SLICE(state->cells[8].slices[3].value, state->cells[8].slices[0].value);
	
	state->cells[9].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[9].avx2_simd_cell, 0b00111001);
	state->cells[9].slices[3].value = XOR_SLICE(state->cells[9].slices[3].value, state->cells[9].slices[0].value);
	
	state->cells[0xa].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xa].avx2_simd_cell, 0b00111001);
	state->cells[0xa].slices[3].value = XOR_SLICE(state->cells[0xa].slices[3].value, state->cells[0xa].slices[0].value);
	
	state->cells[0xb].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xb].avx2_simd_cell, 0b00111001);
	state->cells[0xb].slices[3].value = XOR_SLICE(state->cells[0xb].slices[3].value, state->cells[0xb].slices[0].value);
	
	state->cells[0xc].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xc].avx2_simd_cell, 0b00111001);
	state->cells[0xc].slices[3].value = XOR_SLICE(state->cells[0xc].slices[3].value, state->cells[0xc].slices[0].value);
	
	state->cells[0xd].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xd].avx2_simd_cell, 0b00111001);
	state->cells[0xd].slices[3].value = XOR_SLICE(state->cells[0xd].slices[3].value, state->cells[0xd].slices[0].value);
	
	state->cells[0xe].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xe].avx2_simd_cell, 0b00111001);
	state->cells[0xe].slices[3].value = XOR_SLICE(state->cells[0xe].slices[3].value, state->cells[0xe].slices[0].value);
	
	state->cells[0xf].avx2_simd_cell = _mm256_permute4x64_epi64(state->cells[0xf].avx2_simd_cell, 0b00111001);
	state->cells[0xf].slices[3].value = XOR_SLICE(state->cells[0xf].slices[3].value, state->cells[0xf].slices[0].value);
	
	
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

#endif //FORKSKINNYPLUS_FIXSLICING_INTERNAL_H
