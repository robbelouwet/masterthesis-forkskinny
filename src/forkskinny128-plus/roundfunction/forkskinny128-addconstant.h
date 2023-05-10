#ifndef FORKSKINNYPLUS128_FORKSKINNY_ADDCONSTANT_H
#define FORKSKINNYPLUS128_FORKSKINNY_ADDCONSTANT_H

#include <cstdint>
#include "../utils/forkskinny128-datatypes.h"
#include "../../constants.h"

static inline void forkskinny128_add_constant(HalfState128Sliced_t *state, uint16_t iteration) {
	#if AVX2_acceleration || AVX512_acceleration
	// Cell 0 XOR_AVX2 C0
	Cell128_t C0 = {.slices = {
			forkskinny_precomputed_round_constants[iteration][0],
			forkskinny_precomputed_round_constants[iteration][1],
			forkskinny_precomputed_round_constants[iteration][2],
			forkskinny_precomputed_round_constants[iteration][3]
	}};
	state->cells[0].avx2_simd_cells[0] = _mm256_xor_si256(state->cells[0].avx2_simd_cells[0], C0.avx2_simd_cells[0]);
	
	// Cell 4 XOR_AVX2 C1
	Cell128_t C1 = {.slices = {
			forkskinny_precomputed_round_constants[iteration][4],
			forkskinny_precomputed_round_constants[iteration][5],
			forkskinny_precomputed_round_constants[iteration][6],
			0
	}};
	state->cells[4].avx2_simd_cells[0] = _mm256_xor_si256(state->cells[4].avx2_simd_cells[0], C1.avx2_simd_cells[0]);
	#else
	// Cell 0 XOR_AVX2 C0
	state->cells[0].slices[0].value = XOR_SLICE(state->cells[0].slices[0].value, forkskinny_precomputed_round_constants[iteration][0]);
	state->cells[0].slices[1].value = XOR_SLICE(state->cells[0].slices[1].value, forkskinny_precomputed_round_constants[iteration][1]);
	state->cells[0].slices[2].value = XOR_SLICE(state->cells[0].slices[2].value, forkskinny_precomputed_round_constants[iteration][2]);
	state->cells[0].slices[3].value = XOR_SLICE(state->cells[0].slices[3].value, forkskinny_precomputed_round_constants[iteration][3]);

	// Cell 4 XOR_AVX2 C1
	state->cells[4].slices[0].value = XOR_SLICE(state->cells[4].slices[0].value, forkskinny_precomputed_round_constants[iteration][4]);
	state->cells[4].slices[1].value = XOR_SLICE(state->cells[4].slices[1].value, forkskinny_precomputed_round_constants[iteration][5]);
	state->cells[4].slices[2].value = XOR_SLICE(state->cells[4].slices[2].value, forkskinny_precomputed_round_constants[iteration][6]);
	#endif
	
	// only for TK2 and TK3
	state->cells[2].slices[1].value = XOR_SLICE(state->cells[2].slices[1].value, ONE);
	
	//auto unsliced_res = unslice_accelerated(*state).values[0].raw;
	//int appel = 1;
}

#endif //FORKSKINNYPLUS_FORKSKINNY_ADDCONSTANT_H
