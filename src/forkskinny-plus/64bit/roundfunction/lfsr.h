#ifndef FORKSKINNYPLUS_LFSR_H
#define FORKSKINNYPLUS_LFSR_H

#include "../forkskinny64-plus.h"
#include <immintrin.h>

/*static inline void tk2_lfsr_simd(State64Sliced_t *state) {
	for (int i = 0; i < 8; ++i) {
		// 0b10010011 = 2 1 0 3 -> lanes for the simd permutation
		state->cells[i].simd_cell = _mm256_permute4x64_epi64(state->cells[i].simd_cell, 0b10010011);
		state->cells[i].slices[0] ^= state->cells[i].slices[3];
	}
}*/

#endif //FORKSKINNYPLUS_LFSR_H
