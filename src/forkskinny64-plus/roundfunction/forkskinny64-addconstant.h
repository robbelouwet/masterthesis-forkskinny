#ifndef FORKSKINNYPLUS64_FORKSKINNY_ADDCONSTANT_H
#define FORKSKINNYPLUS64_FORKSKINNY_ADDCONSTANT_H

#include <cstdint>
#include "../utils/forkskinny64-datatypes.h"
#include "../../constants.h"

static inline void forkskinny64_add_constant(HalfState64Sliced_t *state, uint16_t iteration) {
	// The beauty of unions:
	
	#if AVX2_acceleration || AVX512_acceleration
	// Cell 0 XOR C0
	Cell64_t C0 = {.slices = {
			forkskinny_precomputed_round_constants[iteration][0],
			forkskinny_precomputed_round_constants[iteration][1],
			forkskinny_precomputed_round_constants[iteration][2],
			forkskinny_precomputed_round_constants[iteration][3]
	}};
	state->cells[1].avx2_simd_cell = _mm256_xor_si256(state->cells[1].avx2_simd_cell, C0.avx2_simd_cell);
	
	// Cell 4 XOR C1
	Cell64_t C1 = {.slices = {
			forkskinny_precomputed_round_constants[iteration][4],
			forkskinny_precomputed_round_constants[iteration][5],
			forkskinny_precomputed_round_constants[iteration][6],
			0
	}};
	state->cells[5].avx2_simd_cell = _mm256_xor_si256(state->cells[5].avx2_simd_cell, C1.avx2_simd_cell);
	#else
	// Cell 0 XOR C0
	state->cells[1].slices[0].value = XOR_SLICE(state->cells[1].slices[0].value, forkskinny64_precomputed_round_constants[iteration][0]);
	state->cells[1].slices[1].value = XOR_SLICE(state->cells[1].slices[1].value, forkskinny64_precomputed_round_constants[iteration][1]);
	state->cells[1].slices[2].value = XOR_SLICE(state->cells[1].slices[2].value, forkskinny64_precomputed_round_constants[iteration][2]);
	state->cells[1].slices[3].value = XOR_SLICE(state->cells[1].slices[3].value, forkskinny64_precomputed_round_constants[iteration][3]);
	
	// Cell 4 XOR C1
	state->cells[5].slices[0].value = XOR_SLICE(state->cells[5].slices[0].value, forkskinny64_precomputed_round_constants[iteration][4]);
	state->cells[5].slices[1].value = XOR_SLICE(state->cells[5].slices[1].value, forkskinny64_precomputed_round_constants[iteration][5]);
	state->cells[5].slices[2].value = XOR_SLICE(state->cells[5].slices[2].value, forkskinny64_precomputed_round_constants[iteration][6]);
	#endif
	
	// only for TK2 and TK3
	state->raw[13].value = XOR_SLICE(state->raw[13].value, ONE);
	
	//auto unsliced_res = unslice(*state).values[0].raw;
	//int appel = 1;
	
//	 Cell 8 XOR 0x2, aka Slice64_t 1 of cell 8
//	state->cells[9].slices[1] ^= ONE;
}

#endif //FORKSKINNYPLUS_FORKSKINNY_ADDCONSTANT_H
