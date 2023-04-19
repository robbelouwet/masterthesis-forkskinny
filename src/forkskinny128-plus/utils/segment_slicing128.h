#ifndef FORKSKINNYPLUS_SEGMENT_SLICING128_H
#define FORKSKINNYPLUS_SEGMENT_SLICING128_H

#include <immintrin.h>
#include "forkskinny128-datatypes.h"

/**
 * Segment slicing is the concept of grouping together multiple slices into a single SIMD lane, or as I call it, a 'segment'.
 * Whereas a slice is a grouping of all equally significant bits, a segment is a grouping of multiple slices where
 * each slice of such a group is an equally significant slice of a cell.
 *
 * In other words, a SlicedState64_t cipher state may look like this:
 * cell 0: [A, B, C, D], cell 1: [E, F, G, H], cell 2: [I, J, K, L], cell 3: [M, N, O, P]
 * where every cell contains 4 uint64_t slices (4 letters).
 *
 * A Segment then takes 4 sequential cells, and groups equally significant slices AGAIN into a segment:
 * segment 0: [A, E, I, M], segment 1: [B, F, J, N], ...
 */

#if AVX512_acceleration // don't compile if not supported, otherwise optimizer -O3 gives errors!
#define SLICE_SEGMENT_AVX512(state, first, significance){       \
    _mm512_set_epi64(                                           \
        state->cells[first+7].slices[significance].value,         \
        state->cells[first+6].slices[significance].value,       \
        state->cells[first+5].slices[significance].value,       \
        state->cells[first+4].slices[significance].value,       \
        state->cells[first+3].slices[significance].value,       \
        state->cells[first+2].slices[significance].value,       \
        state->cells[first+1].slices[significance].value,       \
        state->cells[first].slices[significance].value       \
    )                                                           \
}

#define UNSLICE_SEGMENT_AVX512(segment, first, significance, state_ptr){      \
    state->cells[first].slices[significance].value = segment.slices[0];       \
    state->cells[first+1].slices[significance].value = segment[1];     \
    state->cells[first+2].slices[significance].value = segment[2];     \
    state->cells[first+3].slices[significance].value = segment[3];     \
    state->cells[first+4].slices[significance].value = segment[4];     \
    state->cells[first+5].slices[significance].value = segment[5];     \
    state->cells[first+6].slices[significance].value = segment[6];     \
    state->cells[first+7].slices[significance].value = segment[7];     \
    }
#endif
#if AVX2_acceleration
#define SLICE_SEGMENT_AVX2(state_ptr, first, significance){         \
    _mm256_set_epi64x(                                              \
        state_ptr->cells[first+3].slices[significance].value,         \
        state_ptr->cells[first+2].slices[significance].value,       \
        state_ptr->cells[first+1].slices[significance].value,       \
        state_ptr->cells[first].slices[significance].value        \
    )                                                               \
}

#define UNSLICE_SEGMENT_AVX2(segment, first, significance, state_ptr){ \
    state_ptr->cells[first].slices[significance].value = segment[0];       \
    state_ptr->cells[first+1].slices[significance].value = segment[1];     \
    state_ptr->cells[first+2].slices[significance].value = segment[2];     \
    state_ptr->cells[first+3].slices[significance].value = segment[3];     \
    }
#endif

#endif //FORKSKINNYPLUS_SEGMENT_SLICING128_H
