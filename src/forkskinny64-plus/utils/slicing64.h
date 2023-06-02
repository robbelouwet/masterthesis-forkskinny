#ifndef FORKSKINNYPLUS_SLICING64_H
#define FORKSKINNYPLUS_SLICING64_H

#include "forkskinny64-datatypes.h"
#include "slicing64-accelerated-internal.h"
#include "slicing64-internal.h"

static inline void slice64(Blocks64_t *blocks, State64Sliced_t *result,
                           const bool segment = SEGMENTATION) {
	#if FAST_SLICING
	slice_accelerated_internal64(blocks, result, segment);
	#else
	*result = slice_internal64(blocks, segment);
	#endif
}

static inline State64Sliced_t slice64(Blocks64_t *blocks,
                                      const bool segment = SEGMENTATION) {
	#if FAST_SLICING
	return slice_accelerated_internal64(blocks, segment);
	#else
	return slice_internal64(blocks, segment);
	#endif
}

static inline void unslice64(State64Sliced_t *state, Blocks64_t *result,
                             const bool segmented = SEGMENTATION) {
	#if FAST_SLICING
	unslice_accelerated_internal64(state, result, segmented);
	#else
	*result = unslice_internal64(state, segmented);
	#endif
}

static inline Blocks64_t unslice64(State64Sliced_t *state,
                                   const bool segmented = SEGMENTATION) {
	#if FAST_SLICING
	return unslice_accelerated_internal64(state, segmented);
	#else
	return unslice_internal64(state, segmented);
	#endif
}

static inline Blocks64_t unslice64(State64Sliced_t state,
                                   const bool segmented = SEGMENTATION) {
	#if FAST_SLICING
	return unslice_accelerated_internal64(&state, segmented);
	#else
	return unslice_internal64(&state, segmented);
	#endif
}

static inline u64 expand64_64bit(u64 bit){
	u64 result = 0;
	
	result |= bit | (bit << 1);
	result |= result | (result << 2);
	result |= result | (result << 4);
	result |= result | (result << 8);
	result |= result | (result << 16);
	result |= result | (result << 32);
	
	return result;
}

/**
 * This method expands a number into a SlicedCipherState.
 * In other words, if bit n == 1, slice64 n becomes 0xFFFFFFFFFFFFFFFF, same for bit n == 0
 * @param value
 * @param out
 */
static inline void expand64(u64 in, State64Sliced_t *out){
	Slice64_t slices[64];
	for (u64 i = 0; i < 64; ++i) {
		// trick to make it constant time:
		auto expanded = expand64_64bit(in);
		#if slice_size == 512
		slices[i].value = _mm512_set1_epi64(expanded);
		#elif slice_size == 256
		slices[i].value = _mm256_set1_epi64x(expanded);
		#elif slice_size == 128
		slices[i].value = _mm_set1_epi64x(expanded);
		#else
		slices[i].value = expanded;
		#endif
	}
	
	#if SEGMENTATION
	try_segment64(slices, out, true);
	#else
	for (int i = 0; i < 64; ++i) out->raw[i].value = slices[i].value;
	#endif
}

#endif //FORKSKINNYPLUS_SLICING64_H
