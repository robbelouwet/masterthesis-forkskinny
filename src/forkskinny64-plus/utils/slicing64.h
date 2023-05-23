#ifndef FORKSKINNYPLUS_SLICING64_H
#define FORKSKINNYPLUS_SLICING64_H

#include "forkskinny64-datatypes.h"
#include "slicing64-accelerated-internal.h"

static inline void slice(Blocks64_t *blocks, State64Sliced_t *result,
                         const bool segment = (AVX2_acceleration || AVX512_acceleration)) {
	#if FAST_SLICING
	slice_accelerated_internal(blocks, result, segment);
	#else
	*result = slice_internal(blocks);
	#endif
}

static inline State64Sliced_t slice(Blocks64_t *blocks,
                                    const bool segment = (AVX2_acceleration || AVX512_acceleration)) {
	#if FAST_SLICING
	return slice_accelerated_internal(blocks, segment);
	#else
	return slice_internal(blocks);
	#endif
}

static inline void unslice(State64Sliced_t *state, Blocks64_t *result,
                           const bool segmented = (AVX2_acceleration || AVX512_acceleration)) {
	#if FAST_SLICING
	unslice_accelerated_internal(state, result, segmented);
	#else
	*result = unslice_internal(state, segmented);
	#endif
}

static inline Blocks64_t unslice(State64Sliced_t *state,
                                 const bool segmented = (AVX2_acceleration || AVX512_acceleration)) {
	#if FAST_SLICING
	return unslice_accelerated_internal(state, segmented);
	#else
	return unslice_internal(state, segmented);
	#endif
}

static inline Blocks64_t unslice(State64Sliced_t state,
                                 const bool segmented = (AVX2_acceleration || AVX512_acceleration)) {
	#if FAST_SLICING
	return unslice_accelerated_internal(&state, segmented);
	#else
	return unslice_internal(&state, segmented);
	#endif
}

#endif //FORKSKINNYPLUS_SLICING64_H
