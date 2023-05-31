#ifndef FORKSKINNYPLUS128_FORKSKINNY128_384_H
#define FORKSKINNYPLUS128_FORKSKINNY128_384_H

#include <x86intrin.h>
#include "utils/forkskinny128-datatypes.h"
#include "roundfunction/forkskinny128-sbox.h"
#include "roundfunction/forkskinny128-addconstant.h"
#include "roundfunction/forkskinny128-shiftrows.h"
#include "roundfunction/forkskinny128-mixcols.h"

static inline void add_branch_constant128(State128Sliced_t *state) {
	auto before = unslice128(state).values[0];
	// <editor-fold desc="branch constant">
	// @formatter:off
	#if AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		state->segments256[i][0] = XOR256(state->segments256[i][0], segmented_branch_constant128.segments256[i][0]);
		state->segments256[i][1] = XOR256(state->segments256[i][1], segmented_branch_constant128.segments256[i][1]);
		state->segments256[i][2] = XOR256(state->segments256[i][2], segmented_branch_constant128.segments256[i][2]);
		state->segments256[i][3] = XOR256(state->segments256[i][3], segmented_branch_constant128.segments256[i][3]);
		state->segments256[i][4] = XOR256(state->segments256[i][4], segmented_branch_constant128.segments256[i][4]);
		state->segments256[i][5] = XOR256(state->segments256[i][5], segmented_branch_constant128.segments256[i][5]);
		state->segments256[i][6] = XOR256(state->segments256[i][6], segmented_branch_constant128.segments256[i][6]);
		state->segments256[i][7] = XOR256(state->segments256[i][7], segmented_branch_constant128.segments256[i][7]);
	}
	
	#else
	for (int i = 0; i < 16; ++i) {
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, branch_constant128.cells[i].slices[0].value);
		state->cells[i].slices[1].value = XOR_SLICE(state->cells[i].slices[1].value, branch_constant128.cells[i].slices[1].value);
		state->cells[i].slices[2].value = XOR_SLICE(state->cells[i].slices[2].value, branch_constant128.cells[i].slices[2].value);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, branch_constant128.cells[i].slices[3].value);
		state->cells[i].slices[4].value = XOR_SLICE(state->cells[i].slices[4].value, branch_constant128.cells[i].slices[4].value);
		state->cells[i].slices[5].value = XOR_SLICE(state->cells[i].slices[5].value, branch_constant128.cells[i].slices[5].value);
		state->cells[i].slices[6].value = XOR_SLICE(state->cells[i].slices[6].value, branch_constant128.cells[i].slices[6].value);
		state->cells[i].slices[7].value = XOR_SLICE(state->cells[i].slices[7].value, branch_constant128.cells[i].slices[7].value);
	}
	
	#endif
	// @formatter:on
	// </editor-fold>
	auto after = unslice128(state).values[0];
	int appel = 1;
}

static inline void apply_roundkey(HalfState128Sliced_t *round_key, State128Sliced_t *state) {
	// <editor-fold desc="xor first 8 least significant cells">
	#if AVX2_acceleration
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 8; ++j)
			STOREU256(
					state->segments256[i] + j,
					XOR256(state->segments256[i][j], round_key->segments256[i][j])
			);
	
	// C2 (0x2) at cell 8
	STOREU256(state->segments256[2] + 1, XOR256(state->segments256[2][1], mask_0));
	
	#else
	for (int i = 0; i < 8; ++i) {
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value,
													round_key->cells[i].slices[0].value);
		state->cells[i].slices[1].value = XOR_SLICE(state->cells[i].slices[1].value,
													round_key->cells[i].slices[1].value);
		state->cells[i].slices[2].value = XOR_SLICE(state->cells[i].slices[2].value,
													round_key->cells[i].slices[2].value);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value,
													round_key->cells[i].slices[3].value);
		state->cells[i].slices[4].value = XOR_SLICE(state->cells[i].slices[4].value,
													round_key->cells[i].slices[4].value);
		state->cells[i].slices[5].value = XOR_SLICE(state->cells[i].slices[5].value,
													round_key->cells[i].slices[5].value);
		state->cells[i].slices[6].value = XOR_SLICE(state->cells[i].slices[6].value,
													round_key->cells[i].slices[6].value);
		state->cells[i].slices[7].value = XOR_SLICE(state->cells[i].slices[7].value,
													round_key->cells[i].slices[7].value);
	}
	
	// AddConstant: Cell 8 XOR 0x2, aka slice 1 of cell 8, because C2 is on the third row and not present in the round key!
	state->cells[8].slices[1].value = XOR_SLICE(state->cells[8].slices[1].value, slice_ONE);
	#endif
	
	// 0x 9D55 6079 A69D DDDA
	// 0x 6565 6565 6565 6534
