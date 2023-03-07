#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_H

#include "keyschedule64-internal.h"
#include "../utils/skinny64_datatypes.h"
#include "../roundfunction/skinny64-addconstant.h"

//static inline KeySchedule64Sliced_t skinny64_precompute_64_key_schedules(State64Sliced_t tk1, State64Sliced_t tk2,
//                                                                         State64Sliced_t tk3) {
//	auto schedule = KeySchedule64Sliced_t();
//
//	for (auto &key: schedule.keys) {
//		// Extract RTK
//		auto res = xor_keys(tk2, tk3);
//		res = xor_keys(res, tk1);
//		//auto test_tks = unslice(res);
//
//		//auto test_res = unslice(res);
//
//		key = res.halves[0];
//
//		// Permute TK's
//		tk1 = permute(tk1);
//		tk2 = permute(tk2);
//		tk3 = permute(tk3);
//
//		// LFSR TK2 & TK3
//		tk2_lfsr_simd(&tk2);
//		tk3_lfsr_simd(&tk3);
//	}
//	return schedule;
//}

static inline KeySchedule64Sliced_t forkskinny_64_init_tk1(State64Sliced_t tk1) {
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY_ROUNDS_BEFORE + 2 * FORKSKINNY_ROUNDS_AFTER; ++i) {
		forkskinny64_add_constant(&tk1, i);
		schedule.keys[i] = tk1.halves[0];
		tk1 = permute(tk1);
	}
	
	return schedule;
}

static inline KeySchedule64Sliced_t forkskinny_64_init_tk12(State64Sliced_t tk1, State64Sliced_t tk2) {
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY_ROUNDS_BEFORE + 2 * FORKSKINNY_ROUNDS_AFTER; ++i) {
		auto res = xor_keys(tk2, tk1);
		//auto test_tks = unslice(res);
		
		forkskinny64_add_constant(&res, i);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		schedule.keys[i] = res.halves[0];
		
		// Permute TK's
		tk2 = permute(tk2);
		tk1 = permute(tk1);
		
		// LFSR TK2 & TK3
		tk2_lfsr_simd(&tk2);
	}
	
	return schedule;
};

static inline KeySchedule64Sliced_t forkskinny_64_init_tk123(State64Sliced_t tk1, State64Sliced_t tk2,
                                                             State64Sliced_t tk3) {
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY_ROUNDS_BEFORE + 2 * FORKSKINNY_ROUNDS_AFTER; ++i) {
		auto res = xor_keys(xor_keys(tk2, tk3), tk1);
		//auto test_tks = unslice(res);
		
		forkskinny64_add_constant(&res, i);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		schedule.keys[i] = res.halves[0];
		
		// 0x76541200, 0xa57e182c, 0xb46f097d
		//auto round_key = unslice({.halves = {schedule.keys[i], {}}}).values[0].raw;
		
		// Permute TK's
		tk2 = permute(tk2);
		tk3 = permute(tk3);
		tk1 = permute(tk1);
		
		// LFSR TK2 & TK3
		tk2_lfsr_simd(&tk2);
		tk3_lfsr_simd(&tk3);
	}
	
	return schedule;
}


#endif //FORKSKINNYPLUS_KEYSCHEDULE64_H
