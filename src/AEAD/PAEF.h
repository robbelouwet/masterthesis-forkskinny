#ifndef FORKSKINNYPLUS_PAEF_H
#define FORKSKINNYPLUS_PAEF_H

#include <cstring>
#include <stdexcept>
#include "../forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "../forkskinny64-plus/utils/slicing64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../test_vectors.h"

static inline void to_sliced(uint64_t in, State64Sliced_t *out) {
	for (int i = 0; i < 64; ++i) {
		uint64_t mask = 1 << i;
		if (mask & in) out->raw[i].value = ONE;
		else out->raw[i].value = ZER;
	}
}

/**
 * Encrypts a section of blocks. Tweakey is formed as follows: N || f⁰f¹f² || 0* || {ctr}⁶⁴.
 * In other words - from left to right - the first bits contain the nonce. Immediately thereafter are the 3 bit flags.
 * The last 64 bits (so TK3 basically) contains the counter value. Everything in between is 0.
 * @param a
 * @param nonce_blocks 2 64-bit integers of which every bit can be a nonce bit, only the 3 MSB's of [1] have to be unused.
 * @param nonce_bit_size bit size of the nonce. Has to be <= 125.
 * @param ctr is a uint64. According to the paper, it can be of bit length (192 - t - 4) which could be a lot more than 64,
 * 			  but no one's going to encrypt more than 2000 petabyte (2⁶⁴), right?
 * @return
 */
static inline SlicedCiphertext64_t paef_forkskinny64_192_encrypt_section(
		Blocks64_t a, const Block64_t nonce_blocks[2], uint8_t nonce_bit_size, uint64_t *ctr, int last, char mode) {
	
	/// Construct sliced Tweakeys from nonce, bit flag and block ctr
	/* TK3 is the only one containing different counter values for each block */
	auto tk3_blocks = Blocks64_t();
	for (auto &value: tk3_blocks.values) value.raw = (*ctr)++;
	
	/* Set the nonce across TK1 & 2 */
	State64Sliced_t sliced_tk1;
	to_sliced(nonce_blocks[0].raw, &sliced_tk1);
	State64Sliced_t sliced_tk2;
	to_sliced(nonce_blocks[1].raw, &sliced_tk2);
	
	/* Set the bit flags */
	State64Sliced_t sliced_tks[3] = {sliced_tk1, sliced_tk2, slice(tk3_blocks)};
	auto *recast = (Slice64_t *) &sliced_tks;
	(recast + nonce_bit_size)->value = ZER;
	(recast + nonce_bit_size + 1)->value = ZER;
	(recast + nonce_bit_size + 2)->value = ZER;
	
	/* If this section is the last one, set bit flag of last block in this section */
	///if (last != -1) (recast + nonce_bit_size + 3)->value = BIT_MASK(last);
	
	
	/// Encrypt
	State64Sliced_t state = slice(a);
	auto schedule = forkskinny_64_fixsliced_init_tk23(sliced_tks[0], sliced_tks[1], sliced_tks[2]);
	return forkskinny64_encrypt(schedule, &state, mode);
}

static inline uint64_t paef_forkskinny64_192_encrypt_AD(
		Blocks64_t *pt, int last, uint64_t amount_blocks, const Block64_t nonce_blocks[2], uint64_t nonce_bit_length) {
	
	uint64_t ctr;
	uint64_t AD_tag = 0;
	for (int i = 0; i < amount_blocks; ++i) {
		
		SlicedCiphertext64_t ct;
		if (i == amount_blocks - 1)
			ct = paef_forkskinny64_192_encrypt_section(
					pt[i], nonce_blocks, nonce_bit_length, &ctr, last, '0');
		
		else ct = paef_forkskinny64_192_encrypt_section(
					pt[i], nonce_blocks, nonce_bit_length, &ctr, -1, '0');
		
		uint64_t block_tag = 0;
		for (int j = 0; j < 64; j++) {
			uint64_t tag_bit = 0;
			
			# if slice_size > 64
			auto lanes_in_slice = slice_size >> 6;
			for (int k = 0; k < lanes_in_slice; ++k) {
				int banaan = __builtin_parity(0x888240AE684178D7);
				int appel = __builtin_parity((uint64_t) ct.C0.raw[i].value[k]);
				tag_bit ^= appel;
			}
			
			#else
			tag_bit = __builtin_parity(ct.C0.raw[i].value);
			#endif
			
			block_tag ^= tag_bit << j;
		}
		AD_tag ^= block_tag;
	}
	
	return AD_tag;
}

/**
 *
 * @param m
 * @param a
 * @param nonce_blocks byte array containing the nonce_blocks
 * @param nonce_bit_length (0-based index) length of bits that the nonce_blocks consists of.
 * 							HAS TO BE SMALLER THAN 189 (3 x 64 - 3-bit flag)
 * @return
 */
static inline SlicedCiphertext64_t paef_forkskinny64_192_encrypt(
//		Blocks64_t m, Blocks64_t a,
//                                                                 const Block64_t nonce_blocks[2],
//                                                                 uint8_t nonce_bit_length
) {
	
	// Let's take 61 blocks of AD plaintext
	auto test_blocks = M_rand_64();
	test_blocks.values[slice_size - 3].raw = 0;
	test_blocks.values[slice_size - 2].raw = 0;
	test_blocks.values[slice_size - 1].raw = 0;
	Block64_t nonce[2] = {{.raw = 0xFEDCBA9876543210},
	                      {.raw = 0xAAAAAAAA}};
	
	uint64_t xored_tag = paef_forkskinny64_192_encrypt_AD(
			&test_blocks, slice_size - 4, 1, nonce, 24 * 4);
}

#endif //FORKSKINNYPLUS_PAEF_H
