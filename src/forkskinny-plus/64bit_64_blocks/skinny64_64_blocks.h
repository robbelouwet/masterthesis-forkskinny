#ifndef FORKSKINNYPLUS_SKINNY64_64_BLOCKS_H
#define FORKSKINNYPLUS_SKINNY64_64_BLOCKS_H

#include "utils/skinny64_datatypes.h"
#include "roundfunction/skinny64-sbox.h"
#include "roundfunction/skinny64-addconstant.h"
#include "roundfunction/skinny64-shiftrows.h"
#include "roundfunction/skinny64-mixcols.h"
#include "immintrin.h"

static inline void add_branch_constant(State64Sliced_t *state) {
	for (int i = 0; i < 16; ++i)
		state->cells[i].simd_cell = _mm256_xor_si256(state->cells[i].simd_cell, branch_constant.cells[i].simd_cell);
}

static inline void inject_key(HalfState64Sliced_t round_key, State64Sliced_t *state) {
	for (int i = 0; i < 8; ++i)
		state->cells[i].simd_cell = _mm256_xor_si256(round_key.cells[i].simd_cell, state->cells[i].simd_cell);
}

static inline void encrypt_single_round_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state,
                                                  uint16_t iteration, uint16_t key_index) {
	skinny64_sbox(state);
	skinny64_add_constant(state, iteration);
	
	inject_key(schedule.keys[key_index], state);
	
	skinny64_shiftrows(state);
	skinny64_mixcols(state);
}

static SlicedCiphertext64_t forkskinny64_encrypt_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state) {
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < FORKSKINNY_ROUNDS_BEFORE; i++)
		encrypt_single_round_64_blocks(schedule, state, i, i);
	
	
	// ### LEFT ###
	for (int l = i; l < FORKSKINNY64_MAX_ROUNDS; l++)
		encrypt_single_round_64_blocks(schedule, state, l, i);
	
	
	// ### RIGHT ###
	auto right = *state; // dereference and copy the state
	add_branch_constant(&right);
	for (int r = i; r < FORKSKINNY64_MAX_ROUNDS; r++)
		encrypt_single_round_64_blocks(schedule, &right, r, FORKSKINNY64_MAX_ROUNDS + r);
	
	
	return {*state, right};
}

#endif //FORKSKINNYPLUS_SKINNY64_64_BLOCKS_H
