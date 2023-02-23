#ifndef FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H
#define FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H

#include "keyschedule64-internal.h"


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
static inline KeySchedule64Sliced_t precompute_64_key_schedules(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                                State64Sliced_t *tk3) {
	KeySchedule64Sliced_t schedule = KeySchedule64Sliced_t();
	
	//<editor-fold desc="recursive PT tables"
	uint8_t pt[7][16] =
			{
					{1, 7, 0, 5, 2, 6, 4, 3, 9,  15, 8,  13, 10, 14, 12, 11}, // PT²
					{7, 3, 1, 6, 0, 4, 2, 5, 15, 11, 9,  14, 8,  12, 10, 13}, // PT⁴
					{3, 5, 7, 4, 1, 2, 0, 6, 11, 13, 15, 12, 9,  10, 8,  14}, // PT⁶
					{5, 6, 3, 2, 7, 0, 1, 4, 13, 14, 11, 10, 15, 8,  9,  12}, // PT⁸
					{6, 4, 5, 0, 3, 1, 7, 2, 14, 12, 13, 8,  11, 9,  15, 10}, // PT¹⁰
					{4, 2, 6, 1, 5, 7, 3, 0, 12, 10, 14, 9,  13, 15, 11, 8}, // PT¹²
					{2, 0, 4, 7, 6, 3, 5, 1, 10, 8,  12, 15, 14, 11, 13, 9} // PT¹⁴
			};
	//</editor-fold>
	
	// <editor-fold desc="double RTK i & i+1"
	// every iteration = 2 round keys, so bound = ceil(max_rounds/2)
	auto bound = (FORKSKINNY64_MAX_ROUNDS >> 1) + (FORKSKINNY64_MAX_ROUNDS & 1);
	for (int i = 0; i < bound; i++) {
		// RTK15
		if (i % 8 == 7) {
			auto res = xor_key(*tk3, *tk2, 8, 16);
			res = xor_key(res, *tk1, 8, 16);
			schedule.keys[++i] = res.halves[1]; // RTK0
		}
		
		// RTK0
		if (i % 8 == 0) {
			auto res = xor_key(*tk3, *tk2);
			res = xor_key(res, *tk1);
			schedule.keys[i] = res.halves[0]; // RTK0
			
			tk3_lfsr_simd(tk3); // Update TK3
			tk2_lfsr_simd(tk2); // Update TK2
			continue;
		}
		
		// Add TK states
		auto pt_values = xor_key(*tk3, *tk2, 16);
		pt_values = xor_key(pt_values, *tk1, 16);
		
		// PT² / PT⁴ / PT⁶ / ... / PT¹⁴ / PT² / ...
		auto pt_index = (i * 2) % 16;
		auto double_rtk = permute(pt_values, pt[pt_index]);
		schedule.keys[i] = double_rtk.halves[1];
		schedule.keys[i + 1] = double_rtk.halves[0];
		
		// Update TK2 & TK3
		tk3_lfsr_simd(tk3); // Update TK3
		tk2_lfsr_simd(tk2); // Update TK2
	}
	//</editor-fold>
	
	return schedule;
}

#endif //FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H
