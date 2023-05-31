#ifndef FORKSKINNYPLUS_COMMON_H
#define FORKSKINNYPLUS_COMMON_H

#include "utils/forkskinny128-datatypes.h"

#define x0 state->cells[i].slices[0].value
#define x1 state->cells[i].slices[1].value
#define x2 state->cells[i].slices[2].value
#define x3 state->cells[i].slices[3].value
#define x4 state->cells[i].slices[4].value
#define x5 state->cells[i].slices[5].value
#define x6 state->cells[i].slices[6].value
#define x7 state->cells[i].slices[7].value

#define s0 state->segments256[i][0]
#define s1 state->segments256[i][1]
#define s2 state->segments256[i][2]
#define s3 state->segments256[i][3]
#define s4 state->segments256[i][4]
#define s5 state->segments256[i][5]
#define s6 state->segments256[i][6]
#define s7 state->segments256[i][7]

static inline void xor_row(Row128_t *a, Row128_t *b, Row128_t *out) {
	#if AVX2_acceleration
	for (int i = 0; i < 8; ++i)
		out->segments[i] = XOR256(a->segments[i], b->segments[i]);
	#else
	for (int i = 0; i < 4; ++i) {
		out->cols[i].slices[0].value = XOR_SLICE(a->cols[i].slices[0].value, b->cols[i].slices[0].value);
		out->cols[i].slices[1].value = XOR_SLICE(a->cols[i].slices[1].value, b->cols[i].slices[1].value);
		out->cols[i].slices[2].value = XOR_SLICE(a->cols[i].slices[2].value, b->cols[i].slices[2].value);
		out->cols[i].slices[3].value = XOR_SLICE(a->cols[i].slices[3].value, b->cols[i].slices[3].value);
		out->cols[i].slices[4].value = XOR_SLICE(a->cols[i].slices[4].value, b->cols[i].slices[4].value);
		out->cols[i].slices[5].value = XOR_SLICE(a->cols[i].slices[5].value, b->cols[i].slices[5].value);
		out->cols[i].slices[6].value = XOR_SLICE(a->cols[i].slices[6].value, b->cols[i].slices[6].value);
		out->cols[i].slices[7].value = XOR_SLICE(a->cols[i].slices[7].value, b->cols[i].slices[7].value);
	}
	#endif
}

#endif //FORKSKINNYPLUS_COMMON_H
