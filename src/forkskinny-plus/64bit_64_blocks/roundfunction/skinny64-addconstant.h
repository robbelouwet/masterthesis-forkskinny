#ifndef FORKSKINNYPLUS_SKINNY64_ADDCONSTANT_H
#define FORKSKINNYPLUS_SKINNY64_ADDCONSTANT_H

#include <cstdint>
#include "immintrin.h"
#include "../utils/skinny64_datatypes.h"
#include "../utils/constants.h"

static inline void skinny64_add_constant(State64Sliced_t *state, uint16_t iteration) {
	// The beauty of unions:
	// Cell 0 XOR C0
	Cell64_t C0 = {.slices = {
			precomputed_round_constants[iteration][0],
			precomputed_round_constants[iteration][1],
			precomputed_round_constants[iteration][2],
			precomputed_round_constants[iteration][3]
	}};
	state->cells[0].simd_cell = _mm256_xor_si256(state->cells[0].simd_cell, C0.simd_cell);
	
	// Cell 4 XOR C1
	Cell64_t C1 = {.slices = {
			precomputed_round_constants[iteration][4],
			precomputed_round_constants[iteration][5],
			precomputed_round_constants[iteration][6],
			0
	}};
	state->cells[4].simd_cell = _mm256_xor_si256(state->cells[4].simd_cell, C1.simd_cell);
	
	// Cell 8 XOR 0x2, aka slice 1 of cell 8
	state->cells[8].slices[1] ^= ONE;
}

#endif //FORKSKINNYPLUS_SKINNY64_ADDCONSTANT_H
