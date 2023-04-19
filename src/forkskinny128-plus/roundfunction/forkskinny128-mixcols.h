#ifndef FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H
#define FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H

#include "../utils/forkskinny128-datatypes.h"
#include "../common.h"

static inline void forkskinny128_mixcols(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw = 0x55557555B6988DDF;
//	*state = slice(test_blocks);
	
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);
	state->rows[2] = xor_row(state->rows[2], state->rows[0]);
	
	auto temp = xor_row(state->rows[3], state->rows[2]);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
	
	// 0xF88AC3CD8DDFADDF
//	auto test_res = unslice(*state).values[0].raw;
//	int appel = 1;
}

static inline void forkskinny128_mixcols_inv(State128Sliced_t *state) {
//	auto ct = Blocks128_t{.values = {0xF88AC3CD8DDFADDF}};
//	*state = slice_t(ct);
	
	auto temp = state->rows[3];
	state->rows[3] = state->rows[0];
	state->rows[0] = state->rows[1];
	state->rows[1] = state->rows[2];
	
	
	state->rows[3] = xor_row(state->rows[3], temp);
	state->rows[2] = xor_row(temp, state->rows[0]);
	state->rows[1] = xor_row(state->rows[1], state->rows[2]);

//	auto test_res = unslice(*state).values[0].raw;
//	int appel = 1;
}

#endif //FORKSKINNYPLUS_FORKSKINNY_MIXCOLS_H
