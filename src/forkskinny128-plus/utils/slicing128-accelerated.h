#ifndef FORKSKINNYPLUS_SLICING128_ACCELERATED_H
#define FORKSKINNYPLUS_SLICING128_ACCELERATED_H

#include "forkskinny128-datatypes.h"
#include "../../forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "../../forkskinny64-plus/utils/slicing64.h"
#include "../common.h"

static inline void unpack128(Blocks128_t *state, Block64_t *out) {
	for (int i = 0; i < slice_size; i++) {
		out[i].raw = state->values[i].raw[0];
		out[slice_size + i].raw = state->values[i].raw[1];
	}
}

#if AVX2_acceleration

#include "immintrin.h"

static inline void double_slice128_accelerated_internal(Blocks128_t *state, State128Sliced_t *out) {
	Slice128_t slices[128];
	Block128_t b_blocks[128];
	
	for (int i = 64; i < 128; i++) {
		b_blocks[i].lane = ROL128(state->values[i & 63].lane, (i & 63));
		int appel = 1;
	}
	
	for (int i = 0; i < 64; i++) {
		auto ind = 64 - i;
		
		// slice |= blocks[ind + j].raw & bit_masks[j];
		// A 128-bit lane that packs both halves of a block
		// results in (Slice n, Slice n+64)
		__m128i pack = _mm_setzero_si128();
		for (int j = 0; j < 64; ++j)
			STORE128(&pack,
			         OR128(
					         pack,
					         AND128(LOAD128(b_blocks + ind + j),
					                LOAD128(double_bit_masks + j))
			         )
			);
		
		b_blocks[ind - 1] = b_blocks[ind + 63];
		
		auto realigned = ROR128(pack, i);
		slices[i].value = realigned[0];
		slices[i + 64].value = realigned[1];
		
		int appel = 1;
	}
	
	try_segment128((State128Sliced_t *) &slices, out, true);
}

#endif


static inline void slice128_accelerated_internal(Blocks128_t *state, State128Sliced_t *out,
                                                 const bool segment = AVX2_acceleration) {
	#if AVX2_acceleration
	double_slice128_accelerated_internal(state, out);
	return;
	#endif
	/// Unpack the lower and upper halves of all 128-bit blocks together
	Block64_t buf[slice_size << 1];
	unpack128(state, buf);
	
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

static inline State128Sliced_t slice128_accelerated_internal(Blocks128_t *state,
                                                             const bool segment = AVX2_acceleration) {
	State128Sliced_t res;
	slice128_accelerated_internal(state, &res, segment);
	return res;
}

static inline void unslice128_accelerated_internal(State128Sliced_t *state, Blocks128_t *out,
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

static inline Blocks128_t unslice128_accelerated_internal(State128Sliced_t *state,
                                                          const bool segmented = AVX2_acceleration) {
	Blocks128_t res;
	unslice128_accelerated_internal(state, &res, segmented);
	return res;
}

#endif //FORKSKINNYPLUS_SLICING128_ACCELERATED_H
