#ifndef FORKSKINNYPLUS_PAEF_H
#define FORKSKINNYPLUS_PAEF_H

#include <cstring>
#include <stdexcept>
#include <iostream>
#include "../forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "../forkskinny64-plus/utils/slicing64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../test_vectors.h"

// TODO: THIS IS NOT CONSTANT TIME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  ----
//  ----
static inline void to_sliced(u64 in, State64Sliced_t *out) {
	for (int i = 0; i < 64; ++i) {
		u64 mask = 1 << i;
		if (mask & in) out->raw[i].value = slice_ONE;
		else out->raw[i].value = slice_ZER;
	}
}
//  ----
//  ----

static inline u64
extract_segment_tag(SlicedCiphertext64_t ct, bool last_segment, int last_block_index, char tag_leg) {
	/// Calculate the AD tag of this segment (these 64 or less blocks)
	/// by calculating the parity of every individual slice and packing those together
	u64 block_tag = 0;
	State64Sliced_t *pLeg;
	if (tag_leg == '0') pLeg = &(ct.C0);
	else pLeg = &(ct.C1);
	
	for (int j = 0; j < 64; j++) {
		u64 tag_bit = 0;
		
		# if slice_size > 64
		/* If the slice was a SIMD vector, iterate over its lanes */
		auto lanes_in_slice = slice_size >> 6;
		for (int k = 0; k < lanes_in_slice; ++k) {
			if (last_segment && last_block_index != -1 && last_block_index < 64)
				ct.C0.raw[j].value[k] &= -1ULL >> (64 - last_block_index); // PAS OP AANGEPAST
			u64 bit = __builtin_parity((u64) ct.C0.raw[j].value[k]);
			tag_bit ^= bit;
			last_block_index -= 64;
		}
		#else
		
		/* With non-SIMD slices, calculate the normal parity of the slice */
			tag_bit = __builtin_parity(ct.C0.raw[j].value);
		#endif
		
		block_tag ^= tag_bit << j;
	}
	
	return block_tag;
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
static inline SlicedCiphertext64_t paef_forkskinny64_192_encrypt_section(
		Blocks64_t ma, const Block64_t nonce_blocks[2], uint8_t nonce_bit_size, u64 *ctr, int last, char mode,
		bool isAD) {

	/// Construct sliced Tweakeys from nonce, bit flags and block ctr
	/* TK3 is the only one containing different counter values for each block */
	auto tk3_blocks = Blocks64_t();
	for (auto &value: tk3_blocks.values) value.raw = (*ctr)++;

	/* Set the nonce across TK1 & TK2 */
	State64Sliced_t sliced_tk1;
	to_sliced(nonce_blocks[0].raw, &sliced_tk1);
	State64Sliced_t sliced_tk2;
	to_sliced(nonce_blocks[1].raw, &sliced_tk2);

	/* Set the bit flags */
	State64Sliced_t sliced_tks[3] = {sliced_tk1, sliced_tk2, slice(tk3_blocks)};
	auto *recast = (Slice64_t *) &sliced_tks;
	(recast + nonce_bit_size)->value = slice_ZER;
	(recast + nonce_bit_size + 1)->value = slice_ZER;
	(recast + nonce_bit_size + 2)->value = isAD ? slice_ZER : slice_ONE;

	/* If this section is the last one, set bit flag of last block in this section */
	if (last != -1)
		(recast + nonce_bit_size)->value |= BIT(last);

	/// Encrypt
	State64Sliced_t state = slice(ma);
	auto schedule = forkskinny_64_fixsliced_init_tk23(sliced_tks[0], sliced_tks[1], sliced_tks[2]);
	return forkskinny64_encrypt(schedule, &state, mode);
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
static inline u64 paef_forkskinny64_192_encrypt_AD(
		Blocks64_t *pt, int last_block_index, u64 amount_segments, const Block64_t nonce_blocks[2],
		u64 nonce_bit_length) {
	
	u64 ctr = 1;
	u64 AD_tag = 0;
	for (int i = 0; i < amount_segments; ++i) {
		bool last_segment = i == amount_segments - 1;
		SlicedCiphertext64_t ct;
		
		/// Encrypt this segment, if this is the last segment, provide the index of the last block.
		/// E.g. this segment is the result of slicing 64 blocks, but only the first n < 64 were actual plaintext blocks
		
		ct = paef_forkskinny64_192_encrypt_section(
				pt[i],
				nonce_blocks,
				nonce_bit_length,
				&ctr,
				last_segment ? last_block_index : -1,
				'0',
				true);
		
		AD_tag ^= extract_segment_tag(ct, last_segment, last_block_index, '0');
	}
	
	return AD_tag;
}

static inline u64 paef_forkskinny64_192_encrypt_M(
		Blocks64_t *pt, int last_block_index, u64 amount_segments, const Block64_t nonce_blocks[2],
		u64 nonce_bit_length, Blocks64_t *ct_out) {
	
	u64 ctr = 1;
	u64 AD_tag = 0;
	for (int i = 0; i < amount_segments; ++i) {
		bool last_segment = i == amount_segments - 1;
		SlicedCiphertext64_t ct;
		
		/// Encrypt this segment, if this is the last segment, provide the index of the last block.
		/// E.g. this segment is the result of slicing 64 blocks, but only the first 23 were actual plaintext blocks
		ct = paef_forkskinny64_192_encrypt_section(
				pt[i], nonce_blocks, nonce_bit_length, &ctr,
				last_segment ? last_block_index : -1, 'b', false);
		
		// C1 contributes to the tag
		AD_tag ^= extract_segment_tag(ct, last_segment, last_block_index, '1');
		
		// C0 is ciphertext
		ct_out[i] = unslice_accelerated(ct.C0);
	}
	
	return AD_tag;
}

#endif //FORKSKINNYPLUS_PAEF_H
