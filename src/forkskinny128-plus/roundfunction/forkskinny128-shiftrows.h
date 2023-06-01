#ifndef FORKSKINNYPLUS128_FORKSKINNY_SHIFTROWS_H
#define FORKSKINNYPLUS128_FORKSKINNY_SHIFTROWS_H

#include "../utils/forkskinny128-datatypes.h"

static inline void forkskinny128_shiftrows_inv(State128Sliced_t *state){
//	auto ct = Blocks128_t{.values = {0xEDCF98BA47653210}};
//	*state = slice_t(ct);
	
	#if AVX2_acceleration
	for (int i = 0; i < 8; ++i) {
		state->segments256[1][i] = _mm256_permute4x64_epi64(state->segments256[1][i], 0b00111001);
		state->segments256[2][i] = _mm256_permute4x64_epi64(state->segments256[2][i], 0b01001110);
		state->segments256[3][i] = _mm256_permute4x64_epi64(state->segments256[3][i], 0b10010011);
	}
	#else
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
	temp = state->cells[0xF];
	state->cells[0xF] = state->cells[0xE];
	state->cells[0xE] = state->cells[0xD];
	state->cells[0xD] = state->cells[0xC];
	state->cells[0xC] = temp;
	#endif

	// 0x BCF5 AAF1 72D8 8741 || 0x B128 9851 23CF 54B5
//	auto test_res = unslice128(*state).values[0];
//	int appel = 1;
}

/**
 * This ShiftRows implementation conceptually does the correct ShiftRows operation towards the C0 according to the paper,
 * but produces wrong ciphertexts.
 * @param state
 */
static inline void forkskinny128_shiftrows(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t{.values = {{.raw = {0x7766554433221100, 0xffeeddccbbaa9988}}}};
//	*state = slice_internal(test_blocks);
	
	#if AVX2_acceleration
	for (int i = 0; i < 8; ++i) {
		state->segments256[1][i] = _mm256_permute4x64_epi64(state->segments256[1][i], 0b10010011);
		state->segments256[2][i] = _mm256_permute4x64_epi64(state->segments256[2][i], 0b01001110);
		state->segments256[3][i] = _mm256_permute4x64_epi64(state->segments256[3][i], 0b00111001);
	}
	#else
	// shift second row
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
	
	// 0x 5560 799D A69D DDDA
	// 0x 6565 6565 6534 6565
//	auto test_res0 = unslice128(state).values[0].raw[0];
//	auto test_res1 = unslice128(state).values[0].raw[1];

//	forkskinny128_shiftrows_inv(state);
//	auto decr0 = unslice_accelerated_internal(*state).values[0].raw[0];
//	auto decr1 = unslice_accelerated_internal(*state).values[0].raw[1];
//
//	int appel = 1;
}
#endif //FORKSKINNYPLUS128_FORKSKINNY_SHIFTROWS_H
