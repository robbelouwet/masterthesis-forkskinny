#ifndef FORKSKINNYPLUS64_KEYSCHEDULE_H
#define FORKSKINNYPLUS64_KEYSCHEDULE_H

#include "internal64.h"
#include "../utils/forkskinny64-datatypes.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "fixsliced-keyschedule64.h"

static inline KeySchedule64Sliced_t forkskinny_64_init_tk2_internal(State64Sliced_t *tk1, State64Sliced_t *tk2) {
	auto schedule = KeySchedule64Sliced_t();
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		auto res = xor_keys(*tk1, *tk2, 0).halves[0];
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
static inline void forkskinny_64_init_tk23_internal(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                    State64Sliced_t *tk3, KeySchedule64Sliced_t *out) {
	
	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
		auto res = xor_keys(xor_keys(*tk2, *tk3, 0), *tk1, 0).halves[0];
		//auto test_tks = unslice_accelerated(res);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		#if AVX2_acceleration || AVX512_acceleration
		forkskinny64_add_segmented_constant(&res, i); // 1, 3, 7
		#else
		forkskinny64_add_constant(&res, i);
		#endif
		out->keys[i] = res;
		
		permute(tk1);
		permute(tk2);
		permute(tk3);
		
		tk2_lfsr(tk2);
		tk3_lfsr(tk3);
	}
}

static inline void forkskinny64_precompute_key_schedule(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                        State64Sliced_t *tk3, KeySchedule64Sliced_t *out) {
	// acceleration can't be enabled because fixed sliced key schedule doesn't cope with segmented cipher state
	#if FIXED_SLICING && !AVX2_acceleration && !AVX512_acceleration
	forkskinny_64_init_tk23_fixsliced_internal_test(tk1, tk2, tk3, out);
	#else
	forkskinny_64_init_tk23_internal(tk1, tk2, tk3, out);
	#endif
}

#endif //FORKSKINNYPLUS64_KEYSCHEDULE_H
