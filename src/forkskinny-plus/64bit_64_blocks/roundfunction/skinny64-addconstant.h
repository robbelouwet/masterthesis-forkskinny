#ifndef FORKSKINNYPLUS_SKINNY64_ADDCONSTANT_H
#define FORKSKINNYPLUS_SKINNY64_ADDCONSTANT_H

#include <cstdint>
#include "immintrin.h"
#include "../utils/skinny64_datatypes.h"
#include "../utils/constants.h"

static inline void skinny64_add_constant(State64Sliced_t *state, uint16_t iteration) {
	// Cell 0 XOR C0
	// TODO: conversion from long long to unsigned? What happens with sign bit with different compilers? Is this dangerous?
	auto packed_c0 = _mm256_set_epi64x(
			precomputed_round_constants[iteration][0],
			precomputed_round_constants[iteration][1],
			precomputed_round_constants[iteration][2],
			precomputed_round_constants[iteration][3]
	);
	state->cells[0].simd_cell = _mm256_xor_si256(state->cells[0].simd_cell, packed_c0);
//	state->cells[0].slices[0] ^= precomputed_round_constants[iteration][0];
//	state->cells[0].slices[1] ^= precomputed_round_constants[iteration][1];
//	state->cells[0].slices[2] ^= precomputed_round_constants[iteration][2];
//	state->cells[0].slices[3] ^= precomputed_round_constants[iteration][3];
	
	// Cell 5 XOR C1
	auto packed_c1 = _mm256_set_epi64x(
			precomputed_round_constants[iteration][4],
			precomputed_round_constants[iteration][5],
			precomputed_round_constants[iteration][6],
			0
	);
	state->cells[5].simd_cell = _mm256_xor_si256(state->cells[5].simd_cell, packed_c1);
//	state->cells[5].slices[0] ^= precomputed_round_constants[iteration][4];
//	state->cells[5].slices[1] ^= precomputed_round_constants[iteration][5];
//	state->cells[5].slices[2] ^= precomputed_round_constants[iteration][6];
	
	// Cell 9 XOR C2 (0x2)
	state->cells[9].slices[1] ^= ONE;
}

#endif //FORKSKINNYPLUS_SKINNY64_ADDCONSTANT_H
