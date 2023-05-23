#ifndef FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
#define FORKSKINNYPLUS128_FORKSKINNY_SBOX_H

#include "../utils/forkskinny128-datatypes.h"
#include "../utils/segment_slicing128.h"
#include "../utils/slicing128.h"

// for readability:
#define in0 cell.slices[0].value
#define in1 cell.slices[1].value
#define in2 cell.slices[2].value
#define in3 cell.slices[3].value
#define in4 cell.slices[4].value
#define in5 cell.slices[5].value
#define in6 cell.slices[6].value
#define in7 cell.slices[7].value

#define out0 state->cells[i].slices[0].value
#define out1 state->cells[i].slices[1].value
#define out2 state->cells[i].slices[2].value
#define out3 state->cells[i].slices[3].value
#define out4 state->cells[i].slices[4].value
#define out5 state->cells[i].slices[5].value
#define out6 state->cells[i].slices[6].value
#define out7 state->cells[i].slices[7].value

#define XOR_AVX2 _mm256_xor_si256
#define OR_AVX2 _mm256_or_si256
#define ONE_AVX2 _mm256_set_epi64x(-1, -1, -1, -1)

#if AVX512_acceleration
#define XOR_AVX512 _mm512_xor_si512
#define OR_AVX512 _mm512_or_si512
#define ONE_AVX512 _mm512_set_epi64(-1, -1, -1, -1, -1, -1, -1, -1)
#endif

