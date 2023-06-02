#ifndef FORKSKINNYPLUS64_FORKSKINNY_SBOX_H
#define FORKSKINNYPLUS64_FORKSKINNY_SBOX_H

#include "../../constants.h"
#include "../utils/forkskinny64-datatypes.h"

static inline void forkskinny64_sbox_inv(State64Sliced_t *state) {
//	auto blocks = Blocks64_t{.values = {0xF7E4D583B2A1096C}};
//	*state = Slice64_t(blocks);
//	int appel = 1;
	#if AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		auto r1 = XOR256(s0, XOR256(OR256(s3, s2), ONE256));
		auto r2 = XOR256(s1, XOR256(OR256(r1, s3), ONE256));
		auto r3 = XOR256(s2, XOR256(OR256(r2, r1), ONE256));
		auto r0 = XOR256(s3, XOR256(OR256(r3, r2), ONE256));
		
		// @formatter:off
		s0 = r0; s1 = r1; s2 = r2; s3 = r3;
		// @formatter:on
	}
	#else
	for (int i = 0; i < 16; ++i) {
		auto r1 = XOR_SLICE(x0, XOR_SLICE(OR_SLICE(x3, x2), slice_ONE));
		auto r2 = XOR_SLICE(x1, XOR_SLICE(OR_SLICE(r1, x3), slice_ONE));
		auto r3 = XOR_SLICE(x2, XOR_SLICE(OR_SLICE(r2, r1), slice_ONE));
		auto r0 = XOR_SLICE(x3, XOR_SLICE(OR_SLICE(r3, r2), slice_ONE));
		
		// @formatter:off
		x0 = r0; x1 = r1; x2 = r2; x3 = r3;
		// @formatter:on
	}
	#endif
	
	// INPUT:   0x F7E4 D583 B2A1 096C
	// OUTPUT:  0x FEDC BA98 7654 3210
//	auto test_res = unslice128_accelerated_internal(*state).values[0].raw;
//	int banaan = 1;
}

static inline void forkskinny64_sbox(State64Sliced_t *state) {
//	auto blocks = Blocks64_t{.values = {0xFEDCBA9876543210}};
//	*state = slice128_accelerated_internal(&blocks);
//	int appel = 1;
	
	#if AVX2_acceleration
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
		// @formatter:off
		auto r3 = XOR_SLICE( x0 , XOR_SLICE( OR_SLICE(x3 , x2 ) , slice_ONE ));
		auto r2 = XOR_SLICE( x3 , XOR_SLICE( OR_SLICE(x2 , x1 ) , slice_ONE ));
		auto r1 = XOR_SLICE( x2 , XOR_SLICE( OR_SLICE(x1 , r3 ) , slice_ONE ));
		auto r0 = XOR_SLICE( x1 , XOR_SLICE( OR_SLICE(r3 , r2 ) , slice_ONE ));
		// @formatter:on
		
		// @formatter:off
		x0 = r0; x1 = r1; x2 = r2; x3 = r3;
		// @formatter:on
	}
	#endif
	
	// INPUT:   0x FEDC BA98 7654 3210
	// OUTPUT:  0x F7E4 D583 B2A1 096C
//	forkskinny64_sbox_inv(state);
//	auto test_res = unslice128_accelerated_internal(state).values[0].raw;
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS64_FORKSKINNY_SBOX_H
