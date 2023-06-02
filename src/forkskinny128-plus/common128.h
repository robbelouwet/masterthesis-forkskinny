#ifndef FORKSKINNYPLUS_COMMON128_H
#define FORKSKINNYPLUS_COMMON128_H

#include "utils/forkskinny128-datatypes.h"

static inline void xor_row128(Row128_t *a, Row128_t *b, Row128_t *out) {
	#if SEGMENTATION
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

static inline void try_segment128(State128Sliced_t *in, State128Sliced_t *result, const bool segment) {
	if (segment) {
		#if SEGMENTATION
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 8; ++j) {
				result->segments256[i][j] = _mm256_set_epi64x(
						in->raw[(i << 5) + j + 24].value,
						in->raw[(i << 5) + j + 16].value,
						in->raw[(i << 5) + j + 8].value,
						in->raw[(i << 5) + j].value
				);
			}
		}
		#endif
	} else
		for (int i = 0; i < 128; i++)
			result->raw[i].value = in->raw[i].value;
}

static inline void try_unsegment128(State128Sliced_t *in, State128Sliced_t *out,
                                    const bool segmented = SEGMENTATION) {
	if (segmented) {
		#if SEGMENTATION
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 8; ++j) {
				for (int k = 0; k < 4; ++k) {
					out->raw[(i * 32) + (k * 8) + j].value = in->segments256[i][j][k];
				}
			}
		}
		#endif
	} else
		for (int i = 0; i < 128; ++i)
			out->raw[i].value = in->raw[i].value;
}

#endif //FORKSKINNYPLUS_COMMON128_H
