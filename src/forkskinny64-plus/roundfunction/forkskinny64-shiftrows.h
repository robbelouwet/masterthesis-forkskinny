#ifndef FORKSKINNYPLUS64_FORKSKINNY_SHIFTROWS_H
#define FORKSKINNYPLUS64_FORKSKINNY_SHIFTROWS_H

#include "../utils/forkskinny64-datatypes.h"

static inline void forkskinny64_shiftrows_inv(State64Sliced_t *state){
//	auto ct = Blocks64_t{.values = {0xfedcba9876543210}};
//	*state = slice_accelerated(&ct);
	#if AVX2_acceleration || AVX512_acceleration
	for (int i = 0; i < 4; ++i) {
		state->segments256[1][i] = _mm256_permute4x64_epi64(state->segments256[1][i], 0b10010011);
		state->segments256[2][i] = _mm256_permute4x64_epi64(state->segments256[2][i], 0b01001110);
		state->segments256[3][i] = _mm256_permute4x64_epi64(state->segments256[3][i], 0b00111001);
	}
	#else
	auto temp = state->cells[7];
	state->cells[7] = state->cells[6];
	state->cells[6] = state->cells[5];
	state->cells[5] = state->cells[4];
	state->cells[4] = temp;
	
	// shift third row
	temp = state->cells[8];
	state->cells[8] = state->cells[0xA];
	state->cells[0xA] = temp;
	temp = state->cells[9];
	state->cells[9] = state->cells[0xB];
	state->cells[0xB] = temp;
	
	// shift fourth row
	temp = state->cells[0xF];
	state->cells[0xF] = state->cells[0xC];
	state->cells[0xC] = state->cells[0xD];
	state->cells[0xD] = state->cells[0xE];
	state->cells[0xE] = temp;
	#endif
//	auto test_res = unslice_accelerated(state).values[0].raw;
//	int appel = 1;
}

static inline void forkskinny64_shiftrows(State64Sliced_t *state) {
//	 0xFEDC BA98 7654 3210
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xFEDCBA9876543210;
//	*state = slice_accelerated(&test_blocks);
	
	#if AVX512_acceleration
	#elif AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		state->segments256[1][i] = _mm256_permute4x64_epi64(state->segments256[1][i], 0b00111001);
		state->segments256[2][i] = _mm256_permute4x64_epi64(state->segments256[2][i], 0b01001110);
		state->segments256[3][i] = _mm256_permute4x64_epi64(state->segments256[3][i], 0b10010011);
	}
	#else
	// shift second row
	auto temp = state->cells[4];
	state->cells[4] = state->cells[5];
	state->cells[5] = state->cells[6];
	state->cells[6] = state->cells[7];
	state->cells[7] = temp;
	
	// shift third row
	temp = state->cells[8];
	state->cells[8] = state->cells[0xA];
	state->cells[0xA] = temp;
	temp = state->cells[9];
	state->cells[9] = state->cells[0xB];
	state->cells[0xB] = temp;
	
	// shift fourth row
	temp = state->cells[0xC];
	state->cells[0xC] = state->cells[0xF];
	state->cells[0xF] = state->cells[0xE];
	state->cells[0xE] = state->cells[0xD];
	state->cells[0xD] = temp;
	#endif
	
	// Erik: 0x EDCF 98BA 4765 3210
	// Us:   0x
//	forkskinny64_shiftrows_inv(state);
//	auto test_res = unslice_accelerated(state).values[0].raw;
//	int appel = 1;
}
#endif //FORKSKINNYPLUS64_FORKSKINNY_SHIFTROWS_H
