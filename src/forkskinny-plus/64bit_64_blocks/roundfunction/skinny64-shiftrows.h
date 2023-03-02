#ifndef FORKSKINNYPLUS_SKINNY64_SHIFTROWS_H
#define FORKSKINNYPLUS_SKINNY64_SHIFTROWS_H

#include "../utils/skinny64_datatypes.h"

/**
 * This ShiftRows implementation conceptually does the correct ShiftRows operation towards the right according to the paper,
 * but produces wrong ciphertexts.
 * @param state
 */
static inline void skinny64_shiftrows(State64Sliced_t *state) {
	// 0xFEDC BA98 7654 3210
	auto test_blocks = Blocks64_t();
	test_blocks.values[0] = 0xFEDCBA9876543210;
	*state = slice(test_blocks);
	
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
	temp = state->cells[0xC];
	state->cells[0xC] = state->cells[0xD];
	state->cells[0xD] = state->cells[0xE];
	state->cells[0xE] = state->cells[0xF];
	state->cells[0xF] = temp;
	
	// Erik:    0x EDCF 98BA 4765 3210
	// Ik:      0x CFED 98BA 6547 3210
	auto test_res = unslice(*state).values[0];
	int appel = 1;
}


/**
 * This does does the ShiftRows operation, but rotates towards the left.
 * I compared the correctness of this ForkSKINNY implementation against another non-bitsliced implementation, and they do it like this.
 * For more info, see https://github.com/ErikP0/forkskinny-c/blob/ee30c666c301ee0ac1097dac1e40db17ebf51c6e/forkskinny64-cipher.c#L286.
 *
 * When executing the ShiftRows of the other implementation, rows are rotated towards the left when executing.
 * And since I have no other implementation of ForkSKINNY or no test vectors, I can't do much else than adopting this
 * other-way-around ShiftRows operation.
 *
 * This might have something to do with the endiannes? The other implementation incorporates endianness of the system,
 * maybe something's going wrong there?
 * @param state
 */
static inline void skinny64_shiftrows_old(State64Sliced_t *state) {
	// 0xFEDC BA98 7654 3210
	auto test_blocks = Blocks64_t();
	test_blocks.values[0] = 0xFEDCBA9876543210;
	*state = slice(test_blocks);
	
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
	temp = state->cells[0xF];
	state->cells[0xF] = state->cells[0xE];
	state->cells[0xE] = state->cells[0xD];
	state->cells[0xD] = state->cells[0xC];
	state->cells[0xC] = temp;
	
	// Erik:    0x EDCF 98BA 4765 3210
	// Ik:      0x CFED 98BA 6547 3210
	auto test_res = unslice(*state).values[0];
	int appel = 1;
}


#endif //FORKSKINNYPLUS_SKINNY64_SHIFTROWS_H
