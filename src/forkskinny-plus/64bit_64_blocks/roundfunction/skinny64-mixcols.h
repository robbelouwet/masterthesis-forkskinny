#ifndef FORKSKINNYPLUS_SKINNY64_MIXCOLS_H
#define FORKSKINNYPLUS_SKINNY64_MIXCOLS_H

#include "../utils/skinny64_datatypes.h"

static inline Row64_t xor_row(Row64_t a, Row64_t b) {
	auto res = Row64_t();
	for (int i = 0; i < 4; ++i)
		res.cols[i].simd_cell = _mm256_xor_si256(a.cols[i].simd_cell, b.cols[i].simd_cell);
	
	return res;
}

static inline void skinny64_mixcols(State64Sliced_t *state) {
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);
	state->rows[2] = xor_row(state->rows[2], state->rows[0]);
	
	auto temp = xor_row(state->rows[3], state->rows[2]);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
}

#endif //FORKSKINNYPLUS_SKINNY64_MIXCOLS_H
