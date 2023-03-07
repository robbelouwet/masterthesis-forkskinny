#ifndef FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H
#define FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H

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
	
	// AddConstant: Cell 8 XOR 0x2, aka slice 1 of cell 8, because C2 is on the third row and not present in the round key!
	state->cells[9].slices[1] ^= ONE;
}

static inline void encrypt_single_round_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state,
                                                  uint16_t iteration) {
	auto test_sbox_before = unslice(*state).values[0].raw;
	skinny64_sbox(state);
	auto test_state = unslice(*state).values[0].raw;
	
	/* round constant is added during pre computation of key schedule and added to the roundkey */
	inject_key(schedule.keys[iteration], state);
	test_state = unslice(*state).values[0].raw;
	
	skinny64_shiftrows(state);
	test_state = unslice(*state).values[0].raw;
	
	skinny64_mixcols(state);
	test_state = unslice(*state).values[0].raw;
	
	int appel = 1;
}

static SlicedCiphertext64_t forkskinny64_encrypt_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state) {
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < FORKSKINNY_ROUNDS_BEFORE; i++)
		encrypt_single_round_64_blocks(schedule, state, i);
	
	
	// ### LEFT ###
	auto left = *state; // dereference and copy the state
	auto unsliced_left = unslice(left).values[0];
	for (int l = i; l < i + FORKSKINNY_ROUNDS_AFTER; l++)
		encrypt_single_round_64_blocks(schedule, &left, l);
	
	
	// ### RIGHT ###
	auto right = *state; // dereference and copy the state
	add_branch_constant(&right);
	auto unsliced_right = unslice(right).values[0];
	for (int r = i; r < i + FORKSKINNY_ROUNDS_AFTER; r++)
		encrypt_single_round_64_blocks(schedule, &right, FORKSKINNY_ROUNDS_AFTER + r);
	
	
	return {left, right};
}

#endif //FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H
