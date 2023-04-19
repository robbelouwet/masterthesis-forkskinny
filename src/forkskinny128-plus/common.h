#ifndef FORKSKINNYPLUS_COMMON_H
#define FORKSKINNYPLUS_COMMON_H

#include "utils/forkskinny128-datatypes.h"

//#define LOAD_2 _mm256_load_si256
//#define XOR_2 _mm256_xor_si256
//#define STORE_2 _mm256_store_si256
//#define COL_2(state, col, i) (state.cols[col].avx2_simd_cells + i)
//
//#define LOAD_512 _mm256_load_si256
//#define XOR_512 _mm256_xor_si256
//#define STORE_512 _mm256_store_si256
//#define COL_512(state, col) &(state.cols[col].avx512_simd_cell)

static inline Row128_t xor_row(Row128_t a, Row128_t b) {
	auto res = Row128_t();
	
	#if AVX512_acceleration
	STORE_512(COL_512(a, 0), XOR_512(LOAD_512(COL_512(a, 0)), LOAD_512(COL_512(b, 0)))); // cell 0
	STORE_512(COL_512(a, 1), XOR_512(LOAD_512(COL_512(a, 1)), LOAD_512(COL_512(b, 1)))); // cell 1
	STORE_512(COL_512(a, 2), XOR_512(LOAD_512(COL_512(a, 2)), LOAD_512(COL_512(b, 2)))); // cell 2
	STORE_512(COL_512(a, 3), XOR_512(LOAD_512(COL_512(a, 3)), LOAD_512(COL_512(b, 3)))); // cell 3
	
//	res.cols[0].avx512_simd_cell = _mm512_xor_si512(a.cols[0].avx512_simd_cell, b.cols[0].avx512_simd_cell);
//	res.cols[1].avx512_simd_cell = _mm512_xor_si512(a.cols[1].avx512_simd_cell, b.cols[1].avx512_simd_cell);
//	res.cols[2].avx512_simd_cell = _mm512_xor_si512(a.cols[2].avx512_simd_cell, b.cols[2].avx512_simd_cell);
//	res.cols[3].avx512_simd_cell = _mm512_xor_si512(a.cols[3].avx512_simd_cell, b.cols[3].avx512_simd_cell);
	
	#elif AVX2_acceleration
//	STORE_2(COL_2(res, 0, 0), XOR_2(LOAD_2(COL_2(a, 0, 0)), LOAD_2(COL_2(b, 0, 0)))); // cell 0, lower half
//	STORE_2(COL_2(res, 0, 1), XOR_2(LOAD_2(COL_2(a, 0, 1)), LOAD_2(COL_2(b, 0, 1)))); // cell 0, upper half
//	STORE_2(COL_2(res, 1, 0), XOR_2(LOAD_2(COL_2(a, 1, 0)), LOAD_2(COL_2(b, 1, 0)))); // cell 1, lower half
//	STORE_2(COL_2(res, 1, 1), XOR_2(LOAD_2(COL_2(a, 1, 1)), LOAD_2(COL_2(b, 1, 1)))); // cell 1, upper half
//	STORE_2(COL_2(res, 2, 0), XOR_2(LOAD_2(COL_2(a, 2, 0)), LOAD_2(COL_2(b, 2, 0)))); // ...
//	STORE_2(COL_2(res, 2, 1), XOR_2(LOAD_2(COL_2(a, 2, 1)), LOAD_2(COL_2(b, 2, 1))));
//	STORE_2(COL_2(res, 3, 0), XOR_2(LOAD_2(COL_2(a, 3, 0)), LOAD_2(COL_2(b, 3, 0))));
//	STORE_2(COL_2(res, 3, 1), XOR_2(LOAD_2(COL_2(a, 3, 1)), LOAD_2(COL_2(b, 3, 1))));
	res.cols[0].avx2_simd_cells[0] = _mm256_xor_si256(a.cols[0].avx2_simd_cells[0], b.cols[0].avx2_simd_cells[0]);
	res.cols[0].avx2_simd_cells[1] = _mm256_xor_si256(a.cols[0].avx2_simd_cells[1], b.cols[0].avx2_simd_cells[1]);
	res.cols[1].avx2_simd_cells[0] = _mm256_xor_si256(a.cols[1].avx2_simd_cells[0], b.cols[1].avx2_simd_cells[0]);
	res.cols[1].avx2_simd_cells[1] = _mm256_xor_si256(a.cols[1].avx2_simd_cells[1], b.cols[1].avx2_simd_cells[1]);
	res.cols[2].avx2_simd_cells[0] = _mm256_xor_si256(a.cols[2].avx2_simd_cells[0], b.cols[2].avx2_simd_cells[0]);
	res.cols[2].avx2_simd_cells[1] = _mm256_xor_si256(a.cols[2].avx2_simd_cells[1], b.cols[2].avx2_simd_cells[1]);
	res.cols[3].avx2_simd_cells[0] = _mm256_xor_si256(a.cols[3].avx2_simd_cells[0], b.cols[3].avx2_simd_cells[0]);
	res.cols[3].avx2_simd_cells[1] = _mm256_xor_si256(a.cols[3].avx2_simd_cells[1], b.cols[3].avx2_simd_cells[1]);
	
	#else
	for (int i = 0; i < 4; ++i) {
		res.cols[i].slices[0].value = XOR_SLICE(a.cols[i].slices[0].value, b.cols[i].slices[0].value);
		res.cols[i].slices[1].value = XOR_SLICE(a.cols[i].slices[1].value, b.cols[i].slices[1].value);
		res.cols[i].slices[2].value = XOR_SLICE(a.cols[i].slices[2].value, b.cols[i].slices[2].value);
		res.cols[i].slices[3].value = XOR_SLICE(a.cols[i].slices[3].value, b.cols[i].slices[3].value);
		res.cols[i].slices[4].value = XOR_SLICE(a.cols[i].slices[4].value, b.cols[i].slices[4].value);
		res.cols[i].slices[5].value = XOR_SLICE(a.cols[i].slices[5].value, b.cols[i].slices[5].value);
		res.cols[i].slices[6].value = XOR_SLICE(a.cols[i].slices[6].value, b.cols[i].slices[6].value);
		res.cols[i].slices[7].value = XOR_SLICE(a.cols[i].slices[7].value, b.cols[i].slices[7].value);
	}
	#endif
	
	return res;
}

#endif //FORKSKINNYPLUS_COMMON_H
