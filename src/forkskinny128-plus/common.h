#ifndef FORKSKINNYPLUS_COMMON_H
#define FORKSKINNYPLUS_COMMON_H

#include "utils/forkskinny128-datatypes.h"


static inline void xor_row(Row128_t *a, Row128_t *b, Row128_t *out) {
	#if AVX2_acceleration
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
