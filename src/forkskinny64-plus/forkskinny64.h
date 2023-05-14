#ifndef FORKSKINNYPLUS64_FORKSKINNY_H
#define FORKSKINNYPLUS64_FORKSKINNY_H

#include "utils/forkskinny64-datatypes.h"
#include "roundfunction/forkskinny64-sbox.h"
#include "utils/slicing64-accelerated.h"
#include "roundfunction/forkskinny64-shiftrows.h"
#include "roundfunction/forkskinny64-mixcols.h"

static inline void add_branch_constant64(State64Sliced_t *state) {
	// <editor-fold desc="branch constant">
	// @formatter:off
	#if AVX512_acceleration
	for (int i = 0; i < 2; ++i) {
		state->segments512[i][0] = XOR512(state->segments512[i][0], segmented_branch_constant64.segments512[i][0]);
		state->segments512[i][1] = XOR512(state->segments512[i][1], segmented_branch_constant64.segments512[i][1]);
		state->segments512[i][2] = XOR512(state->segments512[i][2], segmented_branch_constant64.segments512[i][2]);
		state->segments512[i][3] = XOR512(state->segments512[i][3], segmented_branch_constant64.segments512[i][3]);
	}
	#elif AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		state->segments256[i][0] = XOR256(state->segments256[i][0], segmented_branch_constant64.segments256[i][0]);
		state->segments256[i][1] = XOR256(state->segments256[i][1], segmented_branch_constant64.segments256[i][1]);
		state->segments256[i][2] = XOR256(state->segments256[i][2], segmented_branch_constant64.segments256[i][2]);
		state->segments256[i][3] = XOR256(state->segments256[i][3], segmented_branch_constant64.segments256[i][3]);
	}
	#else
	for (int i = 0; i < 16; ++i) {
		state->cells[i].slices[0].value = XOR_SLICE(state->cells[i].slices[0].value, branch_constant64[i*4]);
		state->cells[i].slices[1].value = XOR_SLICE(state->cells[i].slices[1].value, branch_constant64[(i*4) + 1]);
		state->cells[i].slices[2].value = XOR_SLICE(state->cells[i].slices[2].value, branch_constant64[(i*4) + 2]);
		state->cells[i].slices[3].value = XOR_SLICE(state->cells[i].slices[3].value, branch_constant64[(i*4) + 3]);
	}
	#endif
	// @formatter:on
	// </editor-fold>
}

auto c2 = _mm256_set_epi64x(0, 0, -1ULL, 0);

static inline void apply_roundkey(HalfState64Sliced_t *round_key, State64Sliced_t *state) {
	// <editor-fold desc="xor first 8 least significant cells">
	#if AVX2_acceleration || AVX512_acceleration
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 4; ++j)
			STOREU256(
					state->segments256[i] + j,
					XOR256(state->segments256[i][j], round_key->segments256[i][j])
			);
	
	// AddConstant: M8 (in cell 9) 0x2, aka slice 1 of cell 8, because C2 is on the third row and not present in the round key!
	// See the thesis on where index segment[2][1] comes from
	state->segments256[2][1] ^= c2;
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
	}
	// AddConstant: Cell 8 XOR_AVX2 0x2, aka Slice64_t 1 of cell 8, because C2 is on the third row and not present in the round key!
	state->cells[9].slices[1].value = XOR_SLICE(state->cells[9].slices[1].value, slice_ONE);
	#endif
	// </editor-fold
}

static inline void forkskinny64_encrypt_round(KeySchedule64Sliced_t *schedule, State64Sliced_t *state,
                                              uint16_t iteration) {
	// i: 0, 0x76541200
//	auto roundkey = unslice_accelerated({.halves= {schedule->keys[iteration], {}}}, false).values[0].raw;

//	auto test_sbox_before = unslice_accelerated(*state).values[0].raw; // 0x EFCD AB89 6745 2301
//	auto before0 = _rdtsc();
	forkskinny64_sbox(state);
//	auto after0 = _rdtsc();
//	std::cout << "SBOX: " << after0 - before0 << std::endl;
//	auto test_state = unslice_accelerated(*state).values[0].raw; // 0x 7F4E 5D38 2B1A 90C6
	
	/* round constant is added during pre computation of key schedule and added to the roundkey */
	apply_roundkey(schedule->keys + iteration, state);
//	auto after1 = _rdtsc();
//	std::cout << "Key injection: " << after1 - before1 << std::endl;
//	test_state = unslice_accelerated(*state).values[0].raw; // 0x 7F4E 5D18 C51A 6D26

//	auto before2 = _rdtsc();
	forkskinny64_shiftrows(state);
//	auto after2 = _rdtsc();
//	std::cout << "ShiftRows: " << after2 - before2 << std::endl;
//	test_state = unslice_accelerated(*state).values[0].raw; // 0x F4E7 185D AC51 6D26

//	auto before3 = _rdtsc();
	forkskinny64_mixcols(state);
//	auto after3 = _rdtsc();
//	std::cout << "MixCols: " << after3 - before3 << std::endl;
//	exit(1);
//	test_state = unslice_accelerated(*state).values[0].raw; // 0x 757B B40C 6D26 819C

//	int appel = 1;

}

/**
 *
 * @param schedule
 * @param state
 * @param mode '0': M -> C0
 * 			   '1': m -> C1
 * 			   'b': m -> (C0, C1)
 * @return
 */