//	auto test_output = unslice128(state).values[0];
//	int appel = 1;
	// </editor-fold
}

static inline void forkskinny128_encrypt_round(KeySchedule128Sliced_t *schedule, State128Sliced_t *state,
                                               uint16_t iteration) {
	// i: 0, rtk: 0x 9AC9 9F33 632C 5A77 (+ 0x02 @ injection step)
	auto roundkey = unslice128({.halves= {schedule->keys[iteration], {}}}).values[0].raw[0];
	
	auto test_sbox_before = unslice128(*state).values[0].raw[0]; // 0x EC4A FF51 7369 C667 | 0x 80
//	auto before = _rdtsc();
	forkskinny128_sbox(state);
//	auto after = _rdtsc();
//	std::cout << "fs128 sbox: " << after - before << "\n";
	auto test_state0 = unslice128(*state).values[0].raw[0]; // 0x 079C FF4A C5B1 87AD | 0x 6565 6565 6565 6536
	auto test_state1 = unslice128(*state).values[0].raw[1];
	
	/* round constant is added during pre computation of key schedule and added to the roundkey */
//	auto before2 = _rdtsc();
	apply_roundkey(&(schedule->keys[iteration]), state);
//	auto after2 = _rdtsc();
//	std::cout << "fs128 key injection: " <<  after2 - before2 << "\n";
	test_state0 = unslice128(*state).values[0].raw[0]; // 0x 9D55 6079 A69D DDDA | 0x 6565 6565 6565 6534
	test_state1 = unslice128(*state).values[0].raw[1];

//	auto before3 = _rdtsc();
	forkskinny128_shiftrows(state);
//	auto after3 = _rdtsc();
//	std::cout << "fs128 ShiftRows: " <<  after3 - before3 << "\n";
	test_state0 = unslice128(*state).values[0].raw[0]; // 0x 5560 799D A69D DDDA | 0x 6565 6565 6534 6565
	test_state1 = unslice128(*state).values[0].raw[1];

//	auto before4 = _rdtsc();
	forkskinny128_mixcols(state);
//	auto after4 = _rdtsc();
//	std::cout << "fs128 MixCols: " <<  after4 - before4 << "\n";

//	exit(0);
	test_state0 = unslice128(*state).values[0].raw[0]; // 0x A69D DDDA A6CC DDDA | 0x C3A9 B8BF 3054 1CF8
	test_state1 = unslice128(*state).values[0].raw[1];
	
	int appel = 1;
}

static inline void forkskinny128_encrypt(KeySchedule128Sliced_t *schedule, State128Sliced_t *state,
                                         State128Sliced_t *C0, State128Sliced_t *C1,
                                         unsigned char mode, uint8_t r_init, uint8_t r_after) {
//	auto initial_state = unslice128(*state).values[0].raw;
	
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < r_init; i++) {
		forkskinny128_encrypt_round(schedule, state, i);
		auto test0 = unslice128(state).values[0].raw[0];
		auto test1 = unslice128(state).values[0].raw[1];
		int appel = 1;
	}

//	auto test1_0 = unslice128(*state).values[0].raw[0];
//	auto test1_1 = unslice128(*state).values[0].raw[1];
	
	// ### C0 ###
	if (mode == '0' || mode == 'b') {
		*C0 = *state;
		for (int c0_i = i; c0_i < r_init + r_after; c0_i++) {
			forkskinny128_encrypt_round(schedule, C0, c0_i);
			auto test2a = unslice128(C0).values[0].raw[0];
			auto test2b = unslice128(C0).values[0].raw[1];
			int appel = 1;
		}
	}

//	auto test2b0 = unslice128(*state).values[0].raw[0];
//	auto test2b1 = unslice128(*state).values[0].raw[1];
//	u64 test3_bc0 = 0x0;
//	u64 test3_bc1 = 0x0;
	
	// ### C1 ###
	if (mode == '1' || mode == 'b') {
		*C1 = *state;
		// 0xB28BA9B609C7732D | 0x16A7C817056FF223
		add_branch_constant128(C1);
		// 0x30CA89A601C3712C | 0x9EE36A462D7BF826
//		test3_bc0 = unslice128(C1).values[0].raw[0];
//		test3_bc1 = unslice128(C1).values[0].raw[1];
		for (int c1_i = r_init + r_after; c1_i < (r_init + 2 * r_after); c1_i++) {
			forkskinny128_encrypt_round(schedule, C1, c1_i);
			auto test3a = unslice128(C1).values[0].raw[0];
			auto test3b = unslice128(C1).values[0].raw[1];
			int appel = 1;
		}
	}

