#ifndef FORKSKINNYPLUS64_COMMON_H
#define FORKSKINNYPLUS64_COMMON_H

#include "../utils/forkskinny64-datatypes.h"

/**
 * XOR's cells of top 2 rows of 2 keys together, and stores them in res
 * @param a
 * @param b
 */
static inline HalfState64Sliced_t xor_half_keys(HalfState64Sliced_t a, HalfState64Sliced_t b) {
	auto res = HalfState64Sliced_t();
	
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

/**
 * XOR's cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param stop amount of cells to xor
 */
static inline State64Sliced_t xor_keys(State64Sliced_t a, State64Sliced_t b) {
	auto res = State64Sliced_t();
	
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i)
		res.pairs[i].avx512_simd_pair = _mm512_xor_si512(a.pairs[i].avx512_simd_pair, b.pairs[i].avx512_simd_pair);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i)
		res.cells[i].avx2_simd_cell = _mm256_xor_si256(a.cells[i].avx2_simd_cell, b.cells[i].avx2_simd_cell);
	
	#else
	for (int i = 0; i < 16; ++i) {
		res.cells[i].slices[0].value = XOR_SLICE(a.cells[i].slices[0].value, b.cells[i].slices[0].value);
		res.cells[i].slices[1].value = XOR_SLICE(a.cells[i].slices[1].value, b.cells[i].slices[1].value);
		res.cells[i].slices[2].value = XOR_SLICE(a.cells[i].slices[2].value, b.cells[i].slices[2].value);
		res.cells[i].slices[3].value = XOR_SLICE(a.cells[i].slices[3].value, b.cells[i].slices[3].value);
	}
	#endif
	
	return res;
}

#endif //FORKSKINNYPLUS_COMMON_H
