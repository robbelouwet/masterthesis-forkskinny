#ifndef FORKSKINNYPLUS64_FORKSKINNY_MIXCOLS_H
#define FORKSKINNYPLUS64_FORKSKINNY_MIXCOLS_H

#include "../utils/forkskinny64-datatypes.h"
#include "../common.h"

static void inline xor_segmented_row(uint8_t a, uint8_t b, uint8_t out_i, State64Sliced_t *state) {
	state->segments256[out_i][0] = XOR256(state->segments256[a][0], state->segments256[b][0]);
	state->segments256[out_i][1] = XOR256(state->segments256[a][1], state->segments256[b][1]);
	state->segments256[out_i][2] = XOR256(state->segments256[a][2], state->segments256[b][2]);
	state->segments256[out_i][3] = XOR256(state->segments256[a][3], state->segments256[b][3]);
}

static void inline assign_segmented_row(uint8_t from, uint8_t to, State64Sliced_t *state){
	state->segments256[to][0] = state->segments256[from][0];
	state->segments256[to][1] = state->segments256[from][1];
	state->segments256[to][2] = state->segments256[from][2];
	state->segments256[to][3] = state->segments256[from][3];
}

static inline void skinny64_mixcols(State64Sliced_t *state) {
//	auto test_blocks = Blocks64_t();
//	test_blocks.values[0].raw = 0x55557555B6988DDF;
//	*state = slice(test_blocks);

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
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);
	state->rows[2] = xor_row(state->rows[2], state->rows[0]);
	
	auto temp = xor_row(state->rows[3], state->rows[2]);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
	#endif
	
	// 0xF88AC3CD8DDFADDF
//	auto test_res = unslice_accelerated(*state).values[0].raw;
//	int appel = 1;
}

static inline void skinny64_mixcols_inv(State64Sliced_t *state) {
//	auto ct = Blocks64_t{.values = {0xF88AC3CD8DDFADDF}};
//	*state = Slice64_t(ct);
	
	auto temp = state->rows[3];
	state->rows[3] = state->rows[0];
	state->rows[0] = state->rows[1];
	state->rows[1] = state->rows[2];
	
	
	state->rows[3] = xor_row(state->rows[3], temp);
	state->rows[2] = xor_row(temp, state->rows[0]);
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);

//	auto test_res = unslice_accelerated(*state).values[0].raw;
//	int appel = 1;
}

#endif //FORKSKINNYPLUS64_FORKSKINNY_MIXCOLS_H
