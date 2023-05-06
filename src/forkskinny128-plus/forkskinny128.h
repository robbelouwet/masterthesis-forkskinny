#ifndef FORKSKINNYPLUS128_FORKSKINNY128_384_H
#define FORKSKINNYPLUS128_FORKSKINNY128_384_H

#include <x86intrin.h>
#include "utils/forkskinny128-datatypes.h"
#include "roundfunction/forkskinny128-sbox.h"
#include "roundfunction/forkskinny128-addconstant.h"
#include "roundfunction/forkskinny128-shiftrows.h"
#include "roundfunction/forkskinny128-mixcols.h"

static inline void add_branch_constant128(State128Sliced_t *state) {
	// <editor-fold desc="branch constant">
	// @formatter:off
	#if AVX512_acceleration
	for (int i = 0; i < 16; ++i)
		state->cells[i].avx512_simd_cell = _mm512_xor_si512(state->cells[i].avx512_simd_cell, branch_constant128.cells[i].avx512_simd_cell);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 16; ++i){
		state->cells[i].avx2_simd_cells[0]   = _mm256_xor_si256(state->cells[i].avx2_simd_cells[0],   branch_constant128.cells[i].avx2_simd_cells[0]);
		state->cells[i].avx2_simd_cells[1]   = _mm256_xor_si256(state->cells[i].avx2_simd_cells[1],   branch_constant128.cells[i].avx2_simd_cells[1]);
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
}

static inline void apply_roundkey(HalfState128Sliced_t round_key, State128Sliced_t *state) {
	// <editor-fold desc="xor first 8 least significant cells">
	#if AVX512_acceleration
	for (int i = 0; i < 8; ++i)
		state->cells[i].avx512_simd_cell = _mm512_xor_si512(state->cells[i].avx512_simd_cell,
															round_key.cells[i].avx512_simd_cell);
	
	#elif AVX2_acceleration
	for (int i = 0; i < 8; ++i){
		state->cells[i].avx2_simd_cells[0] = _mm256_xor_si256(state->cells[i].avx2_simd_cells[0], round_key.cells[i].avx2_simd_cells[0]);
		state->cells[i].avx2_simd_cells[1] = _mm256_xor_si256(state->cells[i].avx2_simd_cells[1], round_key.cells[i].avx2_simd_cells[1]);
	}
	
	#else
	for (int i = 0; i < 8; ++i) {
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, round_key.cells[i].slices[0].value);
		state->cells[i].slices[1].value = XOR_SLICE(state->cells[i].slices[1].value, round_key.cells[i].slices[1].value);
		state->cells[i].slices[2].value = XOR_SLICE(state->cells[i].slices[2].value, round_key.cells[i].slices[2].value);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, round_key.cells[i].slices[3].value);
		state->cells[i].slices[4].value = XOR_SLICE(state->cells[i].slices[4].value, round_key.cells[i].slices[4].value);
		state->cells[i].slices[5].value = XOR_SLICE(state->cells[i].slices[5].value, round_key.cells[i].slices[5].value);
		state->cells[i].slices[6].value = XOR_SLICE(state->cells[i].slices[6].value, round_key.cells[i].slices[6].value);
		state->cells[i].slices[7].value = XOR_SLICE(state->cells[i].slices[7].value, round_key.cells[i].slices[7].value);
	}
	#endif
	// </editor-fold
	
	// AddConstant: Cell 8 XOR 0x2, aka slice 1 of cell 8, because C2 is on the third row and not present in the round key!
	state->cells[8].slices[1].value = XOR_SLICE(state->cells[8].slices[1].value, ONE);
}

