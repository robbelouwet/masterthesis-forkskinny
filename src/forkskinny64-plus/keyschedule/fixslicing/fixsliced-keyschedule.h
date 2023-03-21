#ifndef FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE_H
#define FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H


#include "../common.h"
#include "fixslicing-internal.h"
#include "fixslicing-permutations.h"


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
static inline KeyScheduleSliced_t fs_forkskinny_64_init_tk23(StateSliced_t tk1, StateSliced_t tk2,
                                                             StateSliced_t tk3) {
	KeyScheduleSliced_t schedule = KeyScheduleSliced_t();
	
	int i = 0;
	while (i < FORKSKINNY64_MAX_ROUNDS) {
		// RTK 0
		schedule.keys[i++] = xor_half_keys(xor_half_keys(tk1.halves[0], tk2.halves[0]), tk3.halves[0]);
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 1 & 2
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk12_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk12 = StateSliced_t();
		PT_2(rtk12_temp, rtk12)
		schedule.keys[i++] = rtk12.halves[1];
		schedule.keys[i++] = rtk12.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 3 & 4
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk34_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk34 = StateSliced_t();
		PT_4(rtk34_temp, rtk34)
		schedule.keys[i++] = rtk34.halves[1];
		schedule.keys[i++] = rtk34.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 5 & 6
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk56_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk56 = StateSliced_t();
		PT_6(rtk56_temp, rtk56)
		schedule.keys[i++] = rtk56.halves[1];
		schedule.keys[i++] = rtk56.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 7 & 8
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk78_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk78 = StateSliced_t();
		PT_8(rtk78_temp, rtk78)
		schedule.keys[i++] = rtk78.halves[1];
		schedule.keys[i++] = rtk78.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 9 & 10
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk910_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk910 = StateSliced_t();
		PT_10(rtk910_temp, rtk910)
		schedule.keys[i++] = rtk910.halves[1];
		schedule.keys[i++] = rtk910.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 11 & 12
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1112_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk1112 = StateSliced_t();
		PT_12(rtk1112_temp, rtk1112)
		schedule.keys[i++] = rtk1112.halves[1];
		schedule.keys[i++] = rtk1112.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 13 & 14
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1314_temp = xor_keys(xor_keys(tk1, tk2), tk3);
		auto rtk1314 = StateSliced_t();
		PT_14(rtk1314_temp, rtk1314)
		schedule.keys[i++] = rtk1314.halves[1];
		schedule.keys[i++] = rtk1314.halves[0];
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
		
		// RTK 15
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		schedule.keys[i++] = xor_half_keys(xor_half_keys(tk1.halves[1], tk2.halves[1]), tk3.halves[1]);
		fs_tk2_lfsr(&tk2);
		fs_tk3_lfsr(&tk3);
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
static inline KeyScheduleSliced_t fs_forkskinny_64_init_tk2(StateSliced_t tk1, StateSliced_t tk2) {
	KeyScheduleSliced_t schedule = KeyScheduleSliced_t();
	
	int i = 0;
	while (i < FORKSKINNY64_MAX_ROUNDS) {
		// RTK 0
		schedule.keys[i++] = xor_half_keys(tk1.halves[0], tk2.halves[0]);
		fs_tk2_lfsr(&tk2);
		
		// RTK 1 & 2
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk12_temp = xor_keys(tk1, tk2);
		auto rtk12 = StateSliced_t();
		PT_2(rtk12_temp, rtk12)
		schedule.keys[i++] = rtk12.halves[1];
		schedule.keys[i++] = rtk12.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 3 & 4
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk34_temp = xor_keys(tk1, tk2);
		auto rtk34 = StateSliced_t();
		PT_4(rtk34_temp, rtk34)
		schedule.keys[i++] = rtk34.halves[1];
		schedule.keys[i++] = rtk34.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 5 & 6
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk56_temp = xor_keys(tk1, tk2);
		auto rtk56 = StateSliced_t();
		PT_6(rtk56_temp, rtk56)
		schedule.keys[i++] = rtk56.halves[1];
		schedule.keys[i++] = rtk56.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 7 & 8
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk78_temp = xor_keys(tk1, tk2);
		auto rtk78 = StateSliced_t();
		PT_8(rtk78_temp, rtk78)
		schedule.keys[i++] = rtk78.halves[1];
		schedule.keys[i++] = rtk78.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 9 & 10
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk910_temp = xor_keys(tk1, tk2);
		auto rtk910 = StateSliced_t();
		PT_10(rtk910_temp, rtk910)
		schedule.keys[i++] = rtk910.halves[1];
		schedule.keys[i++] = rtk910.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 11 & 12
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1112_temp = xor_keys(tk1, tk2);
		auto rtk1112 = StateSliced_t();
		PT_12(rtk1112_temp, rtk1112)
		schedule.keys[i++] = rtk1112.halves[1];
		schedule.keys[i++] = rtk1112.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 13 & 14
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		auto rtk1314_temp = xor_keys(tk1, tk2);
		auto rtk1314 = StateSliced_t();
		PT_14(rtk1314_temp, rtk1314)
		schedule.keys[i++] = rtk1314.halves[1];
		schedule.keys[i++] = rtk1314.halves[0];
		fs_tk2_lfsr(&tk2);
		
		// RTK 15
		if (i >= FORKSKINNY64_MAX_ROUNDS) return schedule;
		schedule.keys[i++] = xor_half_keys(tk1.halves[1], tk2.halves[1]);
		fs_tk2_lfsr(&tk2);
	}
	
	return schedule;
}

#endif //FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE_H
