#ifndef FORKSKINNYPLUS128_KEYSCHEDULE_H
#define FORKSKINNYPLUS128_KEYSCHEDULE_H

#include "keyschedule128-internal.h"
#include "../utils/forkskinny128-datatypes.h"
#include "../roundfunction/forkskinny128-addconstant.h"
#include "common128.h"

static inline KeySchedule128Sliced_t forkskinny_128_init_tk2(State128Sliced_t tk1, State128Sliced_t tk2) {
	auto schedule = KeySchedule128Sliced_t();
	
	for (int i = 0; i < FORKSKINNY128_MAX_ROUNDS; ++i) {
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

static inline KeySchedule128Sliced_t forkskinny_128_init_tk23(State128Sliced_t tk1, State128Sliced_t tk2,
                                                              State128Sliced_t tk3) {
	auto schedule = KeySchedule128Sliced_t();
	
	for (int i = 0; i < FORKSKINNY128_MAX_ROUNDS; ++i) {
		// TK 3:
		
		auto res = xor_half_keys(xor_half_keys(tk2.halves[0], tk3.halves[0]), tk1.halves[0]);
		//auto test_tks = unslice(res);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		forkskinny64_add_constant(&res, i);
		schedule.keys[i] = res;
		
		// 0x76541200, 0xa57e182c, 0xb46f097d
		auto round_key = unslice({.halves = {schedule.keys[i], {}}}).values[0].raw[0];
		
		// Permute TK's
		tk1 = permute(tk1);
		// TK 2: 9AC99F33632E5A76
		auto tk2_unsliced_0 = unslice(tk2).values[0].raw[0]; // 0x9AC99F33632E5A76
		auto tk2_unsliced_1 = unslice(tk2).values[0].raw[1]; // 0x1805831B70D3266
		tk2 = permute(tk2);
		auto tk2_unsliced_r_0_p = unslice(tk2).values[0].raw[0]; // 0xB01A6E026563CD01
		auto tk2_unsliced_r_1_p = unslice(tk2).values[0].raw[1]; // 0x9AC99F33632E5A76
		tk3 = permute(tk3);
		
		// LFSR TK2 & TK3
		tk2_lfsr(&tk2);
		auto utk2 = unslice(tk2).values[0].raw[0];  // 0x6E63011AB0CD0265
		tk3_lfsr(&tk3);
		auto utk3 = unslice(tk3).values[0].raw[0];  // 0xFC8D73AAF4BEC6E1

		// TK 2: 6E63011AB0CD0265
		// TK 3:
		int appel = 1;
	}
	
	return schedule;
}


#endif //FORKSKINNYPLUS128_KEYSCHEDULE_H
