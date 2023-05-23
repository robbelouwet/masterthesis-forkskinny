#ifndef FORKSKINNYPLUS64_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS64_KEYSCHEDULE_INTERNAL_H

//#include <immintrin.h>
#include <cstring>
#include "../utils/forkskinny64-datatypes.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "../common.h"
#include "../utils/slicing64-accelerated-internal.h"
#include "../utils/slicing64-internal.h"

static inline void tk2_lfsr(State64Sliced_t *state, const bool full_state = false) {
	int bound = full_state ? 4 : 2;
	#if AVX2_acceleration || AVX512_acceleration
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

static inline void tk3_lfsr(State64Sliced_t *state, const bool full_state = false) {
	int bound = full_state ? 4 : 2;
	#if AVX2_acceleration || AVX512_acceleration
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
		state->cells[i].slices[3].value = XOR_SLICE(temp.value, state->cells[i].slices[2].value);
	}
	#endif
}

#if AVX2_support
auto mask_0 = _mm256_set_epi64x(0, 0, 0, -1ULL);
auto mask_1 = _mm256_set_epi64x(0, 0, -1ULL, 0);
auto mask_2 = _mm256_set_epi64x(0, -1ULL, 0, 0);
#endif

/// Make sure you first understand how the nibble-swapped cipher state looks like
static inline void permute(State64Sliced_t *state) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xEFCDAB8967452301;
//	*state = slice_accelerated_internal(&test_blocks);
	
	#if AVX2_acceleration || AVX512_acceleration
	for (int i = 0; i < 4; ++i) {
		auto bit_lane_row_2 = LOADU256(state->segments256[2] + i);
		auto bit_lane_row_3 = LOADU256(state->segments256[3] + i);
		
		// align M_0x8 (cell 9) into cell 3 and M_0x9 (cell 8) into cell 1
		bit_lane_row_2 = _mm256_permute4x64_epi64(bit_lane_row_2, 0b01100011);
		
		// align M_0xE (cell 15) into cell 4 and M_0xC (cell 13) into cell 7
		bit_lane_row_3 = _mm256_permute4x64_epi64(bit_lane_row_3, 0b01001011);
		
		{
			/* SWAP cell 2 of row 2 & 3 */
			auto r2_cell2 = _mm256_and_si256(bit_lane_row_2, mask_2);
			auto r3_cell2 = _mm256_and_si256(bit_lane_row_3, mask_2);
			bit_lane_row_2 = _mm256_or_si256(_mm256_andnot_si256(mask_2, bit_lane_row_2), r3_cell2);
			bit_lane_row_3 = _mm256_or_si256(_mm256_andnot_si256(mask_2, bit_lane_row_3), r2_cell2);
		}
		
		{
			/* SWAP cell 0 of row 2 with cell 1 of row 3 */
			auto r2_masked = _mm256_permute4x64_epi64(_mm256_and_si256(bit_lane_row_2, mask_0), 0b11100001);
			auto r3_masked = _mm256_permute4x64_epi64(_mm256_and_si256(bit_lane_row_3, mask_1), 0b11100001);
			bit_lane_row_2 = _mm256_or_si256(_mm256_andnot_si256(mask_0, bit_lane_row_2), r3_masked);
			bit_lane_row_3 = _mm256_or_si256(_mm256_andnot_si256(mask_1, bit_lane_row_3), r2_masked);
		}
		
		STOREU256(state->segments256[2] + i, state->segments256[0][i]);
		STOREU256(state->segments256[3] + i, state->segments256[1][i]);
		STOREU256(state->segments256[0] + i, bit_lane_row_2);
		STOREU256(state->segments256[1] + i, bit_lane_row_3);
	}
	
	#else
	//	HalfState64Sliced_t top = (*state).halves[0];
	//	State64Sliced_t temp = State64Sliced_t{.halves = {{}, top}};
	//	auto testtemp = unslice_accelerated_internal(&temp).values[0].raw;
	
	auto s_0 = state->cells[0];
	auto s_1 = state->cells[1];
	auto s_2 = state->cells[2];
	auto s_3 = state->cells[3];
	auto s_4 = state->cells[4];
	auto s_5 = state->cells[5];
	auto s_6 = state->cells[6];
	auto s_7 = state->cells[7];
	
	state->cells[0] = state->cells[0xE];
	state->cells[1] = state->cells[0x8];
	state->cells[2] = state->cells[0xC];
	state->cells[3] = state->cells[0x9];
	state->cells[4] = state->cells[0xF];
	state->cells[5] = state->cells[0xB];
	state->cells[6] = state->cells[0xA];
	state->cells[7] = state->cells[0xD];
	
	state->cells[0x8] = s_0;
	state->cells[0x9] = s_1;
	state->cells[0xA] = s_2;
	state->cells[0xB] = s_3;
	state->cells[0xC] = s_4;
	state->cells[0xD] = s_5;
	state->cells[0xE] = s_6;
	state->cells[0xF] = s_7;
	
	//	state->halves[1] = top;
	#endif
	
	// Input:   0x FEDC BA98 7654 3210
	
	// Erik:    0x 7654 3210 DABF 9C8E
	// Us:      0x 7654 3210 DABF 9C8E

//	auto test_output = unslice_accelerated_internal(state).values[0].raw;
//	int appel = 1;
}

static inline void xor_keys(State64Sliced_t *a, State64Sliced_t *b, State64Sliced_t *out, const int half) {
	if (half == 0 || half == -1) {
		xor_row(&(a->rows[0]), &(b->rows[0]), &(out->rows[0]));
		xor_row(&(a->rows[1]), &(b->rows[1]), &(out->rows[1]));
	}
	
	if (half == -1 || half == 1) {
		xor_row(&(a->rows[2]), &(b->rows[2]), &(out->rows[2]));
		xor_row(&(a->rows[3]), &(b->rows[3]), &(out->rows[3]));
	}
	
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE_INTERNAL_H
