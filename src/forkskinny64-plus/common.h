#ifndef FORKSKINNYPLUS_COMMON_H
#define FORKSKINNYPLUS_COMMON_H

#include "utils/forkskinny64-datatypes.h"

static inline Row64_t xor_row(Row64_t a, Row64_t b) {
	auto res = Row64_t();
	
	#if AVX512_acceleration
	res.pairs[0].avx512_simd_pair = _mm512_xor_si512(a.pairs[0].avx512_simd_pair, b.pairs[0].avx512_simd_pair);
	res.pairs[1].avx512_simd_pair = _mm512_xor_si512(a.pairs[1].avx512_simd_pair, b.pairs[1].avx512_simd_pair);
	
	#elif AVX2_acceleration
	res.cols[0].avx2_simd_cell = _mm256_xor_si256(a.cols[0].avx2_simd_cell, b.cols[0].avx2_simd_cell);
	res.cols[1].avx2_simd_cell = _mm256_xor_si256(a.cols[1].avx2_simd_cell, b.cols[1].avx2_simd_cell);
	res.cols[2].avx2_simd_cell = _mm256_xor_si256(a.cols[2].avx2_simd_cell, b.cols[2].avx2_simd_cell);
	res.cols[3].avx2_simd_cell = _mm256_xor_si256(a.cols[3].avx2_simd_cell, b.cols[3].avx2_simd_cell);
	
	#else
	for (int i = 0; i < 4; ++i) {
		res.cols[i].slices[0].value = XOR_SLICE(a.cols[i].slices[0].value, b.cols[i].slices[0].value);
		res.cols[i].slices[1].value = XOR_SLICE(a.cols[i].slices[1].value, b.cols[i].slices[1].value);
		res.cols[i].slices[2].value = XOR_SLICE(a.cols[i].slices[2].value, b.cols[i].slices[2].value);
		res.cols[i].slices[3].value = XOR_SLICE(a.cols[i].slices[3].value, b.cols[i].slices[3].value);
	}
	#endif
	
	return res;
}

#endif //FORKSKINNYPLUS_COMMON_H