static inline void forkskinny128_sbox(State128Sliced_t *state) {
//	auto blocks = Blocks128_t{.values = {{.raw = {0xFEDCBA9876543210, 0x80}}}};
//	*state = slice_internal(blocks);
	
//	#if AVX512_acceleration
//	for (int i = 0; i < 2; ++i) {
//		Segment128_t x0 = SLICE_SEGMENT_AVX512(state, i, 0);
//		Segment128_t x1 = SLICE_SEGMENT_AVX512(state, i, 1);
//		Segment128_t x2 = SLICE_SEGMENT_AVX512(state, i, 2);
//		Segment128_t x3 = SLICE_SEGMENT_AVX512(state, i, 3);
//		Segment128_t x4 = SLICE_SEGMENT_AVX512(state, i, 4);
//		Segment128_t x5 = SLICE_SEGMENT_AVX512(state, i, 5);
//		Segment128_t x6 = SLICE_SEGMENT_AVX512(state, i, 6);
//		Segment128_t x7 = SLICE_SEGMENT_AVX512(state, i, 7);
//
//		// @formatter:off
//		auto y6 = XOR_AVX512( x4 , XOR_AVX512( OR_AVX512( x7 , x6 ), ONE_AVX512 ) );
//		auto y5 = XOR_AVX512( x0 , XOR_AVX512( OR_AVX512( x2 , x3 ), ONE_AVX512 ) );
//		auto y7 = XOR_AVX512( x5 , XOR_AVX512( OR_AVX512( y6 , y5 ), ONE_AVX512 ) );
//
//		auto y4 = XOR_AVX512( x3 , XOR_AVX512( OR_AVX512( y7 , y6 ), ONE_AVX512 ) );
//		auto y3 = XOR_AVX512( x1 , XOR_AVX512( OR_AVX512( y5 , x3 ), ONE_AVX512 ) );
//		auto y2 = XOR_AVX512( x6 , XOR_AVX512( OR_AVX512( x1 , x2 ), ONE_AVX512 ) );
//		auto y1 = XOR_AVX512( x7 , XOR_AVX512( OR_AVX512( y7 , y2 ), ONE_AVX512 ) );
//		auto y0 = XOR_AVX512( x2 , XOR_AVX512( OR_AVX512( y1 , y3 ), ONE_AVX512 ) );
//		// @formatter:on
//
//		UNSLICE_SEGMENT_AVX512(y0, i, 0, state);
//		UNSLICE_SEGMENT_AVX512(y1, i, 1, state);
//		UNSLICE_SEGMENT_AVX512(y2, i, 2, state);
//		UNSLICE_SEGMENT_AVX512(y3, i, 3, state);
//		UNSLICE_SEGMENT_AVX512(y4, i, 4, state);
//		UNSLICE_SEGMENT_AVX512(y5, i, 5, state);
//		UNSLICE_SEGMENT_AVX512(y6, i, 6, state);
//		UNSLICE_SEGMENT_AVX512(y7, i, 7, state);
//	}
//	#elif AVX2_acceleration
//	for (int i = 0; i < 16; i += 4) {
//		auto before0 = _rdtsc();
//		__m256i x0 = SLICE_SEGMENT_AVX2(state, i, 0);
//		__m256i x1 = SLICE_SEGMENT_AVX2(state, i, 1);
//		__m256i x2 = SLICE_SEGMENT_AVX2(state, i, 2);
//		__m256i x3 = SLICE_SEGMENT_AVX2(state, i, 3);
//		__m256i x4 = SLICE_SEGMENT_AVX2(state, i, 4);
//		__m256i x5 = SLICE_SEGMENT_AVX2(state, i, 5);
//		__m256i x6 = SLICE_SEGMENT_AVX2(state, i, 6);
//		__m256i x7 = SLICE_SEGMENT_AVX2(state, i, 7);
//
//
//		// @formatter:off
//		auto y6 = XOR_AVX2( x4 , XOR_AVX2( OR_AVX2( x7 , x6 ), ONE_AVX2 ) );
//		auto y5 = XOR_AVX2( x0 , XOR_AVX2( OR_AVX2( x2 , x3 ), ONE_AVX2 ) );
//		auto y7 = XOR_AVX2( x5 , XOR_AVX2( OR_AVX2( y6 , y5 ), ONE_AVX2 ) );
//
//		auto y4 = XOR_AVX2( x3 , XOR_AVX2( OR_AVX2( y7 , y6 ), ONE_AVX2 ) );
//		auto y3 = XOR_AVX2( x1 , XOR_AVX2( OR_AVX2( y5 , x3 ), ONE_AVX2 ) );
//		auto y2 = XOR_AVX2( x6 , XOR_AVX2( OR_AVX2( x1 , x2 ), ONE_AVX2 ) );
//		auto y1 = XOR_AVX2( x7 , XOR_AVX2( OR_AVX2( y7 , y2 ), ONE_AVX2 ) );
//		auto y0 = XOR_AVX2( x2 , XOR_AVX2( OR_AVX2( y1 , y3 ), ONE_AVX2 ) );
//		// @formatter:on
//
//
//		UNSLICE_SEGMENT_AVX2(y0, i, 0, state);
//		UNSLICE_SEGMENT_AVX2(y1, i, 1, state);
//		UNSLICE_SEGMENT_AVX2(y2, i, 2, state);
//		UNSLICE_SEGMENT_AVX2(y3, i, 3, state);
//		UNSLICE_SEGMENT_AVX2(y4, i, 4, state);
//		UNSLICE_SEGMENT_AVX2(y5, i, 5, state);
//		UNSLICE_SEGMENT_AVX2(y6, i, 6, state);
//		UNSLICE_SEGMENT_AVX2(y7, i, 7, state);
		
//		auto after0 = _rdtsc();
//		std::cout << "4 sboxes: " << after0 - before0 << " cycles" << &y1 << "\n";
//		exit(0);
//	}
	
////	auto before0 = _rdtsc();
//	for (int i = 0; i < 16; ++i) {
//		auto cell = state->cells[i];
//
//		// @formatter:off
////		auto before = _rdtsc();
//		out6 = XOR_SLICE( in4 , XOR_SLICE( OR_SLICE( in7  , in6 ),  ONE ) );
//		out5 = XOR_SLICE( in0 , XOR_SLICE( OR_SLICE( in2  , in3 ),  ONE ) );
//		out7 = XOR_SLICE( in5 , XOR_SLICE( OR_SLICE( out6 , out5 ), ONE ) );
//
//		out4 = XOR_SLICE( in3 , XOR_SLICE( OR_SLICE( out7 , out6 ), ONE ) );
//		out3 = XOR_SLICE( in1 , XOR_SLICE( OR_SLICE( out5 , in3  ), ONE ) );
//		out2 = XOR_SLICE( in6 , XOR_SLICE( OR_SLICE( in1  , in2  ), ONE ) );
//		out1 = XOR_SLICE( in7 , XOR_SLICE( OR_SLICE( out7 , out2 ), ONE ) );
//		out0 = XOR_SLICE( in2 , XOR_SLICE( OR_SLICE( out1 , out3 ), ONE ) );
//		auto after = _rdtsc();
//		std::cout << "1 s box: " << after - before << " cycles";
//		exit(0);
		// @formatter:on
//	}
//	auto after0 = _rdtsc();
//	std::cout << "16 sboxes: " << after0 - before0 << " cycles\n";
//	exit(0);

//	#else
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		// @formatter:off
//		auto before = _rdtsc();
		out6 = XOR_SLICE( in4 , XOR_SLICE( OR_SLICE( in7  , in6 ),  ONE ) );
		out5 = XOR_SLICE( in0 , XOR_SLICE( OR_SLICE( in2  , in3 ),  ONE ) );
		out7 = XOR_SLICE( in5 , XOR_SLICE( OR_SLICE( out6 , out5 ), ONE ) );

		out4 = XOR_SLICE( in3 , XOR_SLICE( OR_SLICE( out7 , out6 ), ONE ) );
		out3 = XOR_SLICE( in1 , XOR_SLICE( OR_SLICE( out5 , in3  ), ONE ) );
		out2 = XOR_SLICE( in6 , XOR_SLICE( OR_SLICE( in1  , in2  ), ONE ) );
		out1 = XOR_SLICE( in7 , XOR_SLICE( OR_SLICE( out7 , out2 ), ONE ) );
		out0 = XOR_SLICE( in2 , XOR_SLICE( OR_SLICE( out1 , out3 ), ONE ) );
//		auto after = _rdtsc();
//		std::cout << "1 s box: " << after - before << " cycles";
//		exit(0);
		// @formatter:on
	}
