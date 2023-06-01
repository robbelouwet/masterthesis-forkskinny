#ifndef FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H
#define FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H

#include "../utils/forkskinny128-datatypes.h"
#include "../common.h"

#if AVX2_acceleration
static void inline assign_segmented_row(uint8_t from, uint8_t to, State128Sliced_t *state){
	for (int i = 0; i < 8; ++i)
		state->segments256[to][i] = state->segments256[from][i];
}
#endif

static inline void forkskinny128_mixcols(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw = 0x55557555B6988DDF;
//	*state = slice_internal(test_blocks);
	
	#if AVX2_acceleration
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));
	xor_row(&(state->rows[2]), &(state->rows[0]), &(state->rows[2]));
	
	auto temp0 = XOR256(state->segments256[3][0], state->segments256[2][0]);
	auto temp1 = XOR256(state->segments256[3][1], state->segments256[2][1]);
	auto temp2 = XOR256(state->segments256[3][2], state->segments256[2][2]);
	auto temp3 = XOR256(state->segments256[3][3], state->segments256[2][3]);
	auto temp4 = XOR256(state->segments256[3][4], state->segments256[2][4]);
	auto temp5 = XOR256(state->segments256[3][5], state->segments256[2][5]);
	auto temp6 = XOR256(state->segments256[3][6], state->segments256[2][6]);
	auto temp7 = XOR256(state->segments256[3][7], state->segments256[2][7]);
	
	xor_row(&(state->rows[2]), &(state->rows[3]), &(state->rows[3]));
	assign_segmented_row(2, 3, state);
	assign_segmented_row(1, 2, state);
	assign_segmented_row(0, 1, state);
	
	state->segments256[0][0] = temp0;
	state->segments256[0][1] = temp1;
	state->segments256[0][2] = temp2;
	state->segments256[0][3] = temp3;
	state->segments256[0][4] = temp4;
	state->segments256[0][5] = temp5;
	state->segments256[0][6] = temp6;
	state->segments256[0][7] = temp7;
	#else
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));
	xor_row(&(state->rows[2]), &(state->rows[0]), &(state->rows[2]));
	
	Row128_t temp;
	xor_row(&(state->rows[3]), &(state->rows[2]), &temp);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
	#endif
	
	// 0x A69D DDDA A6CC DDDA || 0x C3A9 B8BF 3054 1CF8
//	auto test_res = unslice128(state).values[0];
//	int appel = 1;
}

static inline void forkskinny128_mixcols_inv(State128Sliced_t *state) {
//	auto ct = Blocks128_t{.values = {0xF88AC3CD8DDFADDF}};
//	*state = slice_t(ct);
	// 0x 72D8 8741 77DC 8C16 || 0x 266D A48E A11F D273
//	auto test_res_before = unslice128(*state).values[0];
	
	
	#if AVX2_acceleration
	auto temp0 = state->segments256[3][0];
	auto temp1 = state->segments256[3][1];
	auto temp2 = state->segments256[3][2];
	auto temp3 = state->segments256[3][3];
	auto temp4 = state->segments256[3][4];
	auto temp5 = state->segments256[3][5];
	auto temp6 = state->segments256[3][6];
	auto temp7 = state->segments256[3][7];
	
	assign_segmented_row(0, 3, state);
	assign_segmented_row(1, 0, state);
	assign_segmented_row(2, 1, state);
	
	state->segments256[3][0] = XOR256(state->segments256[3][0], temp0);
	state->segments256[3][1] = XOR256(state->segments256[3][1], temp1);
	state->segments256[3][2] = XOR256(state->segments256[3][2], temp2);
	state->segments256[3][3] = XOR256(state->segments256[3][3], temp3);
	state->segments256[3][4] = XOR256(state->segments256[3][4], temp4);
	state->segments256[3][5] = XOR256(state->segments256[3][5], temp5);
	state->segments256[3][6] = XOR256(state->segments256[3][6], temp6);
	state->segments256[3][7] = XOR256(state->segments256[3][7], temp7);
	
	state->segments256[2][0] = XOR256(state->segments256[0][0], temp0);
	state->segments256[2][1] = XOR256(state->segments256[0][1], temp1);
	state->segments256[2][2] = XOR256(state->segments256[0][2], temp2);
	state->segments256[2][3] = XOR256(state->segments256[0][3], temp3);
	state->segments256[2][4] = XOR256(state->segments256[0][4], temp4);
	state->segments256[2][5] = XOR256(state->segments256[0][5], temp5);
	state->segments256[2][6] = XOR256(state->segments256[0][6], temp6);
	state->segments256[2][7] = XOR256(state->segments256[0][7], temp7);
	
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));
	#else
	Row128_t temp = state->rows[3];
	
	state->rows[3] = state->rows[0];
	state->rows[0] = state->rows[1];
	state->rows[1] = state->rows[2];
	
	xor_row(&(state->rows[3]), &temp, &(state->rows[3]));
	xor_row(&(state->rows[0]), &temp, &(state->rows[2]));
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));
	#endif
	
	// F5AA F1BC 72D8 8741 || 0x 51B1 2898 54B5 23CF
//	auto test_res = unslice128(*state).values[0];
//	int appel = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H
