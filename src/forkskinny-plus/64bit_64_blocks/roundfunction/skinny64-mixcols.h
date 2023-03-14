#ifndef FORKSKINNYPLUS_SKINNY64_MIXCOLS_H
#define FORKSKINNYPLUS_SKINNY64_MIXCOLS_H

#include "../utils/skinny64_datatypes.h"

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
		res.cols[i].slices[0] = XOR_SLICE(a.cols[i].slices[0], b.cols[i].slices[0]);
		res.cols[i].slices[1] = XOR_SLICE(a.cols[i].slices[1], b.cols[i].slices[1]);
		res.cols[i].slices[2] = XOR_SLICE(a.cols[i].slices[2], b.cols[i].slices[2]);
		res.cols[i].slices[3] = XOR_SLICE(a.cols[i].slices[3], b.cols[i].slices[3]);
	}
	#endif
	
	return res;
}

static inline void skinny64_mixcols(State64Sliced_t *state) {
//	auto test_blocks = Blocks64_t();
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

static inline void skinny64_mixcols_inv(State64Sliced_t *state) {
//	auto ct = Blocks64_t{.values = {0xF88AC3CD8DDFADDF}};
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

#endif //FORKSKINNYPLUS_SKINNY64_MIXCOLS_H
