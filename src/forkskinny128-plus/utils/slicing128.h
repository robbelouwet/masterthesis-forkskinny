#ifndef FORKSKINNYPLUS_SLICING128_H
#define FORKSKINNYPLUS_SLICING128_H

#include "forkskinny128-datatypes.h"
#include "slicing128-accelerated.h"
#include "slicing128-internal.h"

static inline void slice128(Blocks128_t *blocks, State128Sliced_t *result,
                            const bool segment = AVX2_acceleration) {
	#if FAST_SLICING
	slice_accelerated_internal(blocks, result, segment);
	#else
	slice_internal(blocks, result, segment);
	#endif
}

static inline State128Sliced_t slice128(Blocks128_t *blocks,
                                        const bool segment = AVX2_acceleration) {

	#if FAST_SLICING
	return slice_accelerated_internal(blocks, segment);
	#else
	return slice_internal(blocks, segment);
	#endif
}

static inline void unslice128(State128Sliced_t *state, Blocks128_t *result,
                              const bool segmented = AVX2_acceleration) {
	#if FAST_SLICING
	unslice_accelerated_internal(state, result, segmented);
	#else
	unslice_internal(state, result, segmented);
	#endif
}

static inline Blocks128_t unslice128(State128Sliced_t *state,
                                     const bool segmented = AVX2_acceleration) {
	#if FAST_SLICING
	return unslice_accelerated_internal(state, segmented);
	#else
	return unslice_internal(state, segmented);
	#endif
}

static inline Blocks128_t unslice128(State128Sliced_t state,
                                     const bool segmented = AVX2_acceleration) {
	#if FAST_SLICING
	return unslice_accelerated_internal(&state, segmented);
	#else
	return unslice_internal(&state, segmented);
	#endif
}

#endif //FORKSKINNYPLUS_SLICING128_H
