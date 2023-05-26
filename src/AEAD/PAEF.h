#ifndef FORKSKINNYPLUS_PAEF_H
#define FORKSKINNYPLUS_PAEF_H

#include <cstring>
#include <stdexcept>
#include <iostream>
#include "../forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "../forkskinny64-plus/utils/slicing64-internal.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/keyschedule64.h"
#include "../forkskinny64-plus/utils/slicing64.h"

static inline void to_sliced(u64 in, State64Sliced_t *out) {
	for (int i = 0; i < 64; ++i) {
		// trick to make it constant time:
		auto res = (((1 << i) & in) >> i);
		#if slice_size == 512
		out->raw[i].value = _mm512_set1_epi64((ULL) (((int) 0) - res));
		#elif slice_size == 256
		out->raw[i].value = _mm256_set1_epi64x((ULL) (((int) 0) - res));
		#elif slice_size == 128
		out->raw[i].value = _mm_set1_epi64x((ULL) (((int) 0) - res));
		#else
		auto val = (((int) 0) - res);
		out->raw[i].value = val;
		#endif
	}
}

static inline void extract_segment_tag(State64Sliced_t *state, bool last_segment, int last_block_index,
                                       u64 *result_tag) {
	/// Calculate the AD tag of this segment (these 64 or less blocks)
	/// by calculating the parity of every individual slice_internal and packing those together
	
	for (int j = 0; j < 64; j++) {
		u64 tag_bit;
		# if slice_size > 64
		/* If the slice was a SIMD vector, iterate over its lanes */
		auto lanes_in_slice = slice_size >> 6;
		for (int k = 0; k < lanes_in_slice; ++k) {
			if (last_segment && last_block_index != -1 && last_block_index < 64)
				state->raw[j].value[k] &= -1ULL >> (64 - last_block_index);
			u64 bit = __builtin_parity((u64) state->raw[j].value[k]);
			tag_bit ^= bit;
			last_block_index -= 64;
		}
		#else
		/* With non-SIMD slices, calculate the normal parity of the slice_internal */
		tag_bit = __builtin_parity(state->raw[j].value);
		#endif
		
		*result_tag ^= tag_bit << j;
	}
}

/**
 * Encrypts a section of blocks. Tweakey is formed as follows: N || f⁰f¹f² || 0* || {ctr}⁶⁴.
 * In other words - from left to right - the first bits contain the nonce. Immediately thereafter are the 3 bit flags.
 * The last 64 bits (so TK3 basically) contains the counter value. Everything in between is 0.
 * @param ma input segments. Either AD or M
 * @param nonce_blocks 2 64-bit integers of which every bit can be a nonce bit, only the 3 MSB's of [1] have to be unused.
 * @param nonce_bit_size bit size of the nonce. Has to be <= 125.
 * @param ctr the counter, a uint64. According to the paper, it can be of bit length (192 - t - 4) which could be a lot more than 64,
 * 			  but no one's going to encrypt more than 2000 petabyte (2⁶⁴), right?
 * @return
 */
static inline void paef_forkskinny64_192_encrypt_section(
		Blocks64_t *ma, Block64_t *nonce_blocks, State64Sliced_t *TK1, State64Sliced_t *TK2,
		uint8_t nonce_bit_size, u64 *ctr, int last, char mode,
		bool isAD, SlicedCiphertext64_t *res, double *t_slice, double *t_encrypt) {
	
	/// Construct sliced Tweakeys from nonce, bit flags and block ctr
	/* TK3 is the only one containing different counter values for each block */
	auto tk3_blocks = Blocks64_t();
	for (auto &value: tk3_blocks.values) value.raw = (*ctr)++;
	
	State64Sliced_t sliced_tk3;
	State64Sliced_t state;
	auto t_slice_before = _rdtsc();
	/* Set the nonce across TK1 & TK2 */
	slice(&tk3_blocks, &sliced_tk3);
	slice(ma, &state);
	*t_slice = _rdtsc() - t_slice_before;
	
	/// Encrypt
	auto schedule = KeySchedule64Sliced_t();
	auto t_encrypt_before = _rdtsc();
	forkskinny64_192_precompute_key_schedule(sliced_tks + 0, sliced_tks + 1, sliced_tks + 2, &schedule);
	forkskinny64_encrypt(&schedule, &state, mode, &(res->C0), &(res->C1));
	*t_encrypt = _rdtsc() - t_encrypt_before;
}

