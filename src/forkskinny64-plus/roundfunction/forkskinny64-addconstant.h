#ifndef FORKSKINNYPLUS64_FORKSKINNY_ADDCONSTANT_H
#define FORKSKINNYPLUS64_FORKSKINNY_ADDCONSTANT_H

#include <cstdint>
#include "../utils/forkskinny64-datatypes.h"
#include "../../constants.h"

#if SEGMENTATION
//auto NTWO = _mm256_set_epi64x(-1ULL, 0, 0,0);
#endif
static inline void forkskinny64_add_constant(HalfState64Sliced_t *state, uint16_t iteration) {
	#if SEGMENTATION
	// @formatter:off
	// C0 (with packed 0x2) (at row 0, cell 1)
	
	state->segments256[0][0] = XOR256(state->segments256[0][0], forkskinny64_round_constants[iteration][0]);
	state->segments256[0][1] = XOR256(state->segments256[0][1], forkskinny64_round_constants[iteration][1]);
	state->segments256[0][2] = XOR256(state->segments256[0][2], forkskinny64_round_constants[iteration][2]);
	state->segments256[0][3] = XOR256(state->segments256[0][3], forkskinny64_round_constants[iteration][3]);
	
	// C1 (at row 1, cell 1)
	state->segments256[1][0] = XOR256(state->segments256[1][0], forkskinny64_round_constants[iteration][4]);
	state->segments256[1][1] = XOR256(state->segments256[1][1], forkskinny64_round_constants[iteration][5]);
	state->segments256[1][2] = XOR256(state->segments256[1][2], forkskinny64_round_constants[iteration][6]);
	
	// M2 (+) 0x2 from Appendix (at row 0, cell 3)
//	state->segments256[0][1] = XOR256(state->segments256[0][1], NTWO);
	// @formatter:on
	#else
	state->cells[1].slices[0].value = XOR_SLICE(state->cells[1].slices[0].value, forkskinny_unsegmented_round_constants[iteration][0]);
	state->cells[1].slices[1].value = XOR_SLICE(state->cells[1].slices[1].value, forkskinny_unsegmented_round_constants[iteration][1]);
	state->cells[1].slices[2].value = XOR_SLICE(state->cells[1].slices[2].value, forkskinny_unsegmented_round_constants[iteration][2]);
	state->cells[1].slices[3].value = XOR_SLICE(state->cells[1].slices[3].value, forkskinny_unsegmented_round_constants[iteration][3]);
	
	state->cells[5].slices[0].value = XOR_SLICE(state->cells[5].slices[0].value, forkskinny_unsegmented_round_constants[iteration][4]);
	state->cells[5].slices[1].value = XOR_SLICE(state->cells[5].slices[1].value, forkskinny_unsegmented_round_constants[iteration][5]);
	state->cells[5].slices[2].value = XOR_SLICE(state->cells[5].slices[2].value, forkskinny_unsegmented_round_constants[iteration][6]);
	
	// only for TK2 and TK3
	state->cells[3].slices[1].value = XOR_SLICE(state->cells[3].slices[1].value, slice_ONE);
	#endif
	
	//auto unsliced_res = unslice128_accelerated_internal(*state).values[0].raw;
	//int appel = 1;
}

#endif //FORKSKINNYPLUS_FORKSKINNY_ADDCONSTANT_H
