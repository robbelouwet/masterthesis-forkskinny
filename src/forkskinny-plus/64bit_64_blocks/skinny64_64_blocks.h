#ifndef FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H

#include "utils/skinny64_datatypes.h"
#include "roundfunction/skinny64-sbox.h"
#include "roundfunction/skinny64-addconstant.h"
#include "roundfunction/skinny64-shiftrows.h"
#include "roundfunction/skinny64-mixcols.h"


static inline void skinny_inject_key(HalfState64Sliced_t round_key, State64Sliced_t *state) {
	for (int i = 0; i < 8; ++i)
		state->cells[i].simd_cell = _mm256_xor_si256(round_key.cells[i].simd_cell, state->cells[i].simd_cell);
}

static inline void skinny64_encrypt_single_round_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state,
                                                           uint16_t iteration) {
	skinny64_sbox(state);
	
	skinny64_add_constant(state, iteration);
	
	skinny_inject_key(schedule.keys[iteration], state);
	
	skinny64_shiftrows(state);
	
	skinny64_mixcols(state);
}

static void skinny64_encrypt_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state) {
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < 40; i++)
		skinny64_encrypt_single_round_64_blocks(schedule, state, i);
}

#endif //FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H
