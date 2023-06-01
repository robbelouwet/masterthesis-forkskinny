#ifndef FORKSKINNYPLUS64_KEYSCHEDULE_H
#define FORKSKINNYPLUS64_KEYSCHEDULE_H

#include "internal64.h"
#include "../utils/forkskinny64-datatypes.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "fixsliced-keyschedule64.h"

static inline void forkskinny_64_init_tk2_internal(State64Sliced_t *tk1, State64Sliced_t *tk2, const char mode,
                                                   KeySchedule64Sliced_t *out) {
	// don't compute the full ks when only calculating C0
	auto bound = (mode == 'b' || mode == '1')
	             ? (FORKSKINNY64_MAX_ROUNDS)
	             : (FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER);
	
	for (int i = 0; i < bound; ++i) {
		State64Sliced_t res;
		xor_keys(tk1, tk2, &res, 0);
		//auto test_tks = unslice_accelerated_internal(res);

//		forkskinny64_add_constant(&(res.halves[0]), i);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		out->keys[i] = res.halves[0];
		
		// Permute TK's
		permute(tk1);
		permute(tk2);
		
		// LFSR TK2 & TK3
		tk2_lfsr(tk2);
	}
}

static inline void forkskinny_64_init_tk23_internal(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                    State64Sliced_t *tk3, const char mode, KeySchedule64Sliced_t *out) {
	
	// don't compute the full ks when only calculating C0
	auto bound = (mode == 'b' || mode == '1')
	             ? (FORKSKINNY64_MAX_ROUNDS)
	             : (FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER);
	
	for (int i = 0; i < bound; ++i) {
		State64Sliced_t res;
		xor_keys(tk2, tk3, &res, 0);
		xor_keys(tk1, &res, &res, 0);
		
		// 0x FCDA B896 EE00 DDF0
//		auto test_tks = unslice_accelerated_internal(res).values[0].raw;
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
//		forkskinny64_add_constant(&(res.halves[0]), i);
		
		out->keys[i] = res.halves[0];
		
		// 0x FCDA B896 EE00 FDE0
//		auto test_tks2 = unslice_accelerated_internal(res).values[0].raw;
		
		permute(tk1);
		permute(tk2);
		permute(tk3);
		
		tk2_lfsr(tk2);
		tk3_lfsr(tk3);

//		auto test_tk1 = unslice_accelerated_internal(*tk1).values[0].raw; // 0x 2200 11F1 0334 0004
//		auto test_tk2 = unslice_accelerated_internal(*tk2).values[0].raw; // 0x 6600 55F2 0FF1 0001
//		auto test_tk3 = unslice_accelerated_internal(*tk3).values[0].raw; // 0x AA00 99F3 055E 000E
//
//		int appel = 1;
	}
}

static inline void forkskinny64_precompute_key_schedule(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                        State64Sliced_t *tk3, const char mode, KeySchedule64Sliced_t *out) {
	#if IMPROVED_KEYSCHEDULE
	forkskinny_64_init_tk23_fixsliced_internal(tk1, tk2, tk3, out);
	#else
	forkskinny_64_init_tk23_internal(tk1, tk2, tk3, mode, out);
	#endif
}

static inline void forkskinny64_precompute_key_schedule(State64Sliced_t *tk1, State64Sliced_t *tk2, const char mode,
                                                        KeySchedule64Sliced_t *out) {
	#if IMPROVED_KEYSCHEDULE
	forkskinny_64_init_tk2_fixsliced_internal(tk1, tk2, out);
	#else
	forkskinny_64_init_tk2_internal(tk1, tk2, mode, out);
	#endif
}

#endif //FORKSKINNYPLUS64_KEYSCHEDULE_H