//	#endif
	
	// INPUT:   0x FEDC BA98 7654 3210 | 0x 0000 0000 0000 0080 ---- 0xFE = 0b 1111 1110
	// OUTP :   0x DF57 DA56 C44D A835 | 0x 6565 6565 6565 6536 ---- 0x07 = 0b 0000 0111
//	auto test_res0 = unslice_accelerated_internal(*state).values[0].raw[0];
//	auto test_res1 = unslice_accelerated_internal(*state).values[0].raw[1];
//	int banaan = 1;
}

static inline void forkskinny128_sbox_inv(State128Sliced_t *state) {
//	auto blocks = Blocks128_t{.values = {0xDF57DA56C44DA835, 0x6565656565656536}};
//	*state = slice_internal(blocks);

//	#if AVX512_acceleration
//	for (int i = 0; i < 4; ++i) {
//		Segment128_t x0 = SLICE_SEGMENT_AVX512(state, i, 0);
//		Segment128_t x1 = SLICE_SEGMENT_AVX512(state, i, 1);
//		Segment128_t x2 = SLICE_SEGMENT_AVX512(state, i, 2);
//		Segment128_t x3 = SLICE_SEGMENT_AVX512(state, i, 3);
//		Segment128_t x4 = SLICE_SEGMENT_AVX512(state, i, 4);
//		Segment128_t x5 = SLICE_SEGMENT_AVX512(state, i, 5);
//		Segment128_t x6 = SLICE_SEGMENT_AVX512(state, i, 6);
//		Segment128_t x7 = SLICE_SEGMENT_AVX512(state, i, 7);
//
//		auto y2 = XOR_AVX512(x0, XOR_AVX512(OR_AVX512(x1, x3), ONE_AVX512));
//		auto y7 = XOR_AVX512(x1, XOR_AVX512(OR_AVX512(x7, x2), ONE_AVX512));
//		auto y5 = XOR_AVX512(x7, XOR_AVX512(OR_AVX512(x6, x5), ONE_AVX512));
//		auto y3 = XOR_AVX512(x4, XOR_AVX512(OR_AVX512(x7, x6), ONE_AVX512));
//		auto y1 = XOR_AVX512(x3, XOR_AVX512(OR_AVX512(x5, y3), ONE_AVX512));
//		auto y6 = XOR_AVX512(x2, XOR_AVX512(OR_AVX512(y1, y2), ONE_AVX512));
//		auto y0 = XOR_AVX512(x5, XOR_AVX512(OR_AVX512(y2, y3), ONE_AVX512));
//		auto y4 = XOR_AVX512(x6, XOR_AVX512(OR_AVX512(y7, y6), ONE_AVX512));
//
//		UNSLICE_SEGMENT_AVX512(y0, i, 0, state);
//		UNSLICE_SEGMENT_AVX512(y1, i, 1, state);
//		UNSLICE_SEGMENT_AVX512(y2, i, 2, state);
//		UNSLICE_SEGMENT_AVX512(y3, i, 3, state);
//		UNSLICE_SEGMENT_AVX512(y4, i, 4, state);
//		UNSLICE_SEGMENT_AVX512(y5, i, 5, state);
//		UNSLICE_SEGMENT_AVX512(y6, i, 6, state);
//		UNSLICE_SEGMENT_AVX512(y7, i, 7, state);
//	}
//	#elif AVX2_acceleration
//	for (int i = 0; i < 16; i += 4) {
//		__m256i x0 = SLICE_SEGMENT_AVX2(state, i, 0);
//		__m256i x1 = SLICE_SEGMENT_AVX2(state, i, 1);
//		__m256i x2 = SLICE_SEGMENT_AVX2(state, i, 2);
//		__m256i x3 = SLICE_SEGMENT_AVX2(state, i, 3);
//		__m256i x4 = SLICE_SEGMENT_AVX2(state, i, 4);
//		__m256i x5 = SLICE_SEGMENT_AVX2(state, i, 5);
//		__m256i x6 = SLICE_SEGMENT_AVX2(state, i, 6);
//		__m256i x7 = SLICE_SEGMENT_AVX2(state, i, 7);
//
//		auto y2 = XOR_AVX2(x0, XOR_AVX2(OR_AVX2(x1, x3), ONE_AVX2));
//		auto y7 = XOR_AVX2(x1, XOR_AVX2(OR_AVX2(x7, x2), ONE_AVX2));
//		auto y5 = XOR_AVX2(x7, XOR_AVX2(OR_AVX2(x6, x5), ONE_AVX2));
//		auto y3 = XOR_AVX2(x4, XOR_AVX2(OR_AVX2(x7, x6), ONE_AVX2));
//		auto y1 = XOR_AVX2(x3, XOR_AVX2(OR_AVX2(x5, y3), ONE_AVX2));
//		auto y6 = XOR_AVX2(x2, XOR_AVX2(OR_AVX2(y1, y2), ONE_AVX2));
//		auto y0 = XOR_AVX2(x5, XOR_AVX2(OR_AVX2(y2, y3), ONE_AVX2));
//		auto y4 = XOR_AVX2(x6, XOR_AVX2(OR_AVX2(y7, y6), ONE_AVX2));
//
//		UNSLICE_SEGMENT_AVX2(y0, i, 0, state);
//		UNSLICE_SEGMENT_AVX2(y1, i, 1, state);
//		UNSLICE_SEGMENT_AVX2(y2, i, 2, state);
//		UNSLICE_SEGMENT_AVX2(y3, i, 3, state);
//		UNSLICE_SEGMENT_AVX2(y4, i, 4, state);
//		UNSLICE_SEGMENT_AVX2(y5, i, 5, state);
//		UNSLICE_SEGMENT_AVX2(y6, i, 6, state);
//		UNSLICE_SEGMENT_AVX2(y7, i, 7, state);
//	}
//	#else
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		out2 = XOR_SLICE(in0, XOR_SLICE(OR_SLICE(in1, in3), ONE));
		out7 = XOR_SLICE(in1, XOR_SLICE(OR_SLICE(in7, in2), ONE));
		out5 = XOR_SLICE(in7, XOR_SLICE(OR_SLICE(in6, in5), ONE));
		out3 = XOR_SLICE(in4, XOR_SLICE(OR_SLICE(in7, in6), ONE));
		out1 = XOR_SLICE(in3, XOR_SLICE(OR_SLICE(in5, out3), ONE));
		out6 = XOR_SLICE(in2, XOR_SLICE(OR_SLICE(out1, out2), ONE));
		out0 = XOR_SLICE(in5, XOR_SLICE(OR_SLICE(out2, out3), ONE));
		out4 = XOR_SLICE(in6, XOR_SLICE(OR_SLICE(out7, out6), ONE));
	}
//	#endif
	
	
	// INPUT:   0x DF57 DA56 C44D A835
	// OUTPUT:  0x FEDC BA98 7654 3210
//	auto test_res = unslice_accelerated_internal(*state).values[0];
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
