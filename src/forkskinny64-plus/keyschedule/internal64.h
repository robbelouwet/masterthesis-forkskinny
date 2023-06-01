#ifndef FORKSKINNYPLUS64_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS64_KEYSCHEDULE_INTERNAL_H

//#include <immintrin.h>
#include <cstring>
#include "../utils/forkskinny64-datatypes.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "../common.h"
#include "../utils/slicing64-accelerated-internal.h"
#include "../utils/slicing64-internal.h"
#include "../common.h"
#include "../utils/slicing64.h"

static inline void tk2_lfsr64(State64Sliced_t *state, const bool full_state = false) {
	int bound = full_state ? 4 : 2;
	#if AVX2_acceleration
	for (int i = 0; i < bound; ++i) {
		auto temp = state->segments256[i][3];
		state->segments256[i][3] = state->segments256[i][2];
		state->segments256[i][2] = state->segments256[i][1];
		state->segments256[i][1] = state->segments256[i][0];
		state->segments256[i][0] = XOR256(state->segments256[i][3], temp);
	}
	#else
	for (int i = 0; i < (bound << 2); i++) {
		auto temp = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[0];
		state->cells[i].slices[0].value = XOR_SLICE(temp.value, state->cells[i].slices[3].value);
	}
	#endif
}

static inline void tk3_lfsr64(State64Sliced_t *state, const bool full_state = false) {
	int bound = full_state ? 4 : 2;
	#if AVX2_acceleration
	for (int i = 0; i < bound; ++i) {
		auto temp = state->segments256[i][0];
		state->segments256[i][0] = state->segments256[i][1];
		state->segments256[i][1] = state->segments256[i][2];
		state->segments256[i][2] = state->segments256[i][3];
		state->segments256[i][3] = XOR256(state->segments256[i][3], temp);
	}
	
	#else
	for (int i = 0; i < (bound << 2); i++) {
		// 0b00111001 = 0 3 2 1 -> lanes for the simd permutation
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3].value = XOR_SLICE(temp.value, state->cells[i].slices[3].value);
	}
	#endif
}

/// Make sure you first understand how the nibble-swapped cipher works
static inline void permute64(State64Sliced_t *state) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xFEDCBA9876543210;
//	*state = slice64(&test_blocks);
	
	#if AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		auto row2 = LOAD256(state->segments256[2] + i);
		auto row3 = LOAD256(state->segments256[3] + i);

		/* Align row 2 & 3 for easy segment64 swapping */
		row2 = PERM_4x64(row2, 0b01100011);
		row3 = PERM_4x64(row3, 0b01001011);

		/* SWAP cell 1 of row 2 & 3 */
		auto r2_c2 = AND256(row2, mask_2);
		auto r3_c2 = AND256(row3, mask_2);
		row2 = OR256(ANDNOT256(mask_2, row2), r3_c2);
		row3 = OR256(ANDNOT256(mask_2, row3), r2_c2);

		/* SWAP cell 0 of row 2 with cell 1 of row 3 */
		auto r2_c0 = PERM_4x64(AND256(row2, mask_0), 0b11100001);
		auto r3_c1 = PERM_4x64(AND256(row3, mask_1), 0b11100001);
		row2 = OR256(ANDNOT256(mask_0, row2), r3_c1);
		row3 = OR256(ANDNOT256(mask_1, row3), r2_c0);


		STORE256(state->segments256[2] + i, state->segments256[0][i]);
		STORE256(state->segments256[3] + i, state->segments256[1][i]);
		STORE256(state->segments256[0] + i, row2);
		STORE256(state->segments256[1] + i, row3);
	}
	
	#else
	//	HalfState64Sliced_t top = (*state).halves[0];
	//	State64Sliced_t temp = State64Sliced_t{.halves = {{}, top}};
	//	auto testtemp = unslice128_accelerated_internal(&temp).values[0].raw;
	
	HalfState64Sliced_t copy = state->halves[0];
	
	state->cells[0] = state->cells[0xE];
	state->cells[1] = state->cells[0x8];
	state->cells[2] = state->cells[0xC];
	state->cells[3] = state->cells[0x9];
	state->cells[4] = state->cells[0xF];
	state->cells[5] = state->cells[0xB];
	state->cells[6] = state->cells[0xA];
	state->cells[7] = state->cells[0xD];
	
	state->halves[1] = copy;
	
	//	state->halves[1] = top;
	#endif
	
	// Input:   0x FEDC BA98 7654 3210
	
	// Erik:    0x 7654 3210 DABF 9C8E
	// Us:      0x 7654 3210 DABF 9C8E

//	auto test_output = unslice64(state).values[0].raw;
//	int appel = 1;
}

static inline void xor_keys64(State64Sliced_t *a, State64Sliced_t *b, State64Sliced_t *out, const int half) {
	if (half == 0 || half == -1) {
		xor_row64(&(a->rows[0]), &(b->rows[0]), &(out->rows[0]));
		xor_row64(&(a->rows[1]), &(b->rows[1]), &(out->rows[1]));
	}
	
	if (half == -1 || half == 1) {
		xor_row64(&(a->rows[2]), &(b->rows[2]), &(out->rows[2]));
		xor_row64(&(a->rows[3]), &(b->rows[3]), &(out->rows[3]));
	}
	
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE_INTERNAL_H
