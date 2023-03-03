#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_H

#include "keyschedule64-internal.h"
#include "../utils/skinny64_datatypes.h"
#include "../roundfunction/skinny64-addconstant.h"

static inline KeySchedule64Sliced_t skinny64_precompute_64_key_schedules(State64Sliced_t tk1, State64Sliced_t tk2,
                                                                         State64Sliced_t tk3) {
	
	// TODO: get rid of lookup table to make constant-time, perform permutation manually
	uint8_t const table[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};
	auto schedule = KeySchedule64Sliced_t();
	
	for (auto & key : schedule.keys) {
		// Extract RTK
		auto res = xor_keys(tk2, tk3);
		res = xor_keys(res, tk1);
		//auto test_tks = unslice(res);
		
		//auto test_res = unslice(res);

		key = res.halves[0];
		
		// Permute TK's
		tk1 = permute(tk1, table);
		tk2 = permute(tk2, table);
		tk3 = permute(tk3, table);
		
		// LFSR TK2 & TK3
		tk2_lfsr_simd(&tk2);
		tk3_lfsr_simd(&tk3);
	}
	return schedule;
}

static inline KeySchedule64Sliced_t forkskinny64_precompute_64_key_schedules(State64Sliced_t tk1, State64Sliced_t tk2,
                                                                         State64Sliced_t tk3) {
	
	// TODO: get rid of lookup table to make constant-time, perform permutation manually
	uint8_t const table[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};
	auto schedule = KeySchedule64Sliced_t();
	
	// R_init and R_Left
	int i = 0;
	for (; i < FORKSKINNY_ROUNDS_BEFORE + 2 * FORKSKINNY_ROUNDS_AFTER; ++i) {
		// Extract RTK
		auto res = xor_keys(tk2, tk3);
		//res = xor_keys(res, tk1);
		//auto test_tks = unslice(res);
		
		//auto test_res = unslice(res);
		forkskinny64_add_constant(&res, i);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		schedule.keys[i] = res.halves[0];
		
		// Permute TK's
		tk1 = permute(tk1, table);
		tk2 = permute(tk2, table);
		tk3 = permute(tk3, table);
		
		// LFSR TK2 & TK3
		tk2_lfsr_simd(&tk2);
		tk3_lfsr_simd(&tk3);
	}
	
	return schedule;
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE64_H
