#ifndef FORKSKINNYPLUS_FORKSKINNY64_H
#define FORKSKINNYPLUS_FORKSKINNY64_H

#include "utils/forkskinny-datatypes.h"
#include "roundfunction/forkskinny-sbox.h"
#include "roundfunction/forkskinny-addconstant.h"
#include "roundfunction/forkskinny-shiftrows.h"
#include "roundfunction/forkskinny-mixcols.h"

static inline void add_branch_constant(StateSliced_t *state) {
	// <editor-fold desc="branch constant">
	// @formatter:off
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i)
		state->pairs[i].avx512_simd_pair = _mm512_xor_si512(state->pairs[i].avx512_simd_pair, branch_constant.pairs[i].avx512_simd_pair);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i)
		state->cells[i].avx2_simd_cell   = _mm256_xor_si256(state->cells[i].avx2_simd_cell,   branch_constant.cells[i].avx2_simd_cell);
	
	#else
	for (int i = 0; i < 16; ++i) {
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, branch_constant.cells[i].slices[0].value);
		state->cells[i].slices[1].value = XOR_SLICE(state->cells[i].slices[1].value, branch_constant.cells[i].slices[1].value);
		state->cells[i].slices[2].value = XOR_SLICE(state->cells[i].slices[2].value, branch_constant.cells[i].slices[2].value);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, branch_constant.cells[i].slices[3].value);
	}
	
	#endif
	// @formatter:on
	// </editor-fold>
}

static inline void apply_roundkey(HalfStateSliced_t round_key, StateSliced_t *state) {
	// <editor-fold desc="xor first 8 least significant cells">
	#if AVX512_acceleration
	for (int i = 0; i < 4; ++i)
		state->pairs[i].avx512_simd_pair = _mm512_xor_si512(state->pairs[i].avx512_simd_pair,
															round_key.pairs[i].avx512_simd_pair);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 8; ++i)
		state->cells[i].avx2_simd_cell = _mm256_xor_si256(state->cells[i].avx2_simd_cell,
		                                                  round_key.cells[i].avx2_simd_cell);
	
	#else
	for (int i = 0; i < 8; ++i) {
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, round_key.cells[i].slices[0].value);
		state->cells[i].slices[1].value = XOR_SLICE(state->cells[i].slices[1].value, round_key.cells[i].slices[1].value);
		state->cells[i].slices[2].value = XOR_SLICE(state->cells[i].slices[2].value, round_key.cells[i].slices[2].value);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, round_key.cells[i].slices[3].value);
	}
	#endif
	// </editor-fold
	
	// AddConstant: Cell 8 XOR 0x2, aka slice_t 1 of cell 8, because C2 is on the third row and not present in the round key!
	// cell 8 is at position 9
	state->cells[9].slices[1].value = XOR_SLICE(state->cells[9].slices[1].value, ONE);
}

static inline void forkskinny64_encrypt_round(KeyScheduleSliced_t schedule, StateSliced_t *state,
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
static inline SlicedCiphertext_t forkskinny64_encrypt(KeyScheduleSliced_t schedule,
                                                      StateSliced_t *state, unsigned char mode) {
//	auto initial_state = unslice(*state).values[0].raw;
	
	// default values of branches
	auto C0 = StateSliced_t();
	auto C1 = StateSliced_t();
	
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < FORKSKINNY_ROUNDS_BEFORE; i++)
		forkskinny64_encrypt_round(schedule, state, i);

//	auto test1 = unslice(*state).values[0].raw;
	
	// ### C0 ###
	if (mode == '0' || mode == 'b') {
		C0 = *state;
		for (int c0_i = i; c0_i < FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; c0_i++) {
			forkskinny64_encrypt_round(schedule, &C0, c0_i);
//			auto test2a = unslice(C0).values[0].raw;
//			int appel = 1;
		}
	}

//	auto test2b = unslice(C0).values[0].raw;
//	uint64_t test3_bc = 0x0;
	
	// ### C1 ###
	if (mode == '1' || mode == 'b') {
		C1 = *state;
		add_branch_constant(&C1);
//		test3_bc = unslice(C1).values[0].raw;
		for (int c1_i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; c1_i < FORKSKINNY128_MAX_ROUNDS; c1_i++) {
			forkskinny64_encrypt_round(schedule, &C1, c1_i);
//			auto test3a = unslice(C1).values[0].raw;
//			int appel = 1;
		}
	}

//	auto test3 = unslice(C1).values[0].raw;
	
	return {C1, C0};
}

static inline void forkskinny64_decrypt_round(KeyScheduleSliced_t schedule, StateSliced_t *state,
                                              uint16_t iteration) {
//	auto roundkey = unslice({.halves= {schedule.keys[iteration], {}}}).values[0].raw;

//	auto ct = Blocks_t{.values = {0x9A9BFD8982C66E7C}};
//	*state = slice_t(ct);

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

static inline SlicedCiphertext_t forkskinny64_decrypt_C0(KeyScheduleSliced_t schedule,
                                                         StateSliced_t *state, unsigned char mode) {
	auto M = StateSliced_t();
	auto C1 = StateSliced_t();
	
	auto initial_state = unslice(*state).values[0].raw;
	
	uint64_t test2 = 0;
	// decrypt C0 branch
	int c0_i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER - 1;
	for (; c0_i > FORKSKINNY_ROUNDS_BEFORE - 1; --c0_i) {
		forkskinny64_decrypt_round(schedule, state, c0_i);
//		test2 = unslice(*state).values[0].raw;
//		int appel = 1;
	}
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		M = *state;
		for (int i = c0_i; i >= 0; --i)
			forkskinny64_decrypt_round(schedule, &M, i);
		
	}
	
	// Re-encrypt to C1
	if (mode == 'o' || mode == 'b') {
		C1 = *state;
		add_branch_constant(&C1);
		for (int i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; i < FORKSKINNY64_MAX_ROUNDS; ++i)
			forkskinny64_encrypt_round(schedule, &C1, i);
	}
	
	return {C1, StateSliced_t(), M};
	
}

static inline SlicedCiphertext_t forkskinny64_decrypt_C1(KeyScheduleSliced_t schedule,
                                                         StateSliced_t *state, unsigned char mode) {
	auto M = StateSliced_t();
	auto C0 = StateSliced_t();
	
	// decrypt C1 branch
	int c1_i = FORKSKINNY64_MAX_ROUNDS - 1;
	for (; c1_i > FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER - 1; --c1_i)
		forkskinny64_decrypt_round(schedule, state, c1_i);
	
	add_branch_constant(state);
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		M = *state;
		for (int i = c1_i - FORKSKINNY_ROUNDS_AFTER; i >= 0; --i)
			forkskinny64_decrypt_round(schedule, &M, i);
	}
	
	// Re-encrypt to C0
	if (mode == 'o' || mode == 'b') {
		C0 = *state;
		for (int i = FORKSKINNY_ROUNDS_BEFORE; i < FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; ++i)
			forkskinny64_encrypt_round(schedule, &C0, i);
	}
	
	return {StateSliced_t(), C0, M};
	
}

static inline SlicedCiphertext_t forkskinny64_decrypt(KeyScheduleSliced_t schedule, SlicedCiphertext_t *ct,
                                                      unsigned char input_label, unsigned char mode) {
	if (input_label == '0')
		return forkskinny64_decrypt_C0(schedule, &(ct->C0), mode);
	return forkskinny64_decrypt_C1(schedule, &(ct->C1), mode);
}

#endif //FORKSKINNYPLUS_FORKSKINNY64_H
