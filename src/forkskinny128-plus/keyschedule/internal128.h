#ifndef FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H

//#include <immintrin.h>
#include "../utils/forkskinny128-datatypes.h"
#include "../common.h"

static inline void tk2_lfsr(State128Sliced_t *state, const bool full_state = false) {
	auto bound = full_state ? 4 : 2;
	#if AVX2_acceleration
	for (int i = 0; i < bound; ++i) {
		auto temp = s7;
		s7 = s6;
		s6 = s5;
		s5 = s4;
		s4 = s3;
		s3 = s2;
		s2 = s1;
		s1 = s0;
		s0 = XOR256(temp, s6);
	}
	#else
	for (int i = 0; i < (bound << 2); i++) {
		auto temp = x7;
		x7 = x6;
		x6 = x5;
		x5 = x4;
		x4 = x3;
		x3 = x2;
		x2 = x1;
		x1 = x0;
		x0 = XOR_SLICE(temp, x6);
	}
	#endif
}

static inline void tk3_lfsr(State128Sliced_t *state, const bool full_state = false) {
	auto bound = full_state ? 4 : 2;
	#if AVX2_acceleration
	for (int i = 0; i < bound; ++i) {
		auto temp = s0;
		s0 = s1;
		s1 = s2;
		s2 = s3;
		s3 = s4;
		s4 = s5;
		s5 = s6;
		s6 = s7;
		s7 = XOR256(temp, s5);
	}
	#else
	for (int i = 0; i < (bound << 2); i++) {
		auto temp = x0;
		x0 = x1;
		x1 = x2;
		x2 = x3;
		x3 = x4;
		x4 = x5;
		x5 = x6;
		x6 = x7;
		x7 = XOR_SLICE(temp, x5);
	}
	#endif
}

static inline void permute(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw[0] = 0x7766554433221100;
//	test_blocks.values[0].raw[1] = 0xFFEEDDCCBBAA9988;
//	*state = slice128(&test_blocks);
	
	#if AVX2_acceleration
	for (int i = 0; i < 8; ++i) {
		auto row2 = LOADU256(state->segments256[2] + i);
		auto row3 = LOADU256(state->segments256[3] + i);
		
		/* Align row 2 & 3 for easy segment64 swapping */
		row2 = PERM_4x64(row2, 0b11001001);
		row3 = PERM_4x64(row3, 0b01001011);
		
		/* SWAP cell 3 of row 2 & 3 */
		auto r2_c3 = AND256(row2, mask_3);
		auto r3_c3 = AND256(row3, mask_3);
		row2 = OR256(ANDNOT256(mask_3, row2), r3_c3);
		row3 = OR256(ANDNOT256(mask_3, row3), r2_c3);
		
		/* SWAP cell 1 of row 2 with cell 0 of row 3 */
		auto r2_c1 = PERM_4x64(AND256(row2, mask_1), 0b11100001);
		auto r3_c0 = PERM_4x64(AND256(row3, mask_0), 0b11100001);
		row2 = OR256(ANDNOT256(mask_1, row2), r3_c0);
		row3 = OR256(ANDNOT256(mask_0, row3), r2_c1);
		
		
		STOREU256(state->segments256[2] + i, state->segments256[0][i]);
		STOREU256(state->segments256[3] + i, state->segments256[1][i]);
		STOREU256(state->segments256[0] + i, row2);
		STOREU256(state->segments256[1] + i, row3);
	}
	#else
	auto copy = state->halves[0];
	
	state->cells[0] = state->cells[0x9];
	state->cells[1] = state->cells[0xF];
	state->cells[2] = state->cells[0x8];
	state->cells[3] = state->cells[0xD];
	state->cells[4] = state->cells[0xA];
	state->cells[5] = state->cells[0xE];
	state->cells[6] = state->cells[0xC];
	state->cells[7] = state->cells[0xB];
	
	state->halves[1] = copy;
	#endif

	// 0x F81B E754 E87C 8DC2
	// 0x 46E3 FBF2 ABBA CD29
//	auto res = unslice128(state).values[0];
//	int appel = 1;
}

static inline void xor_keys(State128Sliced_t *a, State128Sliced_t *b, State128Sliced_t *out, const int half) {
	if (half == 0 || half == -1) {
		xor_row(&(a->rows[0]), &(b->rows[0]), &(out->rows[0]));
		xor_row(&(a->rows[1]), &(b->rows[1]), &(out->rows[1]));
	}
	
	if (half == -1 || half == 1) {
		xor_row(&(a->rows[2]), &(b->rows[2]), &(out->rows[2]));
		xor_row(&(a->rows[3]), &(b->rows[3]), &(out->rows[3]));
	}
	
}

#endif //FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H