// TODO: return by reference
static inline SlicedCiphertext64_t forkskinny64_encrypt(KeySchedule64Sliced_t *schedule,
                                                        State64Sliced_t *state, unsigned char const mode) {
//	auto initial_state = unslice_accelerated(*state).values[0].raw;
	
	// default values of branches
	auto C0 = State64Sliced_t();
	auto C1 = State64Sliced_t();
	
	// ### INITIAL ROUNDS ###
	int i = 0;
	for (; i < FORKSKINNY_ROUNDS_BEFORE; i++) {
		forkskinny64_encrypt_round(schedule, state, i);
//		auto test = unslice_accelerated(*state).values[0].raw;
//		int bannn = 1;
	}


//	auto test1 = unslice_accelerated(*state).values[0].raw; // 0xE86B7E7E22F3BA92
	
	// ### C0 ###
	if (mode == '0' || mode == 'b') {
		C0 = *state;
		for (int c0_i = i; c0_i < FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; c0_i++) {
			forkskinny64_encrypt_round(schedule, &C0, c0_i);
//			auto test2a = unslice_accelerated(C0).values[0].raw;
//			int appel = 1;
		}
	}

//	auto test2b = unslice_accelerated(C0).values[0].raw;
//	u64 test3_bc = 0x0;
	
	// ### C1 ###
	if (mode == '1' || mode == 'b') {
		C1 = *state;
//		auto test3_before = unslice_accelerated(C1).values[0].raw;
		add_branch_constant64(&C1);
//		test3_bc = unslice_accelerated(C1).values[0].raw;
		for (int c1_i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER;
		     c1_i < FORKSKINNY64_MAX_ROUNDS; c1_i++) {
			forkskinny64_encrypt_round(schedule, &C1, c1_i);
//			auto test3a = unslice_accelerated(C1).values[0].raw;
//			int appel = 1;
		}
	}

//	auto test3 = unslice_accelerated(C1).values[0].raw;
	
	return {C1, C0};
}

static inline void forkskinny64_decrypt_round(KeySchedule64Sliced_t *schedule, State64Sliced_t *state,
                                              uint16_t iteration) {
//	State64Sliced_t uRTK = {.halves= {schedule->keys[iteration], {}}};
//	auto roundkey = unslice_accelerated(uRTK).values[0].raw;

//	auto ct = Blocks64_t{.values = {0x9A9BFD8982C66E7C}};
//	*state = Slice64_t(ct);

//	auto test_sbox_before = unslice_accelerated(*state).values[0].raw;
	forkskinny64_mixcols_inv(state);
//	auto test_state = unslice_accelerated(*state).values[0].raw;

	forkskinny64_shiftrows_inv(state);
//	test_state = unslice_accelerated(*state).values[0].raw;
	
	apply_roundkey(schedule->keys + iteration, state);
//	test_state = unslice_accelerated(*state).values[0].raw;

	forkskinny64_sbox_inv(state);
//	test_state = unslice_accelerated(*state).values[0].raw;

//	int appel = 1;
}

// TODO: return by reference
static inline SlicedCiphertext64_t forkskinny64_decrypt_C0(KeySchedule64Sliced_t *schedule,
                                                           State64Sliced_t *state, unsigned char mode) {
	auto M = State64Sliced_t();
	auto C1 = State64Sliced_t();

//	auto initial_state = unslice_accelerated(*state).values[0].raw;
	
	u64 test2 = 0;
	// decrypt C0 branch
	int c0_i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER - 1;
	for (; c0_i > FORKSKINNY_ROUNDS_BEFORE - 1; --c0_i) {
		forkskinny64_decrypt_round(schedule, state, c0_i);
//		test2 = unslice_accelerated(*state).values[0].raw;
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
		add_branch_constant64(&C1);
		for (int i = FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; i < FORKSKINNY64_MAX_ROUNDS; ++i)
			forkskinny64_encrypt_round(schedule, &C1, i);
	}
	
	return {C1, State64Sliced_t(), M};
	
}

// TODO: return by reference
static inline SlicedCiphertext64_t forkskinny64_decrypt_C1(KeySchedule64Sliced_t *schedule,
                                                           State64Sliced_t *state, unsigned char mode) {
	auto M = State64Sliced_t();
	auto C0 = State64Sliced_t();
	
	// decrypt C1 branch
	int c1_i = FORKSKINNY64_MAX_ROUNDS - 1;
	for (; c1_i > FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER - 1; --c1_i)
		forkskinny64_decrypt_round(schedule, state, c1_i);

	// 0x 6987 0125 F8C5 F380
//	auto before = unslice_accelerated(*state).values[0].raw;
	add_branch_constant64(state);
//	auto after = unslice_accelerated(*state).values[0].raw;
	// 0x E86B 7E7E 22F3 BA92
	
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
	
	return {State64Sliced_t(), C0, M};
	
}

/**
 *
 * @param schedule
 * @param ct
 * @param input_label '0': input C0
 * 					  '1': input C1
 * @param mode 'o': CX -> CY
 * 			   'b': CX -> (CY, M)
 * @return
 */
 // TODO return by reference
static inline SlicedCiphertext64_t forkskinny64_decrypt(KeySchedule64Sliced_t *schedule, SlicedCiphertext64_t *ct,
                                                        unsigned char input_label, unsigned char mode) {
	if (input_label == '0')
		return forkskinny64_decrypt_C0(schedule, &(ct->C0), mode);
	return forkskinny64_decrypt_C1(schedule, &(ct->C1), mode);
}

#endif //FORKSKINNYPLUS64_FORKSKINNY_H
