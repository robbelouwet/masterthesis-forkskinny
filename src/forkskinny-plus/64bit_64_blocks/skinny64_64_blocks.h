#ifndef FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H

#include "utils/skinny64_datatypes.h"
#include "roundfunction/skinny64-sbox.h"
#include "roundfunction/skinny64-addconstant.h"
#include "roundfunction/skinny64-shiftrows.h"
#include "roundfunction/skinny64-mixcols.h"


//static inline void skinny_inject_key(HalfState64Sliced_t round_key, State64Sliced_t *state) {
//	#if AVX512_acceleration
//	for (int i = 0; i < 4; ++i)
//		state->pairs[i].avx512_simd_pair = _mm512_xor_si512(round_key.pairs[i].avx512_simd_pair, state->pairs[i].avx512_simd_pair);
//
//	#elif AVX2_acceleration
//	for (int i = 0; i < 8; ++i)
//		state->cells[i].avx2_simd_cell = _mm256_xor_si256(round_key.cells[i].avx2_simd_cell, state->cells[i].avx2_simd_cell);
//
//	#else
//	for (int i = 0; i < 8; ++i){
//		state->cells[i].slices[0] = XOR_SLICE(state->cells[i].slices[0], round_key.cells[i].slices[0]);
//		state->cells[i].slices[1] = XOR_SLICE(state->cells[i].slices[1], round_key.cells[i].slices[1]);
//		state->cells[i].slices[2] = XOR_SLICE(state->cells[i].slices[2], round_key.cells[i].slices[2]);
//		state->cells[i].slices[3] = XOR_SLICE(state->cells[i].slices[3], round_key.cells[i].slices[3]);
//	}
//	#endif
//}

//static inline void skinny64_encrypt_single_round_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state,
//                                                           uint16_t iteration) {
//	skinny64_sbox(state);
//
//	skinny64_add_constant(state, iteration);
//
//	skinny_inject_key(schedule.keys[iteration], state);
//
//	skinny64_shiftrows(state);
//
//	skinny64_mixcols(state);
//}
//
//static void skinny64_encrypt_64_blocks(KeySchedule64Sliced_t schedule, State64Sliced_t *state) {
//	// ### INITIAL ROUNDS ###
//	int i = 0;
//	for (; i < 40; i++)
//		skinny64_encrypt_single_round_64_blocks(schedule, state, i);
//}

#endif //FORKSKINNYPLUS_FORKSKINNY64_64_BLOCKS_H
