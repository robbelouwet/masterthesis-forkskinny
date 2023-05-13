#ifndef FORKSKINNYPLUS64_FORKSKINNY_ADDCONSTANT_H
#define FORKSKINNYPLUS64_FORKSKINNY_ADDCONSTANT_H

#include <cstdint>
#include "../utils/forkskinny64-datatypes.h"
#include "../../constants.h"

#if AVX2_acceleration || AVX512_acceleration
auto TWO = _mm256_set_epi64x(-1ULL, 0, 0,0);
static inline void forkskinny64_add_segmented_constant(HalfState64Sliced_t *state, uint16_t iteration) {
	// @formatter:off
	// C0 (at row 0, cell 1)
	state->segments256[0][0] = XOR256(state->segments256[0][0], forkskinny_precomputed_segmented_round_constants[iteration][0]);
	state->segments256[0][1] = XOR256(state->segments256[0][1], forkskinny_precomputed_segmented_round_constants[iteration][1]);
	state->segments256[0][2] = XOR256(state->segments256[0][2], forkskinny_precomputed_segmented_round_constants[iteration][2]);
	state->segments256[0][3] = XOR256(state->segments256[0][3], forkskinny_precomputed_segmented_round_constants[iteration][3]);
	
	// C1 (at row 1, cell 1)
	state->segments256[1][0] = XOR256(state->segments256[1][0], forkskinny_precomputed_segmented_round_constants[iteration][4]);
	state->segments256[1][1] = XOR256(state->segments256[1][1], forkskinny_precomputed_segmented_round_constants[iteration][5]);
	state->segments256[1][2] = XOR256(state->segments256[1][2], forkskinny_precomputed_segmented_round_constants[iteration][6]);
	
	// M2 (+) 0x2 from Appendix (at row 0, cell 3)
	state->segments256[0][1] = XOR256(state->segments256[0][1], TWO);
	// @formatter:on
}
#endif

static inline void forkskinny64_add_constant(HalfState64Sliced_t *state, uint16_t iteration) {
	// The beauty of unions:
	
	#if !FIXED_SLICING && (AVX2_acceleration || AVX512_acceleration)
	// Cell 0 XOR_AVX2 C0
	Cell64_t C0 = {.slices = {
			forkskinny_precomputed_round_constants[iteration][0],
			forkskinny_precomputed_round_constants[iteration][1],
			forkskinny_precomputed_round_constants[iteration][2],
			forkskinny_precomputed_round_constants[iteration][3]
	}};
	state->cells[1].avx2_simd_cell = _mm256_xor_si256(state->cells[1].avx2_simd_cell, C0.avx2_simd_cell);
	
	// Cell 4 XOR_AVX2 C1
	Cell64_t C1 = {.slices = {
			forkskinny_precomputed_round_constants[iteration][4],
			forkskinny_precomputed_round_constants[iteration][5],
			forkskinny_precomputed_round_constants[iteration][6],
			0
	}};
	state->cells[5].avx2_simd_cell = _mm256_xor_si256(state->cells[5].avx2_simd_cell, C1.avx2_simd_cell);
	#else
	state->cells[1].slices[0].value = XOR_SLICE(state->cells[1].slices[0].value, forkskinny_precomputed_round_constants[iteration][0]);
	state->cells[1].slices[1].value = XOR_SLICE(state->cells[1].slices[1].value, forkskinny_precomputed_round_constants[iteration][1]);
	state->cells[1].slices[2].value = XOR_SLICE(state->cells[1].slices[2].value, forkskinny_precomputed_round_constants[iteration][2]);
	state->cells[1].slices[3].value = XOR_SLICE(state->cells[1].slices[3].value, forkskinny_precomputed_round_constants[iteration][3]);
	
	state->cells[5].slices[0].value = XOR_SLICE(state->cells[5].slices[0].value, forkskinny_precomputed_round_constants[iteration][4]);
	state->cells[5].slices[1].value = XOR_SLICE(state->cells[5].slices[1].value, forkskinny_precomputed_round_constants[iteration][5]);
	state->cells[5].slices[2].value = XOR_SLICE(state->cells[5].slices[2].value, forkskinny_precomputed_round_constants[iteration][6]);
	#endif
	
	// only for TK2 and TK3
	state->cells[3].slices[1].value = XOR_SLICE(state->cells[3].slices[1].value, slice_ONE);
	
	//auto unsliced_res = unslice_accelerated(*state).values[0].raw;
	//int appel = 1;
}

#endif //FORKSKINNYPLUS_FORKSKINNY_ADDCONSTANT_H
