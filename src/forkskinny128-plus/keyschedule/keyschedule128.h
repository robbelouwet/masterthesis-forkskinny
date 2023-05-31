#ifndef FORKSKINNYPLUS128_KEYSCHEDULE_H
#define FORKSKINNYPLUS128_KEYSCHEDULE_H

#include "internal128.h"
#include "../utils/forkskinny128-datatypes.h"
#include "../roundfunction/forkskinny128-addconstant.h"
#include "fixsliced-keyschedule128.h"

static inline void forkskinny_128_init_tk2_internal(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                    KeySchedule128Sliced_t *out) {
	for (int i = 0; i < FORKSKINNY128_MAX_ROUNDS; ++i) {
		State128Sliced_t res;
		xor_keys(tk1, tk2, &res, 0);
		//auto test_tks = unslice_accelerated_internal(res);
		
		forkskinny128_add_constant(&(res.halves[0]), i);
		
		// Keep in mind: the C2 constant relating to the 8th cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
//		for (int j = 0; j < 64; ++j) {
//			std::cout << "slice_internal " << j << " -> " << schedule.keys[i].raw[j].value;
//		}
		out->keys[i] = res.halves[0];
//		for (int j = 0; j < 64; ++j) {
//			std::cout << "slice_internal " << j << " -> " << schedule.keys[i].raw[j].value;
//		}
		
		// Permute TK's
		permute(tk1);
		permute(tk2);
		
		// LFSR TK2 & TK3
		tk2_lfsr(tk2);
	}
}

static inline void forkskinny_128_init_tk23_internal(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                                       State128Sliced_t *tk3,
                                                                       KeySchedule128Sliced_t *out) {
	for (int i = 0; i < FORKSKINNY128_MAX_ROUNDS; ++i) {
		State128Sliced_t res;
		xor_keys(tk2, tk3, &res, 0);
		xor_keys(tk1, &res, &res, 0);
		//auto test_tks = unslice_accelerated_internal(res);
		
		// Keep in mind: the C2 constant relating to the 9nth cell is part of the 2nd 'half'!
		// So we add 0x2 at the key injection step
		forkskinny128_add_constant(&(res.halves[0]), i);
		out->keys[i] = res.halves[0];
		
		// 0x76541200, 0xa57e182c, 0xb46f097d
//		auto round_key = unslice_accelerated_internal({.halves = {schedule.keys[i], {}}}).values[0].raw[0];
		
		// Permute TK's
		permute(tk1);
		// TK 2: 9AC99F33632E5A76
//		auto tk2_unsliced_0 = unslice_accelerated_internal(tk2).values[0].raw[0]; // 0x9AC99F33632E5A76
//		auto tk2_unsliced_1 = unslice_accelerated_internal(tk2).values[0].raw[1]; // 0x1805831B70D3266
		permute(tk2);
//		auto tk2_unsliced_r_0_p = unslice_accelerated_internal(tk2).values[0].raw[0]; // 0xB01A6E026563CD01
//		auto tk2_unsliced_r_1_p = unslice_accelerated_internal(tk2).values[0].raw[1]; // 0x9AC99F33632E5A76
		permute(tk3);
		
		// LFSR TK2 & TK3
		tk2_lfsr(tk2);
//		auto utk2 = unslice_accelerated_internal(tk2).values[0].raw[0];  // 0x6E63011AB0CD0265
		tk3_lfsr(tk3);
//		auto utk3 = unslice_accelerated_internal(tk3).values[0].raw[0];  // 0xFC8D73AAF4BEC6E1
		
		// TK 2: 6E63011AB0CD0265
		// TK 3:
//		int appel = 1;
	}
}

static inline void forkskinny128_precompute_key_schedule(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                         KeySchedule128Sliced_t *out) {
	#if IMPROVED_KEYSCHEDULE
	forkskinny_128_init_tk2_fixsliced_internal(tk1, tk2, out);
	#else
	forkskinny_128_init_tk2_internal(tk1, tk2, out);
	#endif
}

static inline void forkskinny128_precompute_key_schedule(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                         State128Sliced_t *tk3, KeySchedule128Sliced_t *out) {
	#if IMPROVED_KEYSCHEDULE
	forkskinny_128_init_tk23_fixsliced_internal(tk1, tk2, tk3, out);
	#else
	forkskinny_128_init_tk23_internal(tk1, tk2, tk3, out);
	#endif
}

#endif //FORKSKINNYPLUS128_KEYSCHEDULE_H
