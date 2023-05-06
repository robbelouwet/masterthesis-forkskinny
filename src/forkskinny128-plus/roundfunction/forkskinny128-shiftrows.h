#ifndef FORKSKINNYPLUS128_FORKSKINNY_SHIFTROWS_H
#define FORKSKINNYPLUS128_FORKSKINNY_SHIFTROWS_H

#include "../utils/forkskinny128-datatypes.h"

static inline void forkskinny128_shiftrows_inv(State128Sliced_t *state){
//	auto ct = Blocks128_t{.values = {0xEDCF98BA47653210}};
//	*state = slice_t(ct);
	
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

//	auto test_res = unslice_accelerated(*state).values[0].raw;
//	int appel = 1;
}

/**
 * This ShiftRows implementation conceptually does the correct ShiftRows operation towards the C0 according to the paper,
 * but produces wrong ciphertexts.
 * @param state
 */
static inline void forkskinny128_shiftrows(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t{.values = {{.raw = {0x7766554433221100, 0xffeeddccbbaa9988}}}};
//	*state = slice(test_blocks);
	
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
	
	// Input:   0x  FFEE DDCC | BBAA 9988 | 7766 5544 | 3322 1100
	// Erik:    0x  CCFF EEDD | 9988 BBAA | 6655 4477 | 3322 1100
	// Us:      0x  EEDD CCFF | 9988 BBAA | 6655 4477 | 3322 1100
//	auto test_res0 = unslice_accelerated(*state).values[0].raw[0];
//	auto test_res1 = unslice_accelerated(*state).values[0].raw[1];
//
//	forkskinny128_shiftrows_inv(state);
//	auto decr0 = unslice_accelerated(*state).values[0].raw[0];
//	auto decr1 = unslice_accelerated(*state).values[0].raw[1];
//
//	int appel = 1;
}
#endif //FORKSKINNYPLUS128_FORKSKINNY_SHIFTROWS_H