static inline void forkskinny128_encrypt_round(KeySchedule128Sliced_t *schedule, State128Sliced_t *state,
                                               uint16_t iteration) {
	// i: 0, rtk: 0x 9AC9 9F33 632C 5A77 (+ 0x02 @ injection step)
	auto roundkey = unslice_accelerated({.halves= {schedule->keys[iteration], {}}}).values[0].raw[0];

	auto test_sbox_before = unslice_accelerated(*state).values[0].raw[0]; // 0x EC4A FF51 7369 C667 | 0x 80
//	auto before = _rdtsc();
	forkskinny128_sbox(state);
//	auto after = _rdtsc();
//	std::cout << "fs128 sbox: " << after - before << "\n";
	auto test_state0 = unslice_accelerated(*state).values[0].raw[0]; // 0x 079C FF4A C5B1 87AD | 0x 6565 6565 6565 6536
	auto test_state1 = unslice_accelerated(*state).values[0].raw[1];
	
	/* round constant is added during pre computation of key schedule and added to the roundkey */
//	auto before2 = _rdtsc();
	apply_roundkey(schedule->keys[iteration], state);
//	auto after2 = _rdtsc();
//	std::cout << "fs128 key injection: " <<  after2 - before2 << "\n";
	test_state0 = unslice_accelerated(*state).values[0].raw[0]; // 0x 9D55 6079 A69D DDDA | 0x 6565 6565 6565 6534
	test_state1 = unslice_accelerated(*state).values[0].raw[1];
	
//	auto before3 = _rdtsc();
	forkskinny128_shiftrows(state);
//	auto after3 = _rdtsc();
//	std::cout << "fs128 ShiftRows: " <<  after3 - before3 << "\n";
	test_state0 = unslice_accelerated(*state).values[0].raw[0]; // 0x 5560 799D A69D DDDA | 0x 6565 6565 6534 6565
	test_state1 = unslice_accelerated(*state).values[0].raw[1];
	
//	auto before4 = _rdtsc();
	forkskinny128_mixcols(state);
//	auto after4 = _rdtsc();
//	std::cout << "fs128 MixCols: " <<  after4 - before4 << "\n";
	
//	exit(0);
	test_state0 = unslice_accelerated(*state).values[0].raw[0]; // 0x A69D DDDA A6CC DDDA | 0x C3A9 B8BF 3054 1CF8
	test_state1 = unslice_accelerated(*state).values[0].raw[1];

	int appel = 1;
}

/**
 *
 * @param schedule
 * @param state
 * @param mode
 * @param r_init
 * @param r_after
 * @return
 */
static inline SlicedCiphertext128_t forkskinny128_encrypt(KeySchedule128Sliced_t *schedule,
                                                          State128Sliced_t *state, unsigned char mode, uint8_t r_init,
                                                          uint8_t r_after) {
//	auto initial_state = unslice_accelerated(*state).values[0].raw;
	
	// default values of branches
	auto C0 = State128Sliced_t();
	auto C1 = State128Sliced_t();
	
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < r_init; i++){
		forkskinny128_encrypt_round(schedule, state, i);
//		auto test0 = unslice_accelerated(*state).values[0].raw[0];
//		auto test1 = unslice_accelerated(*state).values[0].raw[1];
//		int appel = 1;
	}

//	auto test1_0 = unslice_accelerated(*state).values[0].raw[0];
//	auto test1_1 = unslice_accelerated(*state).values[0].raw[1];
	
	// ### C0 ###
	if (mode == '0' || mode == 'b') {
		C0 = *state;
		for (int c0_i = i; c0_i < r_init + r_after; c0_i++) {
			forkskinny128_encrypt_round(schedule, &C0, c0_i);
//			auto test2a = unslice_accelerated(C0).values[0].raw[0];
//			auto test2b = unslice_accelerated(C0).values[0].raw[1];
//			int appel = 1;
		}
	}

//	auto test2b0 = unslice_accelerated(*state).values[0].raw[0];
//	auto test2b1 = unslice_accelerated(*state).values[0].raw[1];
//	u64 test3_bc0 = 0x0;
//	u64 test3_bc1 = 0x0;
	
	// ### C1 ###
	if (mode == '1' || mode == 'b') {
		C1 = *state;
		// 0xB28BA9B609C7732D | 0x16A7C817056FF223
		add_branch_constant128(&C1);
		// 0x30CA89A601C3712C | 0x9EE36A462D7BF826
//		test3_bc0 = unslice_accelerated(C1).values[0].raw[0];
//		test3_bc1 = unslice_accelerated(C1).values[0].raw[1];
		for (int c1_i = r_init + r_after; c1_i < (r_init + 2 * r_after); c1_i++) {
			forkskinny128_encrypt_round(schedule, &C1, c1_i);
//			auto test3a = unslice_accelerated(C1).values[0].raw[0];
//			auto test3b = unslice_accelerated(C1).values[0].raw[1];
//			int appel = 1;
		}
	}

