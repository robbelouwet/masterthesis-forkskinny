#ifndef FORKSKINNYPLUS_PAEF_H
#define FORKSKINNYPLUS_PAEF_H

#include <cstring>
#include <stdexcept>
#include "../forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "../forkskinny64-plus/utils/slicing64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"

static inline void to_sliced(uint64_t in, State64Sliced_t *out) {
	for (int i = 0; i < 64; ++i) {
		uint64_t mask = 1 << i;
		if (mask & in) out->raw[i].value = ONE;
		else out->raw[i].value = ZER;
	}
}

/**
 * Encrypts a section of associated-data blocks. Tweakey is formed as follows: N || f⁰f¹f² || 0* || {ctr}⁶⁴.
 * In other words - from left to right - the first bits contain the nonce. Immediately thereafter are the 3 bit flags.
 * The last 64 bits (so TK3 basically) contains the counter value. Everything in between is 0.
 * @param a
 * @param nonce_blocks 2 64-bit integers of which every bit can be a nonce bit, only the 3 MSB's of [1] have to be unused.
 * @param nonce_bit_length bit length of the nonce. Has to be <= 125.
 * @param ctr is a uint64. According to the paper, it can be of bit length (192 - t - 4) which could be a lot more than 64,
 * 			  but no one's going to encrypt more than 2000 petabyte (2⁶⁴), right?
 * @return
 */
static inline SlicedCiphertext64_t paef_forkskinny64_192_encrypt_AD_section(Blocks64_t a,
                                                                            const Block64_t nonce_blocks[2],
                                                                            uint8_t nonce_bit_length, uint64_t *ctr,
                                                                            int last) {
	/// Construct sliced Tweakeys from nonce, bit flag and block ctr
	/* TK3 is the only one containing different counter values for each block */
	auto tk3_blocks = Blocks64_t();
	for (auto &value: tk3_blocks.values)
		value.raw = (*ctr)++;
	
	/* Set the nonce across TK1 & 2 */
	State64Sliced_t sliced_tk1;
	to_sliced(nonce_blocks[0].raw, &sliced_tk1);
	State64Sliced_t sliced_tk2;
	to_sliced(nonce_blocks[1].raw, &sliced_tk2);
	
	/* Set the bit flags */
	State64Sliced_t sliced_tks[3] = {sliced_tk1, sliced_tk2, slice(tk3_blocks)};
	auto *recast = (Slice64_t *) &sliced_tks;
	(recast + nonce_bit_length + 1)->value = ZER;
	(recast + nonce_bit_length + 2)->value = ZER;
	(recast + nonce_bit_length + 3)->value = ZER;
	
	/* If this section is the last one, set bit flag of last block in this section */
	if (last != -1) {
		(recast + nonce_bit_length + 3)->value = ZER;
	}
	
	/// Encrypt
	State64Sliced_t state = slice(a);
	auto schedule = forkskinny_64_fixsliced_init_tk23(sliced_tks[0], sliced_tks[1], sliced_tks[2]);
	return forkskinny64_encrypt(schedule, &state, '0');
}

static inline SlicedCiphertext64_t *paef_forkskinny64_192_encrypt_AD() {

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
static inline SlicedCiphertext64_t paef_forkskinny64_192_encrypt(Blocks64_t m, Blocks64_t a,
                                                                 const Block64_t nonce_blocks[3],
                                                                 uint8_t nonce_bit_length) {
	
	
	
	
	// Associated Data
	auto A = slice(a);
	uint64_t ai = 0;
	uint64_t ad_blocks = sizeof(a);
	while (ai < upper_bound && ai < ad_blocks - 1) {
		
		
		ai++;
	}
	
}

#endif //FORKSKINNYPLUS_PAEF_H
