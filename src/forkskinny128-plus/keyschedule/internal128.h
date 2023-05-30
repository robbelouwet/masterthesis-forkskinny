#ifndef FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H
#define FORKSKINNYPLUS128_KEYSCHEDULE_INTERNAL_H

//#include <immintrin.h>
#include "../utils/forkskinny128-datatypes.h"
#include "../common.h"

static inline void tk2_lfsr(State128Sliced_t *state, const bool full_state = false) {
	auto bound = full_state ? 4 : 2;
	#if AVX2_acceleration
	#else
	for (int i = 0; i < (bound << 2); i++) {
		auto temp = state->cells[i].slices[7];
		state->cells[i].slices[7] = state->cells[i].slices[6];
		state->cells[i].slices[6] = state->cells[i].slices[5];
		state->cells[i].slices[5] = state->cells[i].slices[4];
		state->cells[i].slices[4] = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[0];
		state->cells[i].slices[0].value = XOR_SLICE(temp.value, state->cells[i].slices[6].value);
	}
	#endif
}

static inline void tk3_lfsr(State128Sliced_t *state, const bool full_state = false) {
	auto bound = full_state ? 4 : 2;
	#if AVX2_acceleration
	#else
	for (int i = 0; i < (bound << 2); i++) {
		auto temp = state->cells[i].slices[0];
		state->cells[i].slices[0] = state->cells[i].slices[1];
		state->cells[i].slices[1] = state->cells[i].slices[2];
		state->cells[i].slices[2] = state->cells[i].slices[3];
		state->cells[i].slices[3] = state->cells[i].slices[4];
		state->cells[i].slices[4] = state->cells[i].slices[5];
		state->cells[i].slices[5] = state->cells[i].slices[6];
		state->cells[i].slices[6] = state->cells[i].slices[7];
		state->cells[i].slices[7].value = XOR_SLICE(temp.value, state->cells[i].slices[5].value);
	}
	#endif
}

static inline void permute(State128Sliced_t *input) {
//	auto test_blocks = Blocks128_t();
//	test_blocks.values[0].raw[0] = 0x7766554433221100;
//	test_blocks.values[0].raw[1] = 0xffeeddccbbaa9988;
//	input = slice_internal(test_blocks);

	
	auto copy = input->halves[0];
	
	input->cells[0] = input->cells[0x9];
	input->cells[1] = input->cells[0xF];
	input->cells[2] = input->cells[0x8];
	input->cells[3] = input->cells[0xD];
	input->cells[4] = input->cells[0xA];
	input->cells[5] = input->cells[0xE];
	input->cells[6] = input->cells[0xC];
	input->cells[7] = input->cells[0xB];
	
	input->halves[1] = copy;
	
	// Input:   0x FEDC BA98 7654 3210
	
	// Erik:    0x 7654 3210 DABF 9C8E
	// Us:      0x 7654 3210 DABF 9C8E

//	auto res = unslice_accelerated_internal(output).values[0];
//	auto test_output0 = res.raw[0];
//	auto test_output1 = res.raw[1];
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