//	auto test4a = unslice_accelerated(C1).values[0].raw[0];
//	auto test4b = unslice_accelerated(C1).values[0].raw[1];
	
	return {C1, C0};
}

static inline void forkskinny128_decrypt_round(KeySchedule128Sliced_t *schedule, State128Sliced_t *state,
                                               uint16_t iteration) {
//	auto roundkey = unslice_accelerated({.halves= {schedule.keys[iteration], {}}}).values[0].raw;

//	auto ct = Blocks128_t{.values = {0x9A9BFD8982C66E7C}};
//	*state = slice_t(ct);

//	auto test_sbox_before = unslice_accelerated(*state).values[0].raw;
	forkskinny128_mixcols_inv(state);
//	auto test_state = unslice_accelerated(*state).values[0].raw;
	
	forkskinny128_shiftrows_inv(state);
//	test_state = unslice_accelerated(*state).values[0].raw;
	
	apply_roundkey(schedule->keys[iteration], state);
//	test_state = unslice_accelerated(*state).values[0].raw;
	
	forkskinny128_sbox_inv(state);
//	test_state = unslice_accelerated(*state).values[0].raw;

//	int appel = 1;
}

static inline SlicedCiphertext128_t forkskinny128_decrypt_C0(KeySchedule128Sliced_t *schedule,
                                                             State128Sliced_t *state, unsigned char mode, uint8_t r_init,
															 uint8_t r_after) {
	auto M = State128Sliced_t();
	auto C1 = State128Sliced_t();
	
//	auto initial_state = unslice_accelerated(*state).values[0].raw;
	
	u64 test2 = 0;
	// decrypt C0 branch
	int c0_i = r_init + r_after - 1;
	for (; c0_i > r_init - 1; --c0_i) {
		forkskinny128_decrypt_round(schedule, state, c0_i);
//		test2 = unslice_accelerated(*state).values[0].raw;
//		int appel = 1;
	}
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		M = *state;
		for (int i = c0_i; i >= 0; --i)
			forkskinny128_decrypt_round(schedule, &M, i);
	}
	
	// Re-encrypt to C1
	if (mode == 'o' || mode == 'b') {
		C1 = *state;
		add_branch_constant128(&C1);
		for (int i = r_init + r_after; i < (r_init + 2 * r_after); ++i)
			forkskinny128_encrypt_round(schedule, &C1, i);
	}
	
	return {C1, State128Sliced_t(), M};
	
}

static inline SlicedCiphertext128_t forkskinny128_decrypt_C1(KeySchedule128Sliced_t *schedule,
                                                             State128Sliced_t *state, unsigned char mode, uint8_t r_init,
															 uint8_t r_after) {
	auto M = State128Sliced_t();
	auto C0 = State128Sliced_t();
	
	// decrypt C1 branch
	int c1_i = (r_init + 2 * r_after) - 1;
	for (; c1_i > r_init + r_after - 1; --c1_i)
		forkskinny128_decrypt_round(schedule, state, c1_i);
	
	add_branch_constant128(state);
	
	// Further decrypt to M
	if (mode == 'i' || mode == 'b') {
		M = *state;
		for (int i = c1_i - r_after; i >= 0; --i)
			forkskinny128_decrypt_round(schedule, &M, i);
	}
	
	// Re-encrypt to C0
	if (mode == 'o' || mode == 'b') {
		C0 = *state;
		for (int i = r_init; i < r_init + r_after; ++i)
			forkskinny128_encrypt_round(schedule, &C0, i);
	}
	
	return {State128Sliced_t(), C0, M};
	
}

static inline SlicedCiphertext128_t forkskinny128_decrypt(KeySchedule128Sliced_t *schedule, SlicedCiphertext128_t *ct,
                                                          unsigned char input_label, unsigned char mode, uint8_t r_init,
														  uint8_t r_after) {
	if (input_label == '0')
		return forkskinny128_decrypt_C0(schedule, &(ct->C0), mode, r_init, r_after);
	return forkskinny128_decrypt_C1(schedule, &(ct->C1), mode, r_init, r_after);
}

#endif //FORKSKINNYPLUS_FORKSKINNY128_384_H
