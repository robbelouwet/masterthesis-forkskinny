#ifndef FORKSKINNYPLUS128_FORKSKINNY_ADDCONSTANT_H
#define FORKSKINNYPLUS128_FORKSKINNY_ADDCONSTANT_H

#include <cstdint>
#include "../utils/forkskinny128-datatypes.h"
#include "../../constants.h"

static inline void forkskinny128_add_constant(HalfState128Sliced_t *state, uint16_t iteration) {
	#if AVX2_acceleration
	// C0 (at row 0, cell 0)
	state->segments256[0][0] = XOR256(state->segments256[0][0], forkskinny128_round_constants[iteration][0]);
	state->segments256[0][1] = XOR256(state->segments256[0][1], forkskinny128_round_constants[iteration][1]);
	state->segments256[0][2] = XOR256(state->segments256[0][2], forkskinny128_round_constants[iteration][2]);
	state->segments256[0][3] = XOR256(state->segments256[0][3], forkskinny128_round_constants[iteration][3]);

	// C1 (at row 1, cell 0)
	state->segments256[1][0] = XOR256(state->segments256[1][0], forkskinny128_round_constants[iteration][4]);
	state->segments256[1][1] = XOR256(state->segments256[1][1], forkskinny128_round_constants[iteration][5]);
	state->segments256[1][2] = XOR256(state->segments256[1][2], forkskinny128_round_constants[iteration][6]);
	#else
	// Cell 0 XOR_AVX2 C0
	state->cells[0].slices[0].value = XOR_SLICE(state->cells[0].slices[0].value, forkskinny_unsegmented_round_constants[iteration][0]);
	state->cells[0].slices[1].value = XOR_SLICE(state->cells[0].slices[1].value, forkskinny_unsegmented_round_constants[iteration][1]);
	state->cells[0].slices[2].value = XOR_SLICE(state->cells[0].slices[2].value, forkskinny_unsegmented_round_constants[iteration][2]);
	state->cells[0].slices[3].value = XOR_SLICE(state->cells[0].slices[3].value, forkskinny_unsegmented_round_constants[iteration][3]);

	// Cell 4 XOR_AVX2 C1
	state->cells[4].slices[0].value = XOR_SLICE(state->cells[4].slices[0].value, forkskinny_unsegmented_round_constants[iteration][4]);
	state->cells[4].slices[1].value = XOR_SLICE(state->cells[4].slices[1].value, forkskinny_unsegmented_round_constants[iteration][5]);
	state->cells[4].slices[2].value = XOR_SLICE(state->cells[4].slices[2].value, forkskinny_unsegmented_round_constants[iteration][6]);
	
	// only for TK2 and TK3
	state->cells[2].slices[1].value = XOR_SLICE(state->cells[2].slices[1].value, slice_ONE);
	#endif
	
	//auto unsliced_res = unslice128_accelerated_internal(*state).values[0].raw;
	//int appel = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_ADDCONSTANT_H
