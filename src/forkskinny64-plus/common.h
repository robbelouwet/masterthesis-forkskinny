#ifndef FORKSKINNYPLUS_COMMON_H
#define FORKSKINNYPLUS_COMMON_H

#include "utils/forkskinny64-datatypes.h"

static void inline xor_segmented_row(uint8_t a, uint8_t b, uint8_t out_i, State64Sliced_t *state) {
	state->segments256[out_i][0] = XOR256(state->segments256[a][0], state->segments256[b][0]);
	state->segments256[out_i][1] = XOR256(state->segments256[a][1], state->segments256[b][1]);
	state->segments256[out_i][2] = XOR256(state->segments256[a][2], state->segments256[b][2]);
	state->segments256[out_i][3] = XOR256(state->segments256[a][3], state->segments256[b][3]);
}

static inline void xor_row(Row64_t *a, Row64_t *b, Row64_t *out) {
	
	// even though the key schedule might be segmented, you can add rows ike this as if they weren't segmented
	// (it just so happens the memory layout of the union struct allows this exception)
	#if AVX2_acceleration || AVX512_acceleration
	out->cols[0].avx2_simd_cell = _mm256_xor_si256(a->cols[0].avx2_simd_cell, b->cols[0].avx2_simd_cell);
	out->cols[1].avx2_simd_cell = _mm256_xor_si256(a->cols[1].avx2_simd_cell, b->cols[1].avx2_simd_cell);
	out->cols[2].avx2_simd_cell = _mm256_xor_si256(a->cols[2].avx2_simd_cell, b->cols[2].avx2_simd_cell);
	out->cols[3].avx2_simd_cell = _mm256_xor_si256(a->cols[3].avx2_simd_cell, b->cols[3].avx2_simd_cell);
	
	#else
	for (int i = 0; i < 4; ++i) {
		out->cols[i].slices[0].value = XOR_SLICE(a->cols[i].slices[0].value, b->cols[i].slices[0].value);
		out->cols[i].slices[1].value = XOR_SLICE(a->cols[i].slices[1].value, b->cols[i].slices[1].value);
		out->cols[i].slices[2].value = XOR_SLICE(a->cols[i].slices[2].value, b->cols[i].slices[2].value);
		out->cols[i].slices[3].value = XOR_SLICE(a->cols[i].slices[3].value, b->cols[i].slices[3].value);
	}
	#endif
}

#endif //FORKSKINNYPLUS_COMMON_H
