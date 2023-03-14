#ifndef FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H
#define FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H

#include "utils/skinny64_datatypes.h"
#include "roundfunction/skinny64-sbox.h"
#include "roundfunction/skinny64-addconstant.h"
#include "roundfunction/skinny64-shiftrows.h"
#include "roundfunction/skinny64-mixcols.h"

static inline void add_branch_constant(State64Sliced_t *state) {
	// <editor-fold desc="branch constant">
	// @formatter:off
	state->cells[0].simd_cell   = _mm256_xor_si256(state->cells[0].simd_cell,   branch_constant.cells[0].simd_cell);
	state->cells[1].simd_cell   = _mm256_xor_si256(state->cells[1].simd_cell,   branch_constant.cells[1].simd_cell);
	state->cells[2].simd_cell   = _mm256_xor_si256(state->cells[2].simd_cell,   branch_constant.cells[2].simd_cell);
	state->cells[3].simd_cell   = _mm256_xor_si256(state->cells[3].simd_cell,   branch_constant.cells[3].simd_cell);
	state->cells[4].simd_cell   = _mm256_xor_si256(state->cells[4].simd_cell,   branch_constant.cells[4].simd_cell);
	state->cells[5].simd_cell   = _mm256_xor_si256(state->cells[5].simd_cell,   branch_constant.cells[5].simd_cell);
	state->cells[6].simd_cell   = _mm256_xor_si256(state->cells[6].simd_cell,   branch_constant.cells[6].simd_cell);
	state->cells[7].simd_cell   = _mm256_xor_si256(state->cells[7].simd_cell,   branch_constant.cells[7].simd_cell);
	state->cells[8].simd_cell   = _mm256_xor_si256(state->cells[8].simd_cell,   branch_constant.cells[8].simd_cell);
	state->cells[9].simd_cell   = _mm256_xor_si256(state->cells[9].simd_cell,   branch_constant.cells[9].simd_cell);
	state->cells[0xa].simd_cell = _mm256_xor_si256(state->cells[0xa].simd_cell, branch_constant.cells[0xa].simd_cell);
	state->cells[0xb].simd_cell = _mm256_xor_si256(state->cells[0xb].simd_cell, branch_constant.cells[0xb].simd_cell);
	state->cells[0xc].simd_cell = _mm256_xor_si256(state->cells[0xc].simd_cell, branch_constant.cells[0xc].simd_cell);
	state->cells[0xd].simd_cell = _mm256_xor_si256(state->cells[0xd].simd_cell, branch_constant.cells[0xd].simd_cell);
	state->cells[0xe].simd_cell = _mm256_xor_si256(state->cells[0xe].simd_cell, branch_constant.cells[0xe].simd_cell);
	state->cells[0xf].simd_cell = _mm256_xor_si256(state->cells[0xf].simd_cell, branch_constant.cells[0xf].simd_cell);
	// @formatter:on
	// </editor-fold>
}

static inline void apply_roundkey(HalfState64Sliced_t round_key, State64Sliced_t *state) {
	// <editor-fold desc="xor first 8 least significant cells">
	state->cells[0].simd_cell = _mm256_xor_si256(round_key.cells[0].simd_cell, state->cells[0].simd_cell);
	state->cells[1].simd_cell = _mm256_xor_si256(round_key.cells[1].simd_cell, state->cells[1].simd_cell);
	state->cells[2].simd_cell = _mm256_xor_si256(round_key.cells[2].simd_cell, state->cells[2].simd_cell);
	state->cells[3].simd_cell = _mm256_xor_si256(round_key.cells[3].simd_cell, state->cells[3].simd_cell);
	state->cells[4].simd_cell = _mm256_xor_si256(round_key.cells[4].simd_cell, state->cells[4].simd_cell);
	state->cells[5].simd_cell = _mm256_xor_si256(round_key.cells[5].simd_cell, state->cells[5].simd_cell);
	state->cells[6].simd_cell = _mm256_xor_si256(round_key.cells[6].simd_cell, state->cells[6].simd_cell);
	state->cells[7].simd_cell = _mm256_xor_si256(round_key.cells[7].simd_cell, state->cells[7].simd_cell);
	// </editor-fold
	
	// AddConstant: Cell 8 XOR 0x2, aka slice 1 of cell 8, because C2 is on the third row and not present in the round key!
	// cell 8 is at position 9
	state->cells[9].slices[1] ^= ONE;
}

