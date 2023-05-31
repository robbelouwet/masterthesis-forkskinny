#ifndef FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H
#define FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H

#include "../utils/forkskinny128-datatypes.h"
#include "../common.h"

static inline void forkskinny128_mixcols(State128Sliced_t *state) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw = 0x55557555B6988DDF;
//	*state = slice_internal(test_blocks);
	
	/// Because segmentation is applied to rows, .segments256[i] and .rows[i] are
	/// bound within the same memory region within the state's union struct.
	/// Because of this, moving the byte array represented by .rows[i] (for some i in 0, 1, 2, 3) results
	/// in moving the exact same bytes when moving .segments[j] (where i == j)
	
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));
	xor_row(&(state->rows[2]), &(state->rows[0]), &(state->rows[2]));
	
	Row128_t temp;
	xor_row(&(state->rows[3]), &(state->rows[2]), &temp);
	state->rows[3] = state->rows[2];
	state->rows[2] = state->rows[1];
	state->rows[1] = state->rows[0];
	state->rows[0] = temp;
	
	// 0xF88AC3CD8DDFADDF
//	auto test_res = unslice_accelerated_internal(*state).values[0].raw;
//	int appel = 1;
}

static inline void forkskinny128_mixcols_inv(State128Sliced_t *state) {
//	auto ct = Blocks128_t{.values = {0xF88AC3CD8DDFADDF}};
//	*state = slice_t(ct);
	
	/// Because segmentation is applied to rows, .segments256[i] and .rows[i] are
	/// bound within the same memory region within the state's union struct.
	/// Because of this, moving the byte array represented by .rows[i] (for some i in 0, 1, 2, 3) results
	/// in moving the exact same bytes when moving .segments[j] (where i == j)
	
	Row128_t temp = state->rows[3];
	
	state->rows[3] = state->rows[0];
	state->rows[0] = state->rows[1];
	state->rows[1] = state->rows[2];

	xor_row(&(state->rows[3]), &temp, &(state->rows[3]));
	xor_row(&(state->rows[0]), &temp, &(state->rows[2]));
	xor_row(&(state->rows[1]), &(state->rows[2]), &(state->rows[1]));

	// F5AA F1BC 72D8 8741 || 0x 51B1 2898 54B5 23CF
//	auto test_res = unslice128(*state).values[0];
//	int appel = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_MIXCOLS_H
