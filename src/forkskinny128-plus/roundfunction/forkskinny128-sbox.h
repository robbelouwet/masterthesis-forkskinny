#ifndef FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
#define FORKSKINNYPLUS128_FORKSKINNY_SBOX_H

#include "../utils/forkskinny128-datatypes.h"
#include "../utils/segment_slicing128.h"
//#include "immintrin.h"

// for readability:
//#define x0 cell.slices[0].value
//#define x1 cell.slices[1].value
//#define x2 cell.slices[2].value
//#define x3 cell.slices[3].value
//#define x4 cell.slices[4].value
//#define x5 cell.slices[5].value
//#define x6 cell.slices[6].value
//#define x7 cell.slices[7].value
//
//#define y0 state->cells[i].slices[0].value
//#define y1 state->cells[i].slices[1].value
//#define y2 state->cells[i].slices[2].value
//#define y3 state->cells[i].slices[3].value
//#define y4 state->cells[i].slices[4].value
//#define y5 state->cells[i].slices[5].value
//#define y6 state->cells[i].slices[6].value
//#define y7 state->cells[i].slices[7].value

static inline void forkskinny128_sbox(State128Sliced_t *state) {
//	auto blocks = Blocks128_t{.values = {{.raw = {0xEC4AFF517369C667, 0x80}}}};
//	*state = slice(blocks);
	
	#if AVX512_acceleration
	
	#elif AVX2_acceleration
	
	#include <immintrin.h>
	
	#define XOR(a, b) Segment128_t{.lane = _mm256_xor_si256(a.lane, b.lane)}
	#define OR(a, b) Segment128_t{.lane = _mm256_or_si256(a.lane, b.lane)}
	#define TEMP_ONE Segment128_t{.lane= _mm256_set_epi64x(-1, -1, -1, -1)}
	for (int i = 0; i < 4; ++i) {
		Segment128_t x0 = SLICE_SEGMENT_AVX2(state, i, 0);
		Segment128_t x1 = SLICE_SEGMENT_AVX2(state, i, 1);
		Segment128_t x2 = SLICE_SEGMENT_AVX2(state, i, 2);
		Segment128_t x3 = SLICE_SEGMENT_AVX2(state, i, 3);
		Segment128_t x4 = SLICE_SEGMENT_AVX2(state, i, 4);
		Segment128_t x5 = SLICE_SEGMENT_AVX2(state, i, 5);
		Segment128_t x6 = SLICE_SEGMENT_AVX2(state, i, 6);
		Segment128_t x7 = SLICE_SEGMENT_AVX2(state, i, 7);
		
		// @formatter:off
		auto y6 = XOR( x4 , XOR( OR( x7 , x6 ), TEMP_ONE ) );
		auto y5 = XOR( x0 , XOR( OR( x2 , x3 ), TEMP_ONE ) );
		auto y7 = XOR( x5 , XOR( OR( y6 , y5 ), TEMP_ONE ) );
		
		auto y4 = XOR( x3 , XOR( OR( y7 , y6 ), TEMP_ONE ) );
		auto y3 = XOR( x1 , XOR( OR( y5 , x3 ), TEMP_ONE ) );
		auto y2 = XOR( x6 , XOR( OR( x1 , x2 ), TEMP_ONE ) );
		auto y1 = XOR( x7 , XOR( OR( y7 , y2 ), TEMP_ONE ) );
		auto y0 = XOR( x2 , XOR( OR( y1 , y3 ), TEMP_ONE ) );
		// @formatter:on
		
		UNSLICE_SEGMENT_AVX2(y0, i, 0, state);
		UNSLICE_SEGMENT_AVX2(y1, i, 1, state);
		UNSLICE_SEGMENT_AVX2(y2, i, 2, state);
		UNSLICE_SEGMENT_AVX2(y3, i, 3, state);
		UNSLICE_SEGMENT_AVX2(y4, i, 4, state);
		UNSLICE_SEGMENT_AVX2(y5, i, 5, state);
		UNSLICE_SEGMENT_AVX2(y6, i, 6, state);
		UNSLICE_SEGMENT_AVX2(y7, i, 7, state);
		
	}
	
	#else
	
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		// @formatter:off
		auto before = _rdtsc();
		y6 = XOR_SLICE( x4 , XOR_SLICE( OR_SLICE( x7 , x6 ), ONE ) );
		y5 = XOR_SLICE( x0 , XOR_SLICE( OR_SLICE( x2 , x3 ), ONE ) );
		y7 = XOR_SLICE( x5 , XOR_SLICE( OR_SLICE( y6 , y5 ), ONE ) );

		y4 = XOR_SLICE( x3 , XOR_SLICE( OR_SLICE( y7 , y6 ), ONE ) );
		y3 = XOR_SLICE( x1 , XOR_SLICE( OR_SLICE( y5 , x3 ), ONE ) );
		y2 = XOR_SLICE( x6 , XOR_SLICE( OR_SLICE( x1 , x2 ), ONE ) );
		y1 = XOR_SLICE( x7 , XOR_SLICE( OR_SLICE( y7 , y2 ), ONE ) );
		y0 = XOR_SLICE( x2 , XOR_SLICE( OR_SLICE( y1 , y3 ), ONE ) );
//		auto after = _rdtsc();
//		std::cout << "1 s box: " << after - before << " cycles";
//		exit(0);
		// @formatter:on
	}
	
	#endif
	
	// INPUT:   0x FEDC BA98 7654 3210 | 0x 0000 0000 0000 0080 ---- 0xFE = 0b 1111 1110
	// Erik :   0x 079C FF4A C5B1 87AD | 0x 6565 6565 6565 6536 ---- 0x07 = 0b 0000 0111
	// Us:      0x 086A FF9A 134A B656 | 0x
