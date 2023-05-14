#ifndef FORKSKINNYPLUS64_FORKSKINNY_MIXCOLS_H
#define FORKSKINNYPLUS64_FORKSKINNY_MIXCOLS_H

#include "../utils/forkskinny64-datatypes.h"
#include "../common.h"

static void inline assign_segmented_row(uint8_t from, uint8_t to, State64Sliced_t *state){
	state->segments256[to][0] = state->segments256[from][0];
	state->segments256[to][1] = state->segments256[from][1];
	state->segments256[to][2] = state->segments256[from][2];
	state->segments256[to][3] = state->segments256[from][3];
}

static inline void forkskinny64_mixcols_inv(State64Sliced_t *state) {
//	auto ct = Blocks64_t{.values = {0xF88AC3CD8DDFADDF}};
//	*state = Slice64_t(ct);
	
	
	#if AVX2_acceleration || AVX512_acceleration
	auto temp0 = state->segments256[3][0];
	auto temp1 = state->segments256[3][1];
	auto temp2 = state->segments256[3][2];
	auto temp3 = state->segments256[3][3];
	
	assign_segmented_row(0, 3, state);
	assign_segmented_row(1, 0, state);
	assign_segmented_row(2, 1, state);
	
	state->segments256[3][0] = XOR256(state->segments256[3][0], temp0);
	state->segments256[3][1] = XOR256(state->segments256[3][1], temp1);
	state->segments256[3][2] = XOR256(state->segments256[3][2], temp2);
	state->segments256[3][3] = XOR256(state->segments256[3][3], temp3);
	
	state->segments256[2][0] = XOR256(state->segments256[0][0], temp0);
	state->segments256[2][1] = XOR256(state->segments256[0][1], temp1);
	state->segments256[2][2] = XOR256(state->segments256[0][2], temp2);
	state->segments256[2][3] = XOR256(state->segments256[0][3], temp3);
	
	xor_segmented_row(1, 2, 1, state);
	
	#else
	auto temp = state->rows[3];
	state->rows[3] = state->rows[0];
	state->rows[0] = state->rows[1];
	state->rows[1] = state->rows[2];
	
	
	xor_row(&(state->rows[3]), &temp, &(state->rows[3]));
	xor_row(&temp, &(state->rows[0]), &(state->rows[2]));
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));
	#endif

//	auto test_res = unslice_accelerated(*state).values[0].raw;
//	int appel = 1;
}

static inline void forkskinny64_mixcols(State64Sliced_t *state) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0xFEDCBA9876543210;
//	*state = slice_accelerated(&test_blocks);
	
	#if AVX512_acceleration || AVX2_acceleration
	xor_segmented_row(1, 2, 1, state);
	xor_segmented_row(2, 0, 2, state);
	
	auto temp0 = XOR256(state->segments256[3][0], state->segments256[2][0]);
	auto temp1 = XOR256(state->segments256[3][1], state->segments256[2][1]);
	auto temp2 = XOR256(state->segments256[3][2], state->segments256[2][2]);
	auto temp3 = XOR256(state->segments256[3][3], state->segments256[2][3]);
	
	xor_segmented_row(2, 3, 3, state);
	assign_segmented_row(2, 3, state);
	assign_segmented_row(1, 2, state);
	assign_segmented_row(0, 1, state);
	
	state->segments256[0][0] = temp0;
	state->segments256[0][1] = temp1;
	state->segments256[0][2] = temp2;
	state->segments256[0][3] = temp3;
	
	#else
	xor_row(state->rows + 1, state->rows + 2, state->rows + 1);
	xor_row(state->rows + 2, state->rows + 0, state->rows + 2);
	
	Row64_t temp;
	xor_row(state->rows + 3, state->rows + 2, &temp);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
	#endif
	
	// 0x F88A C3CD 8DDF ADDF
//	forkskinny64_mixcols_inv(state);
//	auto test_res = unslice_accelerated(state).values[0].raw;
//	int appel = 1;
}

#endif //FORKSKINNYPLUS64_FORKSKINNY_MIXCOLS_H
