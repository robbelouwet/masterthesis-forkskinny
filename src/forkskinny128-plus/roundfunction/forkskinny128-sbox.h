#ifndef FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
#define FORKSKINNYPLUS128_FORKSKINNY_SBOX_H

#include "../utils/forkskinny128-datatypes.h"

// for readability:
#define x0 cell.slices[0].value
#define x1 cell.slices[1].value
#define x2 cell.slices[2].value
#define x3 cell.slices[3].value
#define x4 cell.slices[4].value
#define x5 cell.slices[5].value
#define x6 cell.slices[6].value
#define x7 cell.slices[7].value

#define y0 state->cells[i].slices[0].value
#define y1 state->cells[i].slices[1].value
#define y2 state->cells[i].slices[2].value
#define y3 state->cells[i].slices[3].value
#define y4 state->cells[i].slices[4].value
#define y5 state->cells[i].slices[5].value
#define y6 state->cells[i].slices[6].value
#define y7 state->cells[i].slices[7].value

static inline void skinny128_sbox(State128Sliced_t *state) {
//	auto blocks = Blocks128_t{.values = {0xFEDCBA9876543210}};
//	*state = slice_t(blocks);
//	int appel = 1;
	
	for (int i = 0; i < 16; ++i) {
		auto cell = state->cells[i];
		
		// @formatter:off
		y6 = XOR_SLICE( x4 , XOR_SLICE( AND_SLICE( x7 , x6 ), ONE ) );
		y5 = XOR_SLICE( x0 , XOR_SLICE( AND_SLICE( x2 , x3 ), ONE ) );
		y7 = XOR_SLICE( x5 , XOR_SLICE( AND_SLICE( y6 , y5 ), ONE ) );
		
		y4 = XOR_SLICE( x3 , XOR_SLICE( AND_SLICE( y7 , y6 ), ONE ) );
		y3 = XOR_SLICE( x1 , XOR_SLICE( AND_SLICE( y5 , x3 ), ONE ) );
		y2 = XOR_SLICE( x6 , XOR_SLICE( AND_SLICE( x1 , x2 ), ONE ) );
		y1 = XOR_SLICE( x7 , XOR_SLICE( AND_SLICE( y7 , y2 ), ONE ) );
		y0 = XOR_SLICE( x2 , XOR_SLICE( AND_SLICE( y1 , y3 ), ONE ) );
		// @formatter:on
	}
	
	// INPUT:   0x FEDC BA98 7654 3210
	// OUTPUT:  0x F7E4 D583 B2A1 096C
//	auto test_res = unslice(*state).values[0].raw;
//	int banaan = 1;
}

static inline void skinny128_sbox_inv(State128Sliced_t *state) {
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
		
		y2 = XOR_SLICE( x0 , XOR_SLICE( AND_SLICE( x1 , x3 ), ONE ) );
		y7 = XOR_SLICE( x1 , XOR_SLICE( AND_SLICE( x7 , x2 ), ONE ) );
		y5 = XOR_SLICE( x7 , XOR_SLICE( AND_SLICE( x6 , x5 ), ONE ) );
		y3 = XOR_SLICE( x4 , XOR_SLICE( AND_SLICE( x7 , x6 ), ONE ) );
		y1 = XOR_SLICE( x3 , XOR_SLICE( AND_SLICE( x5 , y3 ), ONE ) );
		y6 = XOR_SLICE( x2 , XOR_SLICE( AND_SLICE( y1 , y2 ), ONE ) );
		y0 = XOR_SLICE( x5 , XOR_SLICE( AND_SLICE( y2 , y3 ), ONE ) );
		y4 = XOR_SLICE( x6 , XOR_SLICE( AND_SLICE( y7 , y6 ), ONE ) );
	}
	
	// INPUT:   0x F7E4 D583 B2A1 096C
	// OUTPUT:  0x FEDC BA98 7654 3210
//	auto test_res = unslice(*state).values[0].raw;
//	int banaan = 1;
}

#endif //FORKSKINNYPLUS128_FORKSKINNY_SBOX_H
