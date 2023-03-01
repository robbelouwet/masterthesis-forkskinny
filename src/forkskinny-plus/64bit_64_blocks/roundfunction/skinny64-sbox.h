#ifndef FORKSKINNYPLUS_SKINNY64_SBOX_H
#define FORKSKINNYPLUS_SKINNY64_SBOX_H

#include "../utils/skinny64_datatypes.h"

/// VERIFIED: 0xaaaaaaaaaaaaaaaa -> 0x5555555555555555
static inline void skinny64_sbox(State64Sliced_t *state) {
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		state->cells[i].slices[3] = cell.slices[0] ^ ~(cell.slices[3] | cell.slices[2]);
		state->cells[i].slices[2] = cell.slices[3] ^ ~(cell.slices[2] | cell.slices[1]);
		state->cells[i].slices[1] = cell.slices[2] ^ ~(cell.slices[1] | state->cells[i].slices[3]);
		state->cells[i].slices[0] = cell.slices[1] ^ ~(state->cells[i].slices[3] | state->cells[i].slices[2]);
	}
}

static inline Cell64_t skinny64_sbox_inverse(Cell64_t cell) {

}

#endif //FORKSKINNYPLUS_SKINNY64_SBOX_H
