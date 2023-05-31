#ifndef FORKSKINNYPLUS_COMMON_H
#define FORKSKINNYPLUS_COMMON_H

#include "utils/forkskinny64-datatypes.h"
#if AVX2_acceleration
static void inline xor_segmented_row(uint8_t a, uint8_t b, uint8_t out_i, State64Sliced_t *state) {
	for (int i = 0; i < 4; ++i)
		state->segments256[out_i][i] = XOR256(state->segments256[a][i], state->segments256[b][i]);
}
#endif

static inline void xor_row(Row64_t *a, Row64_t *b, Row64_t *out) {
	#if AVX2_acceleration
	for (int i = 0; i < 4; ++i)
		out->segments[i] = XOR256(a->segments[i], b->segments[i]);
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