static inline void encrypt_single_round_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state,
                                                  uint16_t iteration) {
	// i: 0, 0x76541200
//	auto roundkey = unslice({.halves= {schedule.keys[iteration], {}}}).values[0].raw;

//	auto test_sbox_before = unslice(*state).values[0].raw; // 0x EFCD AB89 6745 2301
	skinny64_sbox(state);
//	auto test_state = unslice(*state).values[0].raw; // 0x 7F4E 5D38 2B1A 90C6
	
	/* round constant is added during pre computation of key schedule and added to the roundkey */
	apply_roundkey(schedule.keys[iteration], state);
//	test_state = unslice(*state).values[0].raw; // 0x 7F4E 5D18 5D4E 82C6
	
	skinny64_shiftrows(state);
//	test_state = unslice(*state).values[0].raw; // 0x F4E7 185D E5D4 82C6
	
	skinny64_mixcols(state);
//	test_state = unslice(*state).values[0].raw; // 0x 9A9B FD89 82C6 6E7C

//	int appel = 1;
}

/**
 *
 * @param schedule
 * @param state
 * @param mode 'b', '0', or '1'
 * @return
 */
static inline SlicedCiphertext_t forkskinny64_encrypt_64_blocks(KeySchedule64Sliced_t schedule,
                                                                State64Sliced_t *state, unsigned char mode) {
	auto initial_state = unslice(*state).values[0].raw;
	
	// default values of branches
	auto C0 = State64Sliced_t();
	auto C1 = State64Sliced_t();
	
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < FORKSKINNY_ROUNDS_BEFORE; i++)
		encrypt_single_round_64_blocks(schedule, state, i);
	
	auto test1 = unslice(*state).values[0].raw;
	
	// ### C0 ###
	if (mode == '0' || mode == 'b') {
		C0 = *state;
		for (int c0_i = i; c0_i < FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; c0_i++) {
			encrypt_single_round_64_blocks(schedule, &C0, c0_i);
			auto test2a = unslice(C0).values[0].raw;
			int appel = 1;
		}
	}
	
	auto test2b = unslice(C0).values[0].raw;
	uint64_t test3_bc = 0x0;
	
	// ### C1 ###
	if (mode == '1' || mode == 'b') {
		C1 = *state;
		add_branch_constant(&C1);
		test3_bc = unslice(C1).values[0].raw;
		for (int c1_i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; c1_i < FORKSKINNY64_MAX_ROUNDS; c1_i++) {
			encrypt_single_round_64_blocks(schedule, &C1, c1_i);
			auto test3a = unslice(C1).values[0].raw;
			int appel = 1;
		}
	}
	
	auto test3 = unslice(C1).values[0].raw;
	
	return {C1, C0};
}

static inline void decrypt_single_round_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state,
                                                  uint16_t iteration) {
//	auto roundkey = unslice({.halves= {schedule.keys[iteration], {}}}).values[0].raw;

//	auto ct = Blocks64_t{.values = {0x9A9BFD8982C66E7C}};
//	*state = slice(ct);

//	auto test_sbox_before = unslice(*state).values[0].raw;
	skinny64_mixcols_inv(state);
//	auto test_state = unslice(*state).values[0].raw;
	
	skinny64_shiftrows_inv(state);
//	test_state = unslice(*state).values[0].raw;
	
	apply_roundkey(schedule.keys[iteration], state);
//	test_state = unslice(*state).values[0].raw;
	
	skinny64_sbox_inv(state);
//	test_state = unslice(*state).values[0].raw;

//	int appel = 1;
}

