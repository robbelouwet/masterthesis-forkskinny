#ifndef FORKSKINNYPLUS_KEYSCHEDULE_H
#define FORKSKINNYPLUS_KEYSCHEDULE_H

#include "keyschedule-internal.h"
#include "../utils/forkskinny-datatypes.h"
#include "../roundfunction/forkskinny-addconstant.h"
#include "common.h"

static inline KeyScheduleSliced_t forkskinny_64_init_tk2(StateSliced_t tk1, StateSliced_t tk2) {
	auto schedule = KeyScheduleSliced_t();
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		auto res = xor_half_keys(tk2.halves[0], tk1.halves[0]);
		//auto test_tks = unslice(res);
		
		forkskinny64_add_constant(&res, i);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		schedule.keys[i] = res;
		
		// Permute TK's
		tk1 = permute(tk1);
		tk2 = permute(tk2);
		
		// LFSR TK2 & TK3
		tk2_lfsr(&tk2);
	}
	
	return schedule;
}

static inline KeyScheduleSliced_t forkskinny_64_init_tk23(StateSliced_t tk1, StateSliced_t tk2,
                                                          StateSliced_t tk3) {
	auto schedule = KeyScheduleSliced_t();
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		auto res = xor_half_keys(xor_half_keys(tk2.halves[0], tk3.halves[0]), tk1.halves[0]);
		//auto test_tks = unslice(res);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		forkskinny64_add_constant(&res, i);
		schedule.keys[i] = res;
		
		// 0x76541200, 0xa57e182c, 0xb46f097d
//		auto round_key = unslice({.halves = {schedule.keys[i], {}}}).values[0].raw;
		
		// Permute TK's
		tk1 = permute(tk1);
		tk2 = permute(tk2);
		tk3 = permute(tk3);
		
		// LFSR TK2 & TK3
		tk2_lfsr(&tk2);
//		auto utk2 = unslice(tk2).values[0].raw;  // 0x660055F20FF10001
		tk3_lfsr(&tk3);
//		auto utk3 = unslice(tk3).values[0].raw;  // 0xAA0099F3055E000E
//		int appel = 1;
	}
	
	return schedule;
}


#endif //FORKSKINNYPLUS_KEYSCHEDULE_H
