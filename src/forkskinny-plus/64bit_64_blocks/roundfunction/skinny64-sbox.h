#ifndef FORKSKINNYPLUS_SKINNY64_SBOX_H
#define FORKSKINNYPLUS_SKINNY64_SBOX_H

#include "../utils/skinny64_datatypes.h"

static inline void skinny64_sbox(State64Sliced_t *state) {
//	auto blocks = Blocks64_t{.values = {0xFEDCBA9876543210}};
//	*state = slice(blocks);
//	int appel = 1;
	
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		// @formatter:off
		state->cells[i].slices[3] = cell.slices[0] ^ ~(cell.slices[3]            | cell.slices[2]);
		state->cells[i].slices[2] = cell.slices[3] ^ ~(cell.slices[2]            | cell.slices[1]);
		state->cells[i].slices[1] = cell.slices[2] ^ ~(cell.slices[1]            | state->cells[i].slices[3]);
		state->cells[i].slices[0] = cell.slices[1] ^ ~(state->cells[i].slices[3] | state->cells[i].slices[2]);
		// @formatter:on
	}
	
	// INPUT:   0x FEDC BA98 7654 3210
	// OUTPUT:  0x F7E4 D583 B2A1 096C
//	auto test_res = unslice(*state).values[0].raw;
//	int banaan = 1;
}

static inline void skinny64_sbox_inv(State64Sliced_t *state) {
//	auto blocks = Blocks64_t{.values = {0xF7E4D583B2A1096C}};
//	*state = slice(blocks);
//	int appel = 1;
	
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		/*
		 y3 = x0 ^ ~(x3 | x2);
		 y2 = x3 ^ ~(x2 | x1);
		 y1 = x2 ^ ~(x1 | y3);
		 y0 = x1 ^ ~(y3 | y2);
		 
		 x1 = y0 ^ ~(y3 | y2);
		 x2 = y1 ^ ~(x1 | y3);
		 x3 = y2 ^ ~(x2 | x1);
		 x0 = y3 ^ ~(x3 | x2)
		 */
		
		// @formatter:off
		state->cells[i].slices[1] = cell.slices[0] ^ ~(cell.slices[3]            | cell.slices[2]);
		state->cells[i].slices[2] = cell.slices[1] ^ ~(state->cells[i].slices[1] | cell.slices[3]);
		state->cells[i].slices[3] = cell.slices[2] ^ ~(state->cells[i].slices[2] | state->cells[i].slices[1]);
		state->cells[i].slices[0] = cell.slices[3] ^ ~(state->cells[i].slices[3] | state->cells[i].slices[2]);
		// @formatter:on
	}
	
	// INPUT:   0x F7E4 D583 B2A1 096C
	// OUTPUT:  0x FEDC BA98 7654 3210
//	auto test_res = unslice(*state).values[0].raw;
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS_SKINNY64_SBOX_H
