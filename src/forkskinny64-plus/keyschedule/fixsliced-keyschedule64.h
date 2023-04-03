#ifndef FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H
#define FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H

#include "fixsliced-permutations.h"
#include "internal.h"
#include "../roundfunction/forkskinny64-addconstant.h"


/**
 * Pre-compute round keys of ForkSkinny64 using the fixed slicing technique as described in
 * section 2.3 of https://www.esat.kuleuven.be/cosic/events/silc2020/wp-content/uploads/sites/4/2020/10/Submission6.pdf.
 *
 * TK1 cannot be null.
 * @param tk1
 * @param tk2
 * @param tk3
 * @return
 */
static inline KeySchedule64Sliced_t forkskinny_64_fixsliced_init_tk23(State64Sliced_t tk1, State64Sliced_t tk2,
                                                               State64Sliced_t tk3) {
	KeySchedule64Sliced_t schedule = KeySchedule64Sliced_t();
	
	int i = 0;
	while (i < FORKSKINNY64_MAX_ROUNDS) {
		// RTK 0
		auto res0 = xor_half_keys(xor_half_keys(tk1.halves[0], tk2.halves[0]), tk3.halves[0]);
		//forkskinny64_add_constant(&res0, i);
		schedule.keys[i++] = res0;
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 1 & 2
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk12_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		//forkskinny64_add_constant(&(rtk12_temp.halves[0]), i+1); // RTK N+1 is at [0], and RTK N is at [1]!
		//forkskinny64_add_constant(&(rtk12_temp.halves[1]), i);
		auto rtk12 = State64Sliced_t();
		PT_2(rtk12_temp, rtk12)
		schedule.keys[i++] = rtk12.halves[1];
		schedule.keys[i++] = rtk12.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 3 & 4
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk34_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		forkskinny64_add_constant(&(rtk34_temp.halves[0]), i+1);
		forkskinny64_add_constant(&(rtk34_temp.halves[1]), i);
		auto rtk34 = State64Sliced_t();
		PT_4(rtk34_temp, rtk34)
		schedule.keys[i++] = rtk34.halves[1];
		schedule.keys[i++] = rtk34.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 5 & 6
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk56_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		forkskinny64_add_constant(&(rtk56_temp.halves[0]), i+1);
		forkskinny64_add_constant(&(rtk56_temp.halves[1]), i);
		auto rtk56 = State64Sliced_t();
		PT_6(rtk56_temp, rtk56)
		schedule.keys[i++] = rtk56.halves[1];
		schedule.keys[i++] = rtk56.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 7 & 8
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk78_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		forkskinny64_add_constant(&(rtk78_temp.halves[0]), i+1);
		forkskinny64_add_constant(&(rtk78_temp.halves[1]), i);
		auto rtk78 = State64Sliced_t();
		PT_8(rtk78_temp, rtk78)
		schedule.keys[i++] = rtk78.halves[1];
		schedule.keys[i++] = rtk78.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 9 & 10
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk910_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		forkskinny64_add_constant(&(rtk910_temp.halves[0]), i+1);
		forkskinny64_add_constant(&(rtk910_temp.halves[1]), i);
		auto rtk910 = State64Sliced_t();
		PT_10(rtk910_temp, rtk910)
		schedule.keys[i++] = rtk910.halves[1];
		schedule.keys[i++] = rtk910.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 11 & 12
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1112_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		forkskinny64_add_constant(&(rtk1112_temp.halves[0]), i+1);
		forkskinny64_add_constant(&(rtk1112_temp.halves[1]), i);
		auto rtk1112 = State64Sliced_t();
		PT_12(rtk1112_temp, rtk1112)
		schedule.keys[i++] = rtk1112.halves[1];
		schedule.keys[i++] = rtk1112.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 13 & 14
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1314_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		forkskinny64_add_constant(&(rtk1314_temp.halves[0]), i+1);
		forkskinny64_add_constant(&(rtk1314_temp.halves[1]), i);
		auto rtk1314 = State64Sliced_t();
		PT_14(rtk1314_temp, rtk1314)
		schedule.keys[i++] = rtk1314.halves[1];
		schedule.keys[i++] = rtk1314.halves[0];
		tk2_lfsr_full(&tk2);
		tk3_lfsr_full(&tk3);
		
		// RTK 15
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk15 = xor_half_keys(xor_half_keys(tk1.halves[1], tk2.halves[1]), tk3.halves[1]);
		forkskinny64_add_constant(&rtk15, i);
		schedule.keys[i++]= rtk15;
	}
	
	return schedule;
}

/**
 * Pre-compute round keys of ForkSkinny64 using the fixed slicing technique as described in
 * section 2.3 of https://www.esat.kuleuven.be/cosic/events/silc2020/wp-content/uploads/sites/4/2020/10/Submission6.pdf.
 *
 * TK1 cannot be null.
 * @param tk1
 * @param tk2
 * @param tk3
 * @return
 */
static inline KeySchedule64Sliced_t fs_forkskinny_64_init_tk2(State64Sliced_t tk1, State64Sliced_t tk2) {
	KeySchedule64Sliced_t schedule = KeySchedule64Sliced_t();
	
	int i = 0;
	while (i < FORKSKINNY64_MAX_ROUNDS) {
		// RTK 0
		schedule.keys[i++] = xor_half_keys(tk1.halves[0], tk2.halves[0]);
		tk2_lfsr_full(&tk2);
		
		// RTK 1 & 2
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk12_temp = xor_keys(tk1, tk2);
		auto rtk12 = State64Sliced_t();
		PT_2(rtk12_temp, rtk12)
		schedule.keys[i++] = rtk12.halves[1];
		schedule.keys[i++] = rtk12.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 3 & 4
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk34_temp = xor_keys(tk1, tk2);
		auto rtk34 = State64Sliced_t();
		PT_4(rtk34_temp, rtk34)
		schedule.keys[i++] = rtk34.halves[1];
		schedule.keys[i++] = rtk34.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 5 & 6
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk56_temp = xor_keys(tk1, tk2);
		auto rtk56 = State64Sliced_t();
		PT_6(rtk56_temp, rtk56)
		schedule.keys[i++] = rtk56.halves[1];
		schedule.keys[i++] = rtk56.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 7 & 8
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk78_temp = xor_keys(tk1, tk2);
		auto rtk78 = State64Sliced_t();
		PT_8(rtk78_temp, rtk78)
		schedule.keys[i++] = rtk78.halves[1];
		schedule.keys[i++] = rtk78.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 9 & 10
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk910_temp = xor_keys(tk1, tk2);
		auto rtk910 = State64Sliced_t();
		PT_10(rtk910_temp, rtk910)
		schedule.keys[i++] = rtk910.halves[1];
		schedule.keys[i++] = rtk910.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 11 & 12
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1112_temp = xor_keys(tk1, tk2);
		auto rtk1112 = State64Sliced_t();
		PT_12(rtk1112_temp, rtk1112)
		schedule.keys[i++] = rtk1112.halves[1];
		schedule.keys[i++] = rtk1112.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 13 & 14
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1314_temp = xor_keys(tk1, tk2);
		auto rtk1314 = State64Sliced_t();
		PT_14(rtk1314_temp, rtk1314)
		schedule.keys[i++] = rtk1314.halves[1];
		schedule.keys[i++] = rtk1314.halves[0];
		tk2_lfsr_full(&tk2);
		
		// RTK 15
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		schedule.keys[i++] = xor_half_keys(tk1.halves[1], tk2.halves[1]);
		tk2_lfsr_full(&tk2);
	}
	
	return schedule;
}

#endif //FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H
