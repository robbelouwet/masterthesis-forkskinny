#ifndef FORKSKINNYPLUS_SKINNY64_SHIFTROWS_H
#define FORKSKINNYPLUS_SKINNY64_SHIFTROWS_H

#include "../forkskinny64-plus.h"

static inline void skinny64_shiftrows(State64Sliced_t *state){
	
	// shift second row
	auto temp = state->cells[7];
	state->cells[7] = state->cells[6];
	state->cells[6] = state->cells[5];
	state->cells[5] = state->cells[4];
	state->cells[4] = temp;
	
	// shift third row
	temp = state->cells[8];
	state->cells[8] = state->cells[10];
	state->cells[10] = temp;
	temp = state->cells[9];
	state->cells[9] = state->cells[11];
	state->cells[11] = temp;
	
	// shift fourth row (sr 3 = sl 1)
	temp = state->cells[12];
	state->cells[12] = state->cells[13];
	state->cells[13] = state->cells[14];
	state->cells[14] = state->cells[15];
	state->cells[15] = temp;
}

#endif //FORKSKINNYPLUS_SKINNY64_SHIFTROWS_H
