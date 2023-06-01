#ifndef FORKSKINNYPLUS_SLICING128_ACCELERATED_H
#define FORKSKINNYPLUS_SLICING128_ACCELERATED_H

#include "forkskinny128-datatypes.h"
#include "../../forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "../../forkskinny64-plus/utils/slicing64.h"
#include "../common.h"

static inline void unpack(Blocks128_t *state, Block64_t *out) {
	for (int i = 0; i < slice_size; i++) {
		out[i].raw = state->values[i].raw[0];
		out[slice_size + i].raw = state->values[i].raw[1];
	}
}

static inline void slice_accelerated_internal(Blocks128_t *state, State128Sliced_t *out,
                                              const bool segment = AVX2_acceleration) {
	/// Unpack the lower and upper halves of all 128-bit blocks together
	Block64_t buf[slice_size << 1];
	unpack(state, buf);
	
	/// Slice the upper and lower halves of every block separately
	State64Sliced_t res64_low;
	slice64((Blocks64_t *) buf, &res64_low, false);
	
	State64Sliced_t res64_high;
	slice64((Blocks64_t *) &(buf[slice_size]), &res64_high, false);
	
	/// Re-group the slices
	State128Sliced_t temp;
	int i = 0;
	for (; i < 64; ++i) temp.raw[i].value = res64_low.raw[i].value;
	for (; i < 128; i++) temp.raw[i].value = res64_high.raw[i & 63].value;
	
	/// Segment
	try_segment128(&temp, out, segment);
}

static inline State128Sliced_t slice_accelerated_internal(Blocks128_t *state,
                                                          const bool segment = AVX2_acceleration) {
	State128Sliced_t res;
	slice_accelerated_internal(state, &res, segment);
	return res;
}

static inline void unslice_accelerated_internal(State128Sliced_t *state, Blocks128_t *out,
                                                const bool segmented = AVX2_acceleration) {
	/// Unsegment
	State128Sliced_t unsegmented;
	try_unsegment128(state, &unsegmented, segmented);
	
	/// Unslice lower and upper halves
	Blocks64_t unsliced64_low;
	unslice64((State64Sliced_t *) &unsegmented, &unsliced64_low, false);
	
	Blocks64_t unsliced64_high;
	unslice64((State64Sliced_t *) &(unsegmented.raw[64]), &unsliced64_high, false);
	
	/// Pack blocks back together
	for (int j = 0; j < slice_size; ++j) {
		out->values[j].raw[0] = unsliced64_low.values[j].raw;
		out->values[j].raw[1] = unsliced64_high.values[j].raw;
	}
}

static inline Blocks128_t unslice_accelerated_internal(State128Sliced_t *state,
                                                       const bool segmented = AVX2_acceleration) {
	Blocks128_t res;
	unslice_accelerated_internal(state, &res, segmented);
	return res;
}

#endif //FORKSKINNYPLUS_SLICING128_ACCELERATED_H
