#ifndef FORKSKINNYPLUS64_TK_PERMUTE_H
#define FORKSKINNYPLUS_TK_PERMUTE_H

#include "../headers/forkskinny64-plus.h"

// tables for fixedsliced keyschedule
// uint8_t pt_0[] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};
uint8_t pt_2[] = {1, 7, 0, 5, 2, 6, 4, 3, 9, 15, 8, 13, 10, 14, 12, 11};
uint8_t pt_4[] = {7, 3, 1, 6, 0, 4, 2, 5, 15, 11, 9, 14, 8, 12, 10, 13};
uint8_t pt_6[] = {3, 5, 7, 4, 1, 2, 0, 6, 11, 13, 15, 12, 9, 10, 8, 14};
uint8_t pt_8[] = {5, 6, 3, 2, 7, 0, 1, 4, 13, 14, 11, 10, 15, 8, 9, 12};
uint8_t pt_10[] = {6, 4, 5, 0, 3, 1, 7, 2, 14, 12, 13, 8, 11, 9, 15, 10};
uint8_t pt_12[] = {4, 2, 6, 1, 5, 7, 3, 0, 12, 10, 14, 9, 13, 15, 11, 8};
uint8_t pt_14[] = {2, 0, 4, 7, 6, 3, 5, 1, 10, 8, 12, 15, 14, 11, 13, 9};

static inline State64Sliced_t tk_permute(State64Sliced_t state) {
	auto permuted_state = State64Sliced_t();
	
	for (int i = 0; i < 16; ++i)
		permuted_state.cells[i] = state.cells[pt[i]];
	
	return permuted_state;
}

#endif //FORKSKINNYPLUS64_TK_PERMUTE_H