//	auto test_res0 = unslice(*state).values[0].raw[0];
//	auto test_res1 = unslice(*state).values[0].raw[1];
//	int banaan = 1;
}

static inline void forkskinny128_sbox_inv(State128Sliced_t *state) {
//	auto blocks = Blocks128_t{.values = {0xF7E4D583B2A1096C}};
//	*state = slice_t(blocks);
//	int appel = 1;
	
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		/*
		y2 = x0 ^ ~(x1 & x3);
		y7 = x1 ^ ~(x7 & x2);
		y5 = x7 ^ ~(x6 & x5);
		y3 = x4 ^ ~(x7 & x6);
		y1 = x3 ^ ~(x5 & y3);
		y6 = x2 ^ ~(y1 & y2);
		y0 = x5 ^ ~(y2 & y3);
		y4 = x6 ^ ~(y7 & y6);
		*/
		
		y2 = XOR_SLICE(x0, XOR_SLICE(OR_SLICE(x1, x3), ONE));
		y7 = XOR_SLICE(x1, XOR_SLICE(OR_SLICE(x7, x2), ONE));
		y5 = XOR_SLICE(x7, XOR_SLICE(OR_SLICE(x6, x5), ONE));
		y3 = XOR_SLICE(x4, XOR_SLICE(OR_SLICE(x7, x6), ONE));
		y1 = XOR_SLICE(x3, XOR_SLICE(OR_SLICE(x5, y3), ONE));
		y6 = XOR_SLICE(x2, XOR_SLICE(OR_SLICE(y1, y2), ONE));
		y0 = XOR_SLICE(x5, XOR_SLICE(OR_SLICE(y2, y3), ONE));
		y4 = XOR_SLICE(x6, XOR_SLICE(OR_SLICE(y7, y6), ONE));
	}
	
	// INPUT:   0x F7E4 D583 B2A1 096C
	// OUTPUT:  0x FEDC BA98 7654 3210
//	auto test_res = unslice(*state).values[0].raw;
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
