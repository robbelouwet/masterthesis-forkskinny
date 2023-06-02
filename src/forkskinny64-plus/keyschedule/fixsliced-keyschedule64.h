#ifndef FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H
#define FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H

#include "internal64.h"
#include "../roundfunction/forkskinny64-addconstant.h"
#include "../utils/slicing64-internal.h"

static inline void fixslice_permute64(State64Sliced_t *src, State64Sliced_t *dst, int i) {
	#if SEGMENTATION
	for (int j = 0; j < 16; ++j) {
		auto cell_src = fixsliced_pt64[i][j];
		
		dst->raw[(j >> 2) * 16 + (j & 3) + 0] = src->raw[(cell_src >> 2) * 16 + (cell_src & 3) + 0];
		dst->raw[(j >> 2) * 16 + (j & 3) + 4] = src->raw[(cell_src >> 2) * 16 + (cell_src & 3) + 4];
		dst->raw[(j >> 2) * 16 + (j & 3) + 8] = src->raw[(cell_src >> 2) * 16 + (cell_src & 3) + 8];
		dst->raw[(j >> 2) * 16 + (j & 3) + 12] = src->raw[(cell_src >> 2) * 16 + (cell_src & 3) + 12];
	}
	#else
	for (int j = 0; j < 16; ++j) {
		dst->cells[j] = src->cells[fixsliced_pt64[i][j]];
	}
	#endif
}

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
static inline void forkskinny_64_init_tk23_fixsliced_internal(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                              State64Sliced_t *tk3, KeySchedule64Sliced_t *out) {
	auto i = 0;
	
	// RTK0
	State64Sliced_t res0;
	xor_keys64(tk2, tk3, &res0, 0);
	xor_keys64(tk1, &res0, &res0, 0);
	
//	forkskinny64_add_constant(&(res0.halves[0]), i);
	out->keys[i++] = res0.halves[0];
	
	// RTK N & N+1
	for (; i < FORKSKINNY64_MAX_ROUNDS; i += 2) {
		tk2_lfsr64(tk2, true);
		tk3_lfsr64(tk3, true);
		
		State64Sliced_t rtk_temp;
		xor_keys64(tk2, tk3, &rtk_temp, -1);
		xor_keys64(tk1, &rtk_temp, &rtk_temp, -1);
		
		State64Sliced_t temp; // 0x BB00 9900 2200 EE6C | 0x 9900 7700 EE00 448C
		auto pt_index = (i + 1) & 0xF;
		if (pt_index != 0) fixslice_permute64(&rtk_temp, &temp, pt_index);
		else temp = rtk_temp;
		
//		forkskinny64_add_constant(&(temp.halves[1]), i); // RTK N+1 comes before RTK N!
		out->keys[i] = temp.halves[1];
//		forkskinny64_add_constant(&(temp.halves[0]), i + 1);
		out->keys[i + 1] = temp.halves[0];
	}
}

static inline void forkskinny_64_init_tk2_fixsliced_internal(State64Sliced_t *tk1, State64Sliced_t *tk2,
                                                             KeySchedule64Sliced_t *out) {
	auto i = 0;
	
	// RTK0
	State64Sliced_t res0;
	xor_keys64(tk1, tk2, &res0, 0);
	
//	forkskinny64_add_constant(&(res0.halves[0]), i);
	out->keys[i++] = res0.halves[0];
	
	// RTK N & N+1
	for (; i < FORKSKINNY64_MAX_ROUNDS; i += 2) {
		tk2_lfsr64(tk2, true);
		
		State64Sliced_t rtk_temp;
		xor_keys64(tk1, tk2, &rtk_temp, -1);
		
		State64Sliced_t temp; // 0x BB00 9900 2200 EE6C | 0x 9900 7700 EE00 448C
		auto pt_index = (i + 1) & 0xF;
		if (pt_index != 0) fixslice_permute64(&rtk_temp, &temp, pt_index);
		else temp = rtk_temp;
		
//		forkskinny64_add_constant(&(temp.halves[1]), i); // RTK N+1 comes before RTK N!
		out->keys[i] = temp.halves[1];
//		forkskinny64_add_constant(&(temp.halves[0]), i + 1);
		out->keys[i + 1] = temp.halves[0];
	}
}


#endif //FORKSKINNYPLUS64_FIXSLICED_KEYSCHEDULE_H
