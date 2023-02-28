#ifndef FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H
#define FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H

#include "./forkskinny128-plus.h"

// (defined below)
static inline void tk2_lfsr_simd(State128Sliced_t *state);

static inline void tk3_lfsr_simd(State128Sliced_t *state);

static inline State128Sliced_t xor_key(State128Sliced_t a, State128Sliced_t b, uint8_t n = 8);

static inline State128Sliced_t permute(State128Sliced_t values, const uint8_t table[16]);


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
static inline KeySchedule128Sliced_t precompute_64_key_schedules(State128Sliced_t *tk1, State128Sliced_t *tk2,
                                                                 State128Sliced_t *tk3) {
	KeySchedule128Sliced_t schedule = KeySchedule128Sliced_t();
	
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
	
	// <editor-fold desc="RTK 0">
	auto res = xor_key(*tk3, *tk2); // TK1 ^= TK2 ^ TK3, dereference because we don't want to update the TK's themselves
	res = xor_key(res, *tk1);
	schedule.keys[0] = res.halves[0]; // RTK0
	
	tk3_lfsr_simd(tk3); // Update TK3
	tk2_lfsr_simd(tk2); // Update TK2
	// </editor-fold>
	
	// <editor-fold desc="double RTK: RTK_i & RTK_i+1"
	// every iteration = 2 round keys, so bound = ceil(max_rounds/2)
	auto bound = (FORKSKINNY128_MAX_ROUNDS >> 1) + (FORKSKINNY128_MAX_ROUNDS & 1);
	for (int i = 1; i < bound; i++) {
		// Add TK states
		auto pt_values = xor_key(*tk3, *tk2, 16);
		pt_values = xor_key(pt_values, *tk1, 16);
		
		// Round permutation
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

static inline void tk2_lfsr_simd(State128Sliced_t *state) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (auto &cell: state->cells) {
		
		// a SIMD instr for permuting 8x 64b values doesn't exist
		auto temp = cell.slices[7] ^ cell.slices[5];
		cell.slices[7] = cell.slices[6];
		cell.slices[6] = cell.slices[5];
		cell.slices[5] = cell.slices[4];
		cell.slices[4] = cell.slices[3];
		cell.slices[3] = cell.slices[2];
		cell.slices[2] = cell.slices[1];
		cell.slices[1] = cell.slices[0];
		cell.slices[0] = temp;
	}
}

static inline void tk3_lfsr_simd(State128Sliced_t *state) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (auto &cell: state->cells) {
		
		// a SIMD instr for permuting 8x 64b values doesn't exist
		auto temp = cell.slices[0] ^ cell.slices[6];
		cell.slices[0] = cell.slices[1];
		cell.slices[1] = cell.slices[2];
		cell.slices[2] = cell.slices[3];
		cell.slices[3] = cell.slices[4];
		cell.slices[4] = cell.slices[5];
		cell.slices[5] = cell.slices[6];
		cell.slices[6] = cell.slices[7];
		cell.slices[7] = temp;
	}
}

/**
 * XOR's the first n cells of 2 keys together, and stores them in res
 * @param a
 * @param b
 * @param n amount of cells to xor, default = 8
 */
static inline State128Sliced_t xor_key(State128Sliced_t a, State128Sliced_t b, uint8_t n) {
	auto res = State128Sliced_t();
	for (int i = 0; i < n; ++i) {
		// TODO: #IFDEFINED AVX512: provide faster alternative: _mm512_xor_epi64 instead of 2x 256b lanes
		
		// for every sliced cell consisting of 8x 64bit_64_blocks integers, view it as 2x 256bit simd lanes and xor both
		res.cells[i].simd_cells[0] = _mm256_xor_si256(a.cells[i].simd_cells[0], b.cells[i].simd_cells[0]);
		res.cells[i].simd_cells[1] = _mm256_xor_si256(a.cells[i].simd_cells[1], b.cells[i].simd_cells[1]);
	}
	return res;
}

static inline State128Sliced_t permute(State128Sliced_t values, const uint8_t table[16]) {
	auto res = State128Sliced_t();
	for (int i = 0; i < 16; ++i)
		res.cells[i] = values.cells[table[i]];
	return res;
}

#endif //FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE64_H