/**
 * Encrypts a list of segments. A segment is a sliced group of blocks. If last_block_index is specified, then only the
 * first *last_block_index + 1* blocks in the last segment will be used to xor and calculate the tag.
 *
 * @param pt pointer to a list containing *amount_segments* segments (sliced cipher states)
 * @param last_block_index index of the last block inside the last segment. In other words, every segment holds the same
 * amount of blocks, also the last segment will hold (and also encrypt) more blocks than necessary. This param forces
 * the method to not inlcude the last n blocks to calculate the tag with.
 * @param amount_segments amount of sliced cipher states that *pt* holds (because we can't calculate length of a list that a pointer points to)
 * @param nonce_blocks The nonce N, segmented into 2 64-bit blocks.
 * @param nonce_bit_length since the nonce N is held in a buffer of 2 blocks stretching 128 bits, we manually specify
 * the bit length of the nonce itself, within its 128-bit buffer of TK1 & TK2
 * @return the AD tag resulting from encrypting all the segments in *pt* and XOR'ing all the C0 outputs together (except
 * for the last n blocks in the last segment, specified by *last_block_index*).
 */
static inline void paef_forkskinny64_192_encrypt_AD(
		Blocks64_t *pt, u64 amount_segments, Block64_t *nonce_blocks, u64 nonce_bit_length,
		SlicedCiphertext64_t *result_ct, u64 *result_tag, double *t_slice, double *t_encrypt) {
	
	// Nonce stays the same for evey block, so we can slice and copy beforehand
	State64Sliced_t TK1;
	State64Sliced_t TK2;
	to_sliced(nonce_blocks[0].raw, &TK1);
	to_sliced(nonce_blocks[1].raw, &TK2);
	
	if (nonce_bit_length <= 64) {
		TK1.raw[nonce_bit_length - 64 - 1].value =;
	} else {
		// ----------------- refactoring to pre-compute TK1 and TK2
		
	}
	
	u64 ctr = 1;
	bool last = slice_size - 1; // since we just encrypt 1 full sliced state, provide default last index
	for (int i = 0; i < amount_segments; ++i) {
		paef_forkskinny64_192_encrypt_section(
				pt + i, // -------- is geen arr in callstack
				nonce_blocks,
				&TK1, &TK2,
				nonce_bit_length,
				&ctr,
				last,
				'0',
				true,
				result_ct + i,
				t_slice,
				t_encrypt);
		
		// C0 contributes to the tag
		u64 segment_tag;
		extract_segment_tag(&(result_ct[i].C0), true, slice_size - 1, &segment_tag);
		*result_tag ^= segment_tag;
	}
}

static inline void paef_forkskinny64_192_encrypt_M(
		Blocks64_t *pt, u64 amount_segments, Block64_t *nonce_blocks, u64 nonce_bit_length,
		Blocks64_t *result_ct, u64 *result_tag, double *t_slice, double *t_encrypt) {
	
	// Nonce stays the same for evey block, so we can slice and copy beforehand
	State64Sliced_t TK1;
	State64Sliced_t TK2;
	to_sliced(nonce_blocks[0].raw, &TK1);
	to_sliced(nonce_blocks[1].raw, &TK2);
	
	SlicedCiphertext64_t ct;
	Blocks64_t unsliced;
	
	u64 ctr = 1;
	bool last = slice_size - 1; // since we just encrypt 1 full sliced state, provide default last index
	for (int i = 0; i < amount_segments; ++i) {
		
		paef_forkskinny64_192_encrypt_section(
				pt + i,
				nonce_blocks, &TK1, &TK2,
				nonce_bit_length,
				&ctr,
				last,
				'b',
				false,
				&ct,
				t_slice,
				t_encrypt);
		
		// C1 contributes to the tag
		u64 m_tag;
		// State64Sliced_t *state, bool last_segment, int last_block_index, u64 *result_tag
		extract_segment_tag(&(ct.C1), last, last, &m_tag);
		*result_tag ^= m_tag;
		
		auto before = _rdtsc();
		unslice(&(ct.C1), &unsliced);
		*t_slice += _rdtsc() - before;
		*result_ct = unsliced;
	}
}

#endif //FORKSKINNYPLUS_PAEF_H
