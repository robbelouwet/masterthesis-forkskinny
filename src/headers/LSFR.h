#ifndef FORKSKINNYPLUS_LSFR_H
#define FORKSKINNYPLUS_LSFR_H

#include "../utils.h"
#include "forkskinny64-cipher.h"

static inline State64Sliced_8_t lsfr_64_tk2_4bit(State64Sliced_8_t state) {
	auto x0 = state.slices[0];
	
	state.slices[0] = state.slices[3];
	state.slices[3] = state.slices[2];
	state.slices[2] = state.slices[1];
	state.slices[1] = x0;
	state.slices[0] ^= state.slices[3];
	
	return state;
}

static inline State64Sliced_8_t lsfr_64_tk2_4bit_inverse(State64Sliced_8_t state) {
	auto prev_x3 =state.slices[0] ^ state.slices[3];
	
	state.slices[0] = state.slices[1];
	state.slices[1] = state.slices[2];
	state.slices[2] = state.slices[3];
	state.slices[3] = prev_x3;
	
	return state;
}

static inline State64Sliced_16_t lsfr_64_tk2_4bit(State64Sliced_16_t state) {
	auto x0 = state.slices[0];
	
	state.slices[0] = state.slices[3];
	state.slices[3] = state.slices[2];
	state.slices[2] = state.slices[1];
	state.slices[1] = x0;
	state.slices[0] ^= state.slices[3];
	
	return state;
}

static inline State64Sliced_16_t lsfr_64_tk2_4bit_inverse(State64Sliced_16_t state) {
	auto prev_x3 =state.slices[0] ^ state.slices[3];
	
	state.slices[0] = state.slices[1];
	state.slices[1] = state.slices[2];
	state.slices[2] = state.slices[3];
	state.slices[3] = prev_x3;
	
	return state;
}


#endif //FORKSKINNYPLUS_LSFR_H
