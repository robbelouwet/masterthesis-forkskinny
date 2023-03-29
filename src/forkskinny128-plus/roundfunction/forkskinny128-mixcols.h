#ifndef FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H
#define FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H

#include "../utils/forkskinny128-datatypes.h"

static inline Row128_t xor_row(Row128_t a, Row128_t b) {
	auto res = Row128_t();
	
	#if AVX512_acceleration
	res.pairs[0].avx512_simd_pair = _mm512_xor_si512(a.pairs[0].avx512_simd_pair, b.pairs[0].avx512_simd_pair);
	res.pairs[1].avx512_simd_pair = _mm512_xor_si512(a.pairs[1].avx512_simd_pair, b.pairs[1].avx512_simd_pair);
	
	#elif AVX2_acceleration
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
	}
	#endif
	
	return res;
}

static inline void forkskinny128_mixcols(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw = 0x55557555B6988DDF;
//	*state = slice(test_blocks);
	
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);
	state->rows[2] = xor_row(state->rows[2], state->rows[0]);
	
	auto temp = xor_row(state->rows[3], state->rows[2]);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
	
	// 0xF88AC3CD8DDFADDF
//	auto test_res = unslice(*state).values[0].raw;
//	int appel = 1;
}

static inline void forkskinny128_mixcols_inv(State128Sliced_t *state) {
//	auto ct = Blocks128_t{.values = {0xF88AC3CD8DDFADDF}};
//	*state = slice_t(ct);
	
	auto temp = state->rows[3];
	state->rows[3] = state->rows[0];
	state->rows[0] = state->rows[1];
	state->rows[1] = state->rows[2];
	
	
	state->rows[3] = xor_row(state->rows[3], temp);
	state->rows[2] = xor_row(temp, state->rows[0]);
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);

//	auto test_res = unslice(*state).values[0].raw;
//	int appel = 1;
}

#endif //FORKSKINNYPLUS_FORKSKINNY_MIXCOLS_H
