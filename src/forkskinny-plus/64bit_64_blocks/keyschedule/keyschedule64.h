#ifndef FORKSKINNYPLUS_KEYSCHEDULE64_H
#define FORKSKINNYPLUS_KEYSCHEDULE64_H

#include "keyschedule64-internal.h"
#include "../utils/skinny64_datatypes.h"

static inline KeySchedule64Sliced_t precompute_64_key_schedules(State64Sliced_t tk1, State64Sliced_t tk2,
                                                                State64Sliced_t tk3) {
	
	// TODO: get rid of lookup table to make constant-time, perform permutation manually
	uint8_t const table[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		// Extract RTK
		auto res = xor_keys(tk1, tk2);
		res = xor_keys(res, tk3);
		schedule.keys[i] = res.halves[0];
		
		// Update TK2
		tk2 = permute(tk2, table);
		tk2_lfsr_simd(&tk2);
		
		// Update TK3
		tk3 = permute(tk3, table);
		tk3_lfsr_simd(&tk3);
	}
	return schedule;
}

#endif //FORKSKINNYPLUS_KEYSCHEDULE64_H