//	auto test4a = unslice128(C1).values[0].raw[0];
//	auto test4b = unslice128(C1).values[0].raw[1];
}

static inline void forkskinny128_decrypt_round(KeySchedule128Sliced_t *schedule, State128Sliced_t *state,
                                               uint16_t iteration) {
	// 0x 3A45 57FB 4777 DA1D
	auto roundkey = unslice128({.halves= {schedule->keys[iteration], {}}}).values[0];

//	auto ct = Blocks128_t{.values = {0x9A9BFD8982C66E7C}};
//	*state = slice_t(ct);
	
	auto test_sbox_before = unslice128(*state).values[0];
	// --- 0x 72D8 8741 77DC 8C16 || 0x 266D A48E A11F D273 ---
	forkskinny128_mixcols_inv(state);
	auto test_state = unslice128(*state).values[0];
	
	// --- 0x F5AA F1BC 72D8 8741 || 0x 51B1 2898 54B5 23CF ---
	forkskinny128_shiftrows_inv(state);
	test_state = unslice128(*state).values[0];
	
	// --- BCF5 AAF1 72D8 8741 || 0x B128 9851 23CF 54B5
	apply_roundkey(&(schedule->keys[iteration]), state);
	test_state = unslice128(*state).values[0];
	
	// 0x 86B0 FD0A 35AF 5D5C || 0x B128 9851 23CF 54B7
	forkskinny128_sbox_inv(state);
	test_state = unslice128(*state).values[0];
	
	// 0x C33D 7F3A 10E7 5E5A || 0x 698B 1AD8 1DF4 5CED
	int appel = 1;
}

static inline void forkskinny128_decrypt_C0(KeySchedule128Sliced_t *schedule,
                                            State128Sliced_t *state, unsigned char mode,
                                            SlicedCiphertext128_t *result, uint8_t r_init, uint8_t r_after) {
	auto initial_state = unslice128(state).values[0];
	
	// decrypt C0 branch
	int c0_i = r_init + r_after - 1;
	for (; c0_i > r_init - 1; --c0_i) {
		forkskinny128_decrypt_round(schedule, state, c0_i);
		auto test = unslice128(state).values[0];
		int appel = 1;
	}
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		result->M = *state;
		for (int i = c0_i; i >= 0; --i) {
			forkskinny128_decrypt_round(schedule, &(result->M), i);
			auto test = unslice128(result->M).values[0];
			int appel = 1;
		}
	}
	
	// Re-encrypt to C1
	if (mode == 'o' || mode == 'b') {
		result->C1 = *state;
		auto before = unslice128(&(result->C1));
		add_branch_constant128(&(result->C1));
		auto after = unslice128(&(result->C1));
		for (int i = r_init + r_after; i < (r_init + 2 * r_after); ++i) {
			forkskinny128_encrypt_round(schedule, &(result->C1), i);
			auto test = unslice128(result->C1).values[0];
			int appel = 1;
			std::cout << i << ": ";
			print_block(test.bytes, 8);
			std::cout << ", ";
			print_block(&(test.bytes[8]), 8);
			std::cout << std::endl;
		}
	}
	
	auto testsss = unslice128(result->M).values[0];
	int appel = 1;
}

static inline void forkskinny128_decrypt_C1(KeySchedule128Sliced_t *schedule,
                                            State128Sliced_t *state, unsigned char mode,
                                            SlicedCiphertext128_t *result, uint8_t r_init, uint8_t r_after) {
	// decrypt C1 branch
	int c1_i = (r_init + 2 * r_after) - 1;
	for (; c1_i > r_init + r_after - 1; --c1_i)
		forkskinny128_decrypt_round(schedule, state, c1_i);
	
	add_branch_constant128(state);
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		result->M = *state;
		for (int i = c1_i - r_after; i >= 0; --i)
			forkskinny128_decrypt_round(schedule, &(result->M), i);
	}
	
	// Re-encrypt to C0
	if (mode == 'o' || mode == 'b') {
		result->C0 = *state;
		for (int i = r_init; i < r_init + r_after; ++i)
			forkskinny128_encrypt_round(schedule, &(result->C0), i);
	}
	
}

static inline void forkskinny128_decrypt(KeySchedule128Sliced_t *schedule,
                                         SlicedCiphertext128_t *ct, SlicedCiphertext128_t *result,
                                         unsigned char input_label, unsigned char mode, uint8_t r_init,
                                         uint8_t r_after) {
	if (input_label == '0')
		forkskinny128_decrypt_C0(schedule, &(ct->C0), mode, result, r_init, r_after);
	else forkskinny128_decrypt_C1(schedule, &(ct->C1), mode, result, r_init, r_after);
}

#endif //FORKSKINNYPLUS_FORKSKINNY128_384_H
