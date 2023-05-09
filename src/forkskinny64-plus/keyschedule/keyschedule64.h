#ifndef FORKSKINNYPLUS64_KEYSCHEDULE_H
#define FORKSKINNYPLUS64_KEYSCHEDULE_H

#include "internal64.h"
#include "../utils/forkskinny64-datatypes.h"
#include "../roundfunction/forkskinny64-addconstant.h"

static inline KeySchedule64Sliced_t forkskinny_64_init_tk2(State64Sliced_t *tk1, State64Sliced_t *tk2) {
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		auto res = xor_half_keys(tk2->halves[0], tk1->halves[0]);
		//auto test_tks = unslice_accelerated(res);
		
		forkskinny64_add_constant(&res, i);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		schedule.keys[i] = res;
		
		// Permute TK's
		permute(tk1);
		permute(tk2);
		
		// LFSR TK2 & TK3
		tk2_lfsr(tk2);
	}
	
	return schedule;
}

/// EXPECTS UNSLICED TK STATES
static inline KeySchedule64Sliced_t forkskinny_64_init_tk23(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                            State64Sliced_t *tk3) {
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		auto res = xor_half_keys(xor_half_keys(tk2->halves[0], tk3->halves[0]), tk1->halves[0]);
		//auto test_tks = unslice_accelerated(res);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		forkskinny64_add_segmented_constant(&res, i); // 1, 3, 7
		schedule.keys[i] = res;
		
		// 0x660075E2,
//		auto round_key = unslice_accelerated({.halves = {schedule.keys[i], {}}}).values[0].raw;
		
		// Permute TK's
		permute(tk1);
		permute(tk2);
		permute(tk3);
		
		// LFSR TK2 & TK3
//		auto utk2_before = unslice_accelerated(tk2).values[0].raw;  // 0x660055F20FF10001
		tk2_lfsr(tk2);
//		auto utk2 = unslice_accelerated(tk2).values[0].raw;  // 0x660055F20FF10001
		tk3_lfsr(tk3);
//		auto utk3 = unslice_accelerated(tk3).values[0].raw;  // 0xAA0099F3055E000E
//		int appel = 1;
	}
	
	return schedule;
}


#endif //FORKSKINNYPLUS64_KEYSCHEDULE_H
