#ifndef FIXSLICED_KEYSCHEDULE128
#define FIXSLICED_KEYSCHEDULE128


#include "../utils/forkskinny128-datatypes.h"
#include "internal128.h"
#include "../../constants.h"
#include "../roundfunction/forkskinny128-addconstant.h"

static inline void fixslice_permute(State128Sliced_t *src, State128Sliced_t *dst, int i) {
//	auto test_blocks = Blocks128_t{.values = {{.raw = {0x7766554433221100, 0xFFEEDDCCBBAA9988}}}};
//	*src = slice128(&test_blocks);
	#if AVX2_acceleration
	for (int j = 0; j < 16; ++j) {
		auto cell_src = fixsliced_pt128[i][j];
		
		dst->raw[(j >> 2) * 32 + (j & 3) + 0] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 0];
		dst->raw[(j >> 2) * 32 + (j & 3) + 4] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 4];
		dst->raw[(j >> 2) * 32 + (j & 3) + 8] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 8];
		dst->raw[(j >> 2) * 32 + (j & 3) + 12] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 12];
		dst->raw[(j >> 2) * 32 + (j & 3) + 16] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 16];
		dst->raw[(j >> 2) * 32 + (j & 3) + 20] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 20];
		dst->raw[(j >> 2) * 32 + (j & 3) + 24] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 24];
		dst->raw[(j >> 2) * 32 + (j & 3) + 28] = src->raw[(cell_src >> 2) * 32 + (cell_src & 3) + 28];
	}
	#else
	for (int j = 0; j < 16; ++j) {
		dst->cells[j] = src->cells[fixsliced_pt128[i][j]];
	}
	#endif
	
//	auto test_output = unslice128(dst).values[0];
//	int appel = 1;
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
static inline void forkskinny_128_init_tk23_fixsliced_internal(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                               State128Sliced_t *tk3, KeySchedule128Sliced_t *out) {
	auto i = 0;
	
	// RTK0
	State128Sliced_t res0;
	xor_keys(tk2, tk3, &res0, 0);
	xor_keys(tk1, &res0, &res0, 0);
	
	forkskinny128_add_constant(&(res0.halves[0]), i);
	out->keys[i++] = res0.halves[0];
	
	// RTK N & N+1
	for (; i < FORKSKINNY128_MAX_ROUNDS; i += 2) {
		tk2_lfsr(tk2, true);
		tk3_lfsr(tk3, true);
		
		State128Sliced_t rtk_temp;
		xor_keys(tk2, tk3, &rtk_temp, -1);
		xor_keys(tk1, &rtk_temp, &rtk_temp, -1);
		
		State128Sliced_t temp; // 0x BB00 9900 2200 EE6C | 0x 9900 7700 EE00 448C
		auto pt_index = (i + 1) & 0xF;
		if (pt_index != 0) fixslice_permute(&rtk_temp, &temp, pt_index);
		else temp = rtk_temp;
		
		forkskinny128_add_constant(&(temp.halves[1]), i); // RTK N+1 comes before RTK N!
		out->keys[i] = temp.halves[1];
		forkskinny128_add_constant(&(temp.halves[0]), i + 1);
		out->keys[i + 1] = temp.halves[0];
	}
}

static inline void forkskinny_128_init_tk2_fixsliced_internal(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                              KeySchedule128Sliced_t *out) {
	auto i = 0;
	
	// RTK0
	State128Sliced_t res0;
	xor_keys(tk1, tk2, &res0, 0);
	
	forkskinny128_add_constant(&(res0.halves[0]), i);
	out->keys[i++] = res0.halves[0];
	
	// RTK N & N+1
	for (; i < FORKSKINNY128_MAX_ROUNDS; i += 2) {
		tk2_lfsr(tk2, true);
		
		State128Sliced_t rtk_temp;
		xor_keys(tk1, tk2, &rtk_temp, -1);
		
		State128Sliced_t temp; // 0x BB00 9900 2200 EE6C | 0x 9900 7700 EE00 448C
		auto pt_index = (i + 1) & 0xF;
		if (pt_index != 0) fixslice_permute(&rtk_temp, &temp, pt_index);
		else temp = rtk_temp;
		
		forkskinny128_add_constant(&(temp.halves[1]), i); // RTK N+1 comes before RTK N!
		out->keys[i] = temp.halves[1];
		forkskinny128_add_constant(&(temp.halves[0]), i + 1);
		out->keys[i + 1] = temp.halves[0];
	}
}

#endif //FIXSLICED_KEYSCHEDULE128
