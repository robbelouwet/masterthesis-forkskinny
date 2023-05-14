#ifndef FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H
#define FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H

#include "internal64.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "../utils/slicing64.h"

/**
 * EXPECTS UNSEGMENTED TK STATES
 *
 * Pre-compute round keys of ForkSkinny64 using the fixed slicing technique as described in
 * section 2.3 of https://www.esat.kuleuven.be/cosic/events/silc2020/wp-content/uploads/sites/4/2020/10/Submission6.pdf.
 *
 * TK1 cannot be null.
 * @param tk1
 * @param tk2
 * @param tk3
 * @return
 */
static inline void forkskinny_64_init_tk23_fixsliced_internal_test(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                              State64Sliced_t *tk3, KeySchedule64Sliced_t *out) {
	auto i = 0;

	// RTK0
	auto res0 = xor_keys(xor_keys(*tk1, *tk2, 0), *tk3, 0);
	forkskinny64_add_constant(&(res0.halves[0]), i);
	out->keys[i++] = res0.halves[0];
	
	// RTK N & N+1
	for (; i < FORKSKINNY64_MAX_ROUNDS; i += 2) {
		tk2_lfsr_full(tk2);
		tk3_lfsr_full(tk3);
		auto rtk_temp = xor_keys(xor_keys(*tk1, *tk2), *tk3);
		
		State64Sliced_t temp;
		auto pt_index = (i + 1) & 0xF;
		if (pt_index != 0)
			for (int j = 0; j < 16; ++j)
				temp.cells[j] = rtk_temp.cells[fixslice_pt[pt_index][j]];
		else temp = rtk_temp;
		
		forkskinny64_add_constant(&(temp.halves[1]), i); // RTK N+1 comes before RTK N!
		out->keys[i] = temp.halves[1];
		forkskinny64_add_constant(&(temp.halves[0]), i+1);
		out->keys[i+1] = temp.halves[0];
	}
}


#endif //FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H
