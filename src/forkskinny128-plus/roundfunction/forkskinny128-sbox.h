#ifndef FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
#define FORKSKINNYPLUS128_FORKSKINNY_SBOX_H

#include "../utils/forkskinny128-datatypes.h"
#include "../utils/slicing128-internal.h"
#include "../common.h"
#include "../utils/slicing128.h"

#define in0 cell.slices[0].value
#define in1 cell.slices[1].value
#define in2 cell.slices[2].value
#define in3 cell.slices[3].value
#define in4 cell.slices[4].value
#define in5 cell.slices[5].value
#define in6 cell.slices[6].value
#define in7 cell.slices[7].value

static inline void forkskinny128_sbox(State128Sliced_t *state) {
	#if AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		auto r6 = XOR256( s4 , XOR256( OR256( s7 , s6 ), ONE256 ) );
		auto r5 = XOR256( s0 , XOR256( OR256( s2 , s3 ), ONE256 ) );
		auto r7 = XOR256( s5 , XOR256( OR256( r6 , r5 ), ONE256 ) );
		
		auto r4 = XOR256( s3 , XOR256( OR256( r7 , r6 ), ONE256 ) );
		auto r3 = XOR256( s1 , XOR256( OR256( r5 , s3 ), ONE256 ) );
		auto r2 = XOR256( s6 , XOR256( OR256( s1 , s2 ), ONE256 ) );
		auto r1 = XOR256( s7 , XOR256( OR256( r7 , r2 ), ONE256 ) );
		auto r0 = XOR256( s2 , XOR256( OR256( r1 , r3 ), ONE256 ) );
		
		// @formetter:off
		s0 = r0; s1 = r1; s2 = r2; s3 = r3; s4 = r4; s5 = r5; s6 = r6; s7 = r7;
		// @formatter:on
	}
	#else
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		x6 = XOR_SLICE(in4, XOR_SLICE(OR_SLICE(in7, in6), slice_ONE));
		x5 = XOR_SLICE(in0, XOR_SLICE(OR_SLICE(in2, in3), slice_ONE));
		x7 = XOR_SLICE(in5, XOR_SLICE(OR_SLICE(x6, x5), slice_ONE));
		
		x4 = XOR_SLICE(in3, XOR_SLICE(OR_SLICE(x7, x6), slice_ONE));
		x3 = XOR_SLICE(in1, XOR_SLICE(OR_SLICE(x5, in3), slice_ONE));
		x2 = XOR_SLICE(in6, XOR_SLICE(OR_SLICE(in1, in2), slice_ONE));
		x1 = XOR_SLICE(in7, XOR_SLICE(OR_SLICE(x7, x2), slice_ONE));
		x0 = XOR_SLICE(in2, XOR_SLICE(OR_SLICE(x1, x3), slice_ONE));
	}
	#endif
	// 0x 079C FF4A C5B1 87AD || 0x 6565 6565 6565 6536
//	auto test_output = unslice128(state).values[0];
//	int appel = 1;
}

static inline void forkskinny128_sbox_inv(State128Sliced_t *state) {
//	auto blocks = Blocks128_t{.values = {0xDF57DA56C44DA835, 0x6565656565656536}};
//	*state = slice128_internal(blocks);
	
	#if AVX2_acceleration
	for (int i = 0; i < 4; ++i) {
		auto r2 = XOR256(s0, XOR256(OR256(s1, s3), ONE256));
		auto r7 = XOR256(s1, XOR256(OR256(s7, s2), ONE256));
		auto r5 = XOR256(s7, XOR256(OR256(s6, s5), ONE256));
		auto r3 = XOR256(s4, XOR256(OR256(s7, s6), ONE256));
		auto r1 = XOR256(s3, XOR256(OR256(s5, r3), ONE256));
		auto r6 = XOR256(s2, XOR256(OR256(r1, r2), ONE256));
		auto r0 = XOR256(s5, XOR256(OR256(r2, r3), ONE256));
		auto r4 = XOR256(s6, XOR256(OR256(r7, r6), ONE256));
		
		// @formatter:off
		s0 = r0; s1 = r1; s2 = r2; s3 = r3; s4 = r4; s5 = r5; s6 = r6; s7 = r7;
		// @formatter:on
		
	}
	#else
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		x2 = XOR_SLICE(in0, XOR_SLICE(OR_SLICE(in1, in3), slice_ONE));
		x7 = XOR_SLICE(in1, XOR_SLICE(OR_SLICE(in7, in2), slice_ONE));
		x5 = XOR_SLICE(in7, XOR_SLICE(OR_SLICE(in6, in5), slice_ONE));
		x3 = XOR_SLICE(in4, XOR_SLICE(OR_SLICE(in7, in6), slice_ONE));
		x1 = XOR_SLICE(in3, XOR_SLICE(OR_SLICE(in5, x3), slice_ONE));
		x6 = XOR_SLICE(in2, XOR_SLICE(OR_SLICE(x1, x2), slice_ONE));
		x0 = XOR_SLICE(in5, XOR_SLICE(OR_SLICE(x2, x3), slice_ONE));
		x4 = XOR_SLICE(in6, XOR_SLICE(OR_SLICE(x7, x6), slice_ONE));
	}
	#endif
	
	// 0x C33D 7F3A 10E7 5E5A || 0x 698B 1AD8 1DF4 5CED
//	auto test_res = unslice128(state).values[0];
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
