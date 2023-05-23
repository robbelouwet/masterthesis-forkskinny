#ifndef FORKSKINNYPLUS64_FORKSKINNY_SBOX_H
#define FORKSKINNYPLUS64_FORKSKINNY_SBOX_H

#include "../utils/forkskinny64-datatypes.h"

#define x0 cell.slices[0].value
#define x1 cell.slices[1].value
#define x2 cell.slices[2].value
#define x3 cell.slices[3].value

#define y0 state->cells[i].slices[0].value
#define y1 state->cells[i].slices[1].value
#define y2 state->cells[i].slices[2].value
#define y3 state->cells[i].slices[3].value

#define s0 state->segments256[i][0]
#define s1 state->segments256[i][1]
#define s2 state->segments256[i][2]
#define s3 state->segments256[i][3]

static inline void forkskinny64_sbox_inv(State64Sliced_t *state) {
//	auto blocks = Blocks64_t{.values = {0xF7E4D583B2A1096C}};
//	*state = Slice64_t(blocks);
//	int appel = 1;
	#if AVX2_acceleration || AVX512_acceleration
	for (int i = 0; i < 4; ++i) {
		auto r1 = XOR256(s0, XOR256(OR256(s3, s2), ONE256));
		auto r2 = XOR256(s1, XOR256(OR256(r1, s3), ONE256));
		auto r3 = XOR256(s2, XOR256(OR256(r2, r1), ONE256));
		auto r0 = XOR256(s3, XOR256(OR256(r3, r2), ONE256));
		
		s0 = r0;
		s1 = r1;
		s2 = r2;
		s3 = r3;
	}
	#else
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];

		y1 = XOR_SLICE(x0, XOR_SLICE(OR_SLICE(x3, x2), slice_ONE));
		y2 = XOR_SLICE(x1, XOR_SLICE(OR_SLICE(y1, x3), slice_ONE));
		y3 = XOR_SLICE(x2, XOR_SLICE(OR_SLICE(y2, y1), slice_ONE));
		y0 = XOR_SLICE(x3, XOR_SLICE(OR_SLICE(y3, y2), slice_ONE));
	}
	#endif
	
	// INPUT:   0x F7E4 D583 B2A1 096C
	// OUTPUT:  0x FEDC BA98 7654 3210
//	auto test_res = unslice_accelerated_internal(*state).values[0].raw;
//	int banaan = 1;
}

static inline void forkskinny64_sbox(State64Sliced_t *state) {
//	auto blocks = Blocks64_t{.values = {0xFEDCBA9876543210}};
//	*state = slice_accelerated_internal(&blocks);
//	int appel = 1;
	
	#if AVX512_acceleration || AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		auto r3 = XOR256(s0, XOR256(OR256(s3, s2), ONE256));
		auto r2 = XOR256(s3, XOR256(OR256(s2, s1), ONE256));
		auto r1 = XOR256(s2, XOR256(OR256(s1, r3), ONE256));
		auto r0 = XOR256(s1, XOR256(OR256(r3, r2), ONE256));
		
		s0 = r0;
		s1 = r1;
		s2 = r2;
		s3 = r3;
	}
	#else
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		// @formatter:off
		y3 = XOR_SLICE( x0 , XOR_SLICE( OR_SLICE(x3 , x2 ) , slice_ONE ));
		y2 = XOR_SLICE( x3 , XOR_SLICE( OR_SLICE(x2 , x1 ) , slice_ONE ));
		y1 = XOR_SLICE( x2 , XOR_SLICE( OR_SLICE(x1 , y3 ) , slice_ONE ));
		y0 = XOR_SLICE( x1 , XOR_SLICE( OR_SLICE(y3 , y2 ) , slice_ONE ));
		// @formatter:on
	}
	#endif
	
	// INPUT:   0x FEDC BA98 7654 3210
	// OUTPUT:  0x F7E4 D583 B2A1 096C
//	forkskinny64_sbox_inv(state);
//	auto test_res = unslice_accelerated_internal(state).values[0].raw;
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS64_FORKSKINNY_SBOX_H
