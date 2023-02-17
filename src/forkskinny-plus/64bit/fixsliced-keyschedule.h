#ifndef FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE_H
#define FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE_H

#include "forkskinny64-plus.h"

#define FSKINNY_INIT_ROUNDS 21
#define FSKINNY_BRANCH_ROUNDS 27

// (defined below)
static inline void tk2_lfsr_simd(State64Sliced_t *state);

static inline void tk3_lfsr_simd(State64Sliced_t *state);

static inline void xor_key(State64Sliced_t *a, State64Sliced_t *b);



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
static inline State64Sliced_t precompute_64_key_schedules(State64Sliced_t *tk1, State64Sliced_t *tk2 = nullptr,
                                                          State64Sliced_t *tk3 = nullptr) {
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
	
	KeySchedule64Sliced_t schedule = KeySchedule64Sliced_t();
	
	// ----- STEP 1: RTK0 -----
	// Update TK3
	if (tk3 != nullptr) tk3_lfsr_simd(tk3);
	
	// Update tk2
	// don't xor if one of them is null
	if (tk2 != nullptr && tk3 != nullptr) tk2_lfsr_simd(tk2);
	
	// Update tk1
	if (tk2 != nullptr && tk3 != nullptr){
		xor_key(tk2, tk1);
		xor_key(tk3, tk1);
	}
	
	// extract RTK0
	schedule.keys[0] = tk1->halves[0];
	
	
	// RK1+
	for (int i = 0; i < FSKINNY_INIT_ROUNDS + 2 * FSKINNY_BRANCH_ROUNDS; ++i) {
	
	}
}

static inline void tk2_lfsr_simd(State64Sliced_t *state) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (auto &cell: state->cells) {
		// 0b10010011 = 2 1 0 3 -> lanes for the simd permutation
		cell.simd_cell = _mm256_permute4x64_epi64(cell.simd_cell, 0b10010011);
		cell.slices[0] ^= cell.slices[3];
	}
}

static inline void tk3_lfsr_simd(State64Sliced_t *state) {
	// loop all 16 cells instead of top 2 rows only
	// -> see paper
	for (auto &cell: state->cells) {
		// 0b10010011 = 0 3 2 1 -> lanes for the simd permutation
		cell.simd_cell = _mm256_permute4x64_epi64(cell.simd_cell, 0b00111001);
		cell.slices[3] ^= cell.slices[0];
	}
}

/**
 * XOR's the first n cells of 2 keys together, and stores them in b.
 * @param a
 * @param b destination value
 * @param n amount of cells to xor, default = 8
 */
static inline void xor_key(State64Sliced_t *a, State64Sliced_t *b, uint8_t n = 8) {
	for (int i = 0; i < n; ++i)
		a->cells[i].simd_cell = _mm256_xor_si256(a->cells[i].simd_cell, b->cells[i].simd_cell);
}

#endif //FORKSKINNYPLUS_FIXSLICED_KEYSCHEDULE_H
