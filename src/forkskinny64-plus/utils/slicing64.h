#ifndef FORKSKINNYPLUS_SLICING64_H
#define FORKSKINNYPLUS_SLICING64_H

#include "forkskinny64-datatypes.h"
#include "slicing64-accelerated-internal.h"
#include "slicing64-internal.h"

static inline void slice64(Blocks64_t *blocks, State64Sliced_t *result,
                           const bool segment = AVX2_acceleration) {
	#if FAST_SLICING
	slice_accelerated_internal64(blocks, result, segment);
	#else
	*result = slice_internal(blocks, segment);
	#endif
}

static inline State64Sliced_t slice64(Blocks64_t *blocks,
                                      const bool segment = AVX2_acceleration) {
	#if FAST_SLICING
	return slice_accelerated_internal64(blocks, segment);
	#else
	return slice_internal(blocks, segment);
	#endif
}

static inline void unslice64(State64Sliced_t *state, Blocks64_t *result,
                             const bool segmented = AVX2_acceleration) {
	#if FAST_SLICING
	unslice_accelerated_internal64(state, result, segmented);
	#else
	*result = unslice_internal(state, segmented);
	#endif
}

static inline Blocks64_t unslice64(State64Sliced_t *state,
                                   const bool segmented = AVX2_acceleration) {
	#if FAST_SLICING
	return unslice_accelerated_internal64(state, segmented);
	#else
	return unslice_internal(state, segmented);
	#endif
}

static inline Blocks64_t unslice64(State64Sliced_t state,
                                   const bool segmented = AVX2_acceleration) {
	#if FAST_SLICING
	return unslice_accelerated_internal64(&state, segmented);
	#else
	return unslice_internal(&state, segmented);
	#endif
}

/**
 * This method expands a number into a SlicedCipherState.
 * In other words, if bit n == 1, slice64 n becomes 0xFFFFFFFFFFFFFFFF, same for bit n == 0
 * @param value
 * @param out
 */
static inline void expand64(ULL in, State64Sliced_t *out){
	Slice64_t slices[64];
	for (ULL i = 0; i < 64; ++i) {
		// trick to make it constant time:
		auto res = (((0x1ULL << i) & in) >> i);
		#if slice_size == 512
		slices[i].value = _mm512_set1_epi64((ULL) (((int) 0) - res));
		#elif slice_size == 256
		slices[i].value = _mm256_set1_epi64x((ULL) (((int) 0) - res));
		#elif slice_size == 128
		slices[i].value = _mm_set1_epi64x((ULL) (((int) 0) - res));
		#else
		auto val = (((int) 0) - res);
		slices[i].value = val;
		#endif
	}
	
	#if AVX2_acceleration
	try_segment64(slices, out, true);
	#else
	for (int i = 0; i < 64; ++i) out->raw[i].value = slices[i].value;
	#endif
}

#endif //FORKSKINNYPLUS_SLICING64_H