static inline SlicedCiphertext_t forkskinny64_decrypt_64_blocks_C0(KeySchedule64Sliced_t schedule,
                                                                   State64Sliced_t *state, unsigned char mode) {
	auto M = State64Sliced_t();
	auto C1 = State64Sliced_t();
	
	auto initial_state = unslice(*state).values[0].raw;
	
	uint64_t test2 = 0;
	// decrypt C0 branch
	int c0_i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER - 1;
	for (; c0_i > FORKSKINNY_ROUNDS_BEFORE-1; --c0_i){
		decrypt_single_round_64_blocks(schedule, state, c0_i);
		test2 = unslice(*state).values[0].raw;
		int appel = 1;
	}
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		M = *state;
		for (int i = c0_i; i >= 0; --i){
			decrypt_single_round_64_blocks(schedule, &M, i);
			auto test = unslice(M).values[0].raw;
			int appel = 1;
		}
	}
	
	auto test3 = unslice(M).values[0].raw;
	uint64_t test3c = 0x0;
	
	auto test3_before = unslice(*state).values[0].raw;
	// Re-encrypt to C1
	if (mode == 'o' || mode == 'b') {
		C1 = *state;
		add_branch_constant(&C1);
		test3c = unslice(C1).values[0].raw;
		for (int i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; i < FORKSKINNY64_MAX_ROUNDS; ++i)
			encrypt_single_round_64_blocks(schedule, &C1, i);
	}
	
	auto test4 = unslice(C1).values[0].raw;
	
	return {C1, State64Sliced_t(), M};
	
}

static inline SlicedCiphertext_t forkskinny64_decrypt_64_blocks_C1(KeySchedule64Sliced_t schedule,
                                                                   State64Sliced_t *state, unsigned char mode) {
	auto M = State64Sliced_t();
	auto C0 = State64Sliced_t();
	
	auto initial_state = unslice(*state).values[0].raw;
	
	// decrypt C1 branch
	uint64_t test2 = 0;
	int c1_i = FORKSKINNY64_MAX_ROUNDS-1;
	for (; c1_i > FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER-1; --c1_i){
		decrypt_single_round_64_blocks(schedule, state, c1_i);
		test2 = unslice(*state).values[0].raw;
		int appel = 1;
	}
	
	add_branch_constant(state);
	
	auto test2_bc = unslice(*state).values[0].raw;
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		M = *state;
		for (int i = c1_i - FORKSKINNY_ROUNDS_AFTER; i >= 0; --i){
			decrypt_single_round_64_blocks(schedule, &M, i);
			test2 = unslice(M).values[0].raw;
			int appel = 1;
		}
	}
	
	auto test3 = unslice(*state).values[0].raw;
	// Re-encrypt to C0
	if (mode == 'o' || mode == 'b') {
		C0 = *state;
		for (int i = FORKSKINNY_ROUNDS_BEFORE; i < FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; ++i){
			encrypt_single_round_64_blocks(schedule, &C0, i);
			test2 = unslice(C0).values[0].raw;
			int appel = 1;
		}
	}
	
	auto test4 = unslice(C0).values[0].raw;
	return {State64Sliced_t(), C0, M};
	
}

static inline SlicedCiphertext_t forkskinny64_decrypt_64_blocks(KeySchedule64Sliced_t schedule, SlicedCiphertext_t *ct,
                                                                unsigned char input_label, unsigned char mode) {
	if (input_label == '0')
		return forkskinny64_decrypt_64_blocks_C0(schedule, &(ct->C0), mode);
	return forkskinny64_decrypt_64_blocks_C1(schedule, &(ct->C1), mode);
}

#endif //FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H
