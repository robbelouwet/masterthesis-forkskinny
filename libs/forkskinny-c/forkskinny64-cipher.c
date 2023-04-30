/*
 * Copyright (C) 2017 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR_AVX2 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR_AVX2 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR_AVX2 OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR_AVX2 OTHERWISE, ARISING
 * FROM, OUT OF OR_AVX2 IN CONNECTION WITH THE SOFTWARE OR_AVX2 THE USE OR_AVX2 OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "headers/forkskinny64-cipher.h"
#include "headers/forkskinny-internal.h"
#include <x86intrin.h>

STATIC_INLINE uint32_t skinny64_LFSR2(uint32_t x) {
	return ((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U);
}

STATIC_INLINE uint32_t skinny64_LFSR3(uint32_t x) {
	return ((x >> 1) & 0x77777777U) ^ ((x ^ (x << 3)) & 0x88888888U);
}

STATIC_INLINE void skinny64_permute_tk(State64_t *tk) {
	/* PT = [9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7] */
	#if SKINNY_LITTLE_ENDIAN
	/* Permutation generated by http://programming.sirrida.de/calcperm.php */
	uint32_t x = tk->lrow[1];
	tk->lrow[1] = tk->lrow[0];
	tk->lrow[0] = ((x & 0x0000000FU) << 4) |
	              ((x & 0x00F0F0F0U) << 8) |
	              ((x & 0x0F000000U) >> 24) |
	              ((x & 0x00000F00U) << 16) |
	              ((x & 0xF0000000U) >> 12) |
	              ((x & 0x000F0000U) >> 8);
	#else
	uint16_t row2 = tk->row[2];
	uint16_t row3 = tk->row[3];
	tk->row[2] = tk->row[0];
	tk->row[3] = tk->row[1];
	row3 = (row3 << 8) | (row3 >> 8);
	tk->row[0] = ((row2 << 4) & 0x00F0U) |
				 ((row2 << 8) & 0xF000U) |
				  (row3       & 0x0F0FU);
	tk->row[1] = ((row2 >> 8) & 0x00F0U) |
				  (row2       & 0x0F00U) |
				 ((row3 >> 4) & 0x000FU) |
				 ( row3       & 0xF000U);
	#endif
}

/* Initializes the key keys with TK1 */
void forkskinny_c_64_192_init_tk1(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds) {
	State64_t tk;
	unsigned index;
	
	/* Unpack the key and convert from little-endian to host-endian */
	#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
	tk.llrow = READ_WORD64(key, 0);
	#elif SKINNY_LITTLE_ENDIAN
	tk.lrow[0] = READ_WORD32(key, 0);
	tk.lrow[1] = READ_WORD32(key, 4);
	#else
	tk.row[0] = READ_WORD16(key, 0);
	tk.row[1] = READ_WORD16(key, 2);
	tk.row[2] = READ_WORD16(key, 4);
	tk.row[3] = READ_WORD16(key, 6);
	#endif
	
	/* Generate the key keys words for all rounds */
	for (index = 0; index < nb_rounds; ++index) {
		/* Determine the subkey to use at this point in the key keys */
		ks->schedule[index].lrow = tk.lrow[0];
		
		/* Permute TK1 for the next round */
		skinny64_permute_tk(&tk);
	}
}

void forkskinny_c_64_192_init_tk2_tk3(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds) {
	State64_t tk2, tk3;
	unsigned index;
	
	/* Unpack the key and convert from little-endian to host-endian */
	#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
	tk2.llrow = READ_WORD64(key, 0);
	tk3.llrow = READ_WORD64(key + FORKSKINNY64_BLOCK_SIZE, 0);
	#elif SKINNY_LITTLE_ENDIAN
	tk2.lrow[0] = READ_WORD32(key, 0);
	tk2.lrow[1] = READ_WORD32(key, 4);
	tk3.lrow[0] = READ_WORD32(key + FORKSKINNY64_BLOCK_SIZE, 0);
	tk3.lrow[1] = READ_WORD32(key + FORKSKINNY64_BLOCK_SIZE, 4);
	#else
	tk2.row[0] = READ_WORD16(key, 0);
	tk2.row[1] = READ_WORD16(key, 2);
	tk2.row[2] = READ_WORD16(key, 4);
	tk2.row[3] = READ_WORD16(key, 6);
	tk3.row[0] = READ_WORD16(key + FORKSKINNY64_BLOCK_SIZE, 0);
	tk3.row[1] = READ_WORD16(key + FORKSKINNY64_BLOCK_SIZE, 2);
	tk3.row[2] = READ_WORD16(key + FORKSKINNY64_BLOCK_SIZE, 4);
	tk3.row[3] = READ_WORD16(key + FORKSKINNY64_BLOCK_SIZE, 6);
	#endif
	
	/* Generate the key keys words for all rounds */
	for (index = 0; index < nb_rounds; ++index) {
		/* Determine the subkey to use at this point in the key keys */
		ks->schedule[index].lrow = tk2.lrow[0] ^ tk3.lrow[0];
		
		/* XOR_AVX2 in the round constants for the first two rows.
		   The round constants for the 3rd and 4th rows are
		   fixed and will be applied during encrypt/decrypt */
		ks->schedule[index].row[0] ^= ((RC[index] & 0x0F) << 4) ^ 0x2000;
		ks->schedule[index].row[1] ^= (RC[index] & 0x70);
		
		/* Permute TK2/TK3 for the next round */
		skinny64_permute_tk(&tk2);
		skinny64_permute_tk(&tk3);
		
		/* Apply LFSR2 to the first two rows of TK2 */
		tk2.lrow[0] = skinny64_LFSR2(tk2.lrow[0]);
		/* Apply LFSR3 to the first two rows of TK3 */
		tk3.lrow[0] = skinny64_LFSR3(tk3.lrow[0]);
	}
}

STATIC_INLINE uint16_t skinny64_rotate_right(uint16_t x, unsigned count) {
	return (x >> count) | (x << (16 - count));
}

#if SKINNY_64BIT

STATIC_INLINE u64 skinny64_sbox(u64 x) {
	/* Splitting the bits out individually gives better performance on
	   64-bit platforms because we have more spare registers to work with.
	   This doesn't work as well on 32-bit platforms because register
	   spills start to impact performance.  See below. */
	u64 bit0 = ~x;
	u64 bit1 = bit0 >> 1;
	u64 bit2 = bit0 >> 2;
	u64 bit3 = bit0 >> 3;
	bit0 ^= bit3 & bit2;
	bit3 ^= bit1 & bit2;
	bit2 ^= bit1 & bit0;
	bit1 ^= bit0 & bit3;
	x = ((bit0 << 3) & 0x8888888888888888ULL) |
	    (bit1 & 0x1111111111111111ULL) |
	    ((bit2 << 1) & 0x2222222222222222ULL) |
	    ((bit3 << 2) & 0x4444444444444444ULL);
	return ~x;
}

STATIC_INLINE u64 skinny64_inv_sbox(u64 x) {
	u64 bit0 = ~x;
	u64 bit1 = bit0 >> 1;
	u64 bit2 = bit0 >> 2;
	u64 bit3 = bit0 >> 3;
	bit0 ^= bit3 & bit2;
	bit1 ^= bit3 & bit0;
	bit2 ^= bit1 & bit0;
	bit3 ^= bit1 & bit2;
	x = ((bit0 << 1) & 0x2222222222222222ULL) |
	    ((bit1 << 2) & 0x4444444444444444ULL) |
	    ((bit2 << 3) & 0x8888888888888888ULL) |
	    (bit3 & 0x1111111111111111ULL);
	return ~x;
}

#else

STATIC_INLINE uint32_t skinny64_sbox(uint32_t x)
{
	/* Original version from the specification is equivalent to:
	 *
	 * #define SBOX_MIX(x)
	 *     (((~((((x) >> 1) | (x)) >> 2)) & 0x11111111U) ^ (x))
	 * #define SBOX_SHIFT(x)
	 *     ((((x) << 1) & 0xEEEEEEEEU) | (((x) >> 3) & 0x11111111U))
	 *
	 * x = SBOX_MIX(x);
	 * x = SBOX_SHIFT(x);
	 * x = SBOX_MIX(x);
	 * x = SBOX_SHIFT(x);
	 * x = SBOX_MIX(x);
	 * x = SBOX_SHIFT(x);
	 * return SBOX_MIX(x);
	 *
	 * However, we can mix the bits in their original positions and then
	 * delay the SBOX_SHIFT steps to be performed with one final rotation.
	 * This reduces the number of required shift operations from 14 to 10.
	 *
	 * It is possible to reduce the number of shifts and AND's even further
	 * as shown in the 64-bit version of old_sbox() above.  However on
	 * 32-bit platforms this causes extra register spills which slows down
	 * the implementation more than the improvement gained by reducing the
	 * number of bit operations.
	 *
	 * We can further reduce the number of NOT operations from 4 to 2
	 * using the technique from https://github.com/kste/skinny_avx to
	 * convert NOR-XOR_AVX2 operations into AND-XOR_AVX2 operations by converting
	 * the S-box into its NOT-inverse.
	 */
	x = ~x;
	x = (((x >> 3) & (x >> 2)) & 0x11111111U) ^ x;
	x = (((x << 1) & (x << 2)) & 0x88888888U) ^ x;
	x = (((x << 1) & (x << 2)) & 0x44444444U) ^ x;
	x = (((x >> 2) & (x << 1)) & 0x22222222U) ^ x;
	x = ~x;
	return ((x >> 1) & 0x77777777U) | ((x << 3) & 0x88888888U);
}

STATIC_INLINE uint32_t skinny64_inv_sbox(uint32_t x)
{
	/* Original version from the specification is equivalent to:
	 *
	 * #define SBOX_MIX(x)
	 *     (((~((((x) >> 1) | (x)) >> 2)) & 0x11111111U) ^ (x))
	 * #define SBOX_SHIFT_INV(x)
	 *     ((((x) >> 1) & 0x77777777U) | (((x) << 3) & 0x88888888U))
	 *
	 * x = SBOX_MIX(x);
	 * x = SBOX_SHIFT_INV(x);
	 * x = SBOX_MIX(x);
	 * x = SBOX_SHIFT_INV(x);
	 * x = SBOX_MIX(x);
	 * x = SBOX_SHIFT_INV(x);
	 * return SBOX_MIX(x);
	 */
	x = ~x;
	x = (((x >> 3) & (x >> 2)) & 0x11111111U) ^ x;
	x = (((x << 1) & (x >> 2)) & 0x22222222U) ^ x;
	x = (((x << 1) & (x << 2)) & 0x44444444U) ^ x;
	x = (((x << 1) & (x << 2)) & 0x88888888U) ^ x;
	x = ~x;
	return ((x << 1) & 0xEEEEEEEEU) | ((x >> 3) & 0x11111111U);
}

#endif

static State64_t forkskinny64_encrypt_rounds(
		State64_t state, const KeySchedule64_t *tks1, const KeySchedule64_t *tks23, unsigned from,
		unsigned to) {
	
	const HalfState64_t *schedule1, *schedule2;
	unsigned index;
	uint32_t temp;
	
	/* Perform all encryption rounds */
	schedule1 = tks1->schedule + from;
	schedule2 = tks23->schedule + from;
	for (index = from; index < to; ++index, ++schedule1, ++schedule2) {
		
		/// SubCells
		/* Apply the S-box to all bytes in the state */
		#if SKINNY_64BIT
		// 0x EFCD AB89 6745 2301 | 0x 757B B40C 6D26 819C
		state.llrow = skinny64_sbox(state.llrow);
		#else
		state.lrow[0] = skinny64_sbox(state.lrow[0]);
		state.lrow[1] = old_sbox(state.lrow[1]);
		#endif
		
		/// AddConstants
		/* Apply the subkey for this round */
		#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
		// 0x 7F4E 5D38 2B1A 90C6 | 0x BABD D1C4 2E92 3684
		u64 key = (schedule1->lrow ^ schedule2->lrow) | 0x2000000000ULL;
		state.llrow ^= key;
		
		#else
		state.lrow[0] ^= schedule1->lrow ^ schedule2->lrow;
		state.row[2] ^= 0x20;
		#endif
		
		/* Shift the rows */
		// 0x 7F4E 5D18 C51A 6D26 | 0x BABD D1E4 2709 16BF
		state.row[1] = skinny64_rotate_right(state.row[1], 4);
		state.row[2] = skinny64_rotate_right(state.row[2], 8);
		state.row[3] = skinny64_rotate_right(state.row[3], 12);
		
		/* Mix the columns */
		// 0x F4E7 185D AC51 6D26 | 0x ABDB E4D1 9270 16BF
		state.row[1] ^= state.row[2];
		state.row[2] ^= state.row[0];
		temp = state.row[3] ^ state.row[2];
		state.row[3] = state.row[2];
		state.row[2] = state.row[1];
		state.row[1] = state.row[0];
		state.row[0] = temp;
		
		// 0x 757B B40C 6D26 819C | 0x F26E 76A1 16BF 59B5
		int appel1 = 1;
	}
	return state;
}

void forkskinny_c_64_192_encrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right,
                                 unsigned long *cycles_spent) {
	unsigned long before = _rdtsc();
	State64_t state;
	
	/* Read the input buffer and convert little-endian to host-endian */
	#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
	state.llrow = READ_WORD64(input_right, 0);
	#elif SKINNY_LITTLE_ENDIAN
	state.lrow[0] = READ_WORD32(input_right, 0);
	state.lrow[1] = READ_WORD32(input_right, 4);
	#else
	state.row[0] = READ_WORD16(input_right, 0);
	state.row[1] = READ_WORD16(input_right, 2);
	state.row[2] = READ_WORD16(input_right, 4);
	state.row[3] = READ_WORD16(input_right, 6);
	#endif
	
	/* Run all of the rounds before the forking point */
	state = forkskinny64_encrypt_rounds(state, tks1, tks2, 0, FORKSKINNY_ROUNDS_BEFORE);
	
	/* Determine which output blocks we need */
	if (output_left && output_right) {
		
		/* Generate the C0 output block */
		State64_t fstate = forkskinny64_encrypt_rounds(
				state, tks1, tks2,
				FORKSKINNY_ROUNDS_BEFORE,
				FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER);
		
		/* Convert host-endian back into little-endian in the output buffer */
		#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
		WRITE_WORD64(output_right, 0, fstate.llrow);
		#elif SKINNY_LITTLE_ENDIAN
		WRITE_WORD32(output_right, 0, fstate.lrow[0]);
		WRITE_WORD32(output_right, 4, fstate.lrow[1]);
		#else
		WRITE_WORD16(output_right, 0, fstate.row[0]);
		WRITE_WORD16(output_right, 2, fstate.row[1]);
		WRITE_WORD16(output_right, 4, fstate.row[2]);
		WRITE_WORD16(output_right, 6, fstate.row[3]);
		#endif
	}
	if (output_left) {
		/* Generate the C1 output block */
		#if SKINNY_64BIT
		state.llrow ^= 0x81ec7f5bda364912U;
		#else
		state.row[0] ^= 0x4912U;  /* Branching constant */
		state.row[1] ^= 0xda36U;
		state.row[2] ^= 0x7f5bU;
		state.row[3] ^= 0x81ecU;
		#endif
		
		state = forkskinny64_encrypt_rounds(state, tks1, tks2,
		                                    FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER,
		                                    FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER * 2);
		
		/* Convert host-endian back into little-endian in the output buffer */
		#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
		WRITE_WORD64(output_left, 0, state.llrow);
		
		#elif SKINNY_LITTLE_ENDIAN
		WRITE_WORD32(output_left, 0, state.lrow[0]);
		WRITE_WORD32(output_left, 4, state.lrow[1]);
		#else
		WRITE_WORD16(output_left, 0, state.row[0]);
		WRITE_WORD16(output_left, 2, state.row[1]);
		WRITE_WORD16(output_left, 4, state.row[2]);
		WRITE_WORD16(output_left, 6, state.row[3]);
		#endif
	} else {
		/* We only need the C0 output block */
		state = forkskinny64_encrypt_rounds
				(state, tks1, tks2, FORKSKINNY_ROUNDS_BEFORE,
				 FORKSKINNY_ROUNDS_BEFORE +
				 FORKSKINNY_ROUNDS_AFTER);
		
		/* Convert host-endian back into little-endian in the output buffer */
		#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
		WRITE_WORD64(output_right, 0, state.llrow);
		#elif SKINNY_LITTLE_ENDIAN
		WRITE_WORD32(output_right, 0, state.lrow[0]);
		WRITE_WORD32(output_right, 4, state.lrow[1]);
		#else
		WRITE_WORD16(output_right, 0, state.row[0]);
		WRITE_WORD16(output_right, 2, state.row[1]);
		WRITE_WORD16(output_right, 4, state.row[2]);
		WRITE_WORD16(output_right, 6, state.row[3]);
		#endif
	}
	
	unsigned long after = _rdtsc();
	*cycles_spent = after - before;
}

static State64_t forkskinny64_decrypt_rounds(
		State64_t state, const KeySchedule64_t *ks1, const KeySchedule64_t *ks2, unsigned from,
		unsigned to) {
	
	const HalfState64_t *schedule1, *schedule2;
	unsigned index;
	uint32_t temp;
	
	/* Perform all decryption rounds */
	schedule1 = &(ks1->schedule[from - 1]);
	schedule2 = &(ks2->schedule[from - 1]);
	for (index = from; index > to; --index, --schedule1, --schedule2) {
		/* Inverse mix of the columns */
		temp = state.row[3];
		state.row[3] = state.row[0];
		state.row[0] = state.row[1];
		state.row[1] = state.row[2];
		state.row[3] ^= temp;
		state.row[2] = temp ^ state.row[0];
		state.row[1] ^= state.row[2];
		
		/* Inverse shift of the rows */
		state.row[1] = skinny64_rotate_right(state.row[1], 12);
		state.row[2] = skinny64_rotate_right(state.row[2], 8);
		state.row[3] = skinny64_rotate_right(state.row[3], 4);
		
		/* Apply the subkey for this round */
		#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
		state.llrow ^= (schedule1->lrow ^ schedule2->lrow) | 0x2000000000ULL;
		#else
		state.lrow[0] ^= schedule1->lrow ^ schedule2->lrow;
		state.row[2] ^= 0x20;
		#endif
		
		/* Apply the inverse of the S-box to all bytes in the state */
		#if SKINNY_64BIT
		state.llrow = skinny64_inv_sbox(state.llrow);
		#else
		state.lrow[0] = skinny64_inv_sbox(state.lrow[0]);
		state.lrow[1] = skinny64_inv_sbox(state.lrow[1]);
		#endif
	}
	return state;
}

void forkskinny_c_64_192_decrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right) {
	
	State64_t state;
	
	/* Read the input buffer and convert little-endian to host-endian */
	#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
	state.llrow = READ_WORD64(input_right, 0);
	#elif SKINNY_LITTLE_ENDIAN
	state.lrow[0] = READ_WORD32(input_right, 0);
	state.lrow[1] = READ_WORD32(input_right, 4);
	#else
	state.row[0] = READ_WORD16(input_right, 0);
	state.row[1] = READ_WORD16(input_right, 2);
	state.row[2] = READ_WORD16(input_right, 4);
	state.row[3] = READ_WORD16(input_right, 6);
	#endif
	
	/* Perform the "after" rounds on the input to get back
	 * to the forking point in the cipher */
	state = forkskinny64_decrypt_rounds
			(state, tks1, tks2, FORKSKINNY_ROUNDS_BEFORE +
			                    FORKSKINNY_ROUNDS_AFTER,
			 FORKSKINNY_ROUNDS_BEFORE);
	
	if (output_left) {
		State64_t fstate = state;
		/* Add the branching constant */
		#if SKINNY_64BIT
		fstate.llrow ^= 0x81ec7f5bda364912U;
		#else
		fstate.row[0] ^= 0x4912U;
		fstate.row[1] ^= 0xda36U;
		fstate.row[2] ^= 0x7f5bU;
		fstate.row[3] ^= 0x81ecU;
		#endif
		
		/* Generate the C1 output block after another "after" rounds */
		fstate = forkskinny64_encrypt_rounds(fstate, tks1, tks2,
		                                     FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER,
		                                     FORKSKINNY_ROUNDS_BEFORE + 2 * FORKSKINNY_ROUNDS_AFTER);
		
		/* Convert host-endian back into little-endian in the output buffer */
		#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
		WRITE_WORD64(output_left, 0, fstate.llrow);
		#elif SKINNY_LITTLE_ENDIAN
		WRITE_WORD32(output_left, 0, fstate.lrow[0]);
		WRITE_WORD32(output_left, 4, fstate.lrow[1]);
		#else
		WRITE_WORD16(output_left, 0, fstate.row[0]);
		WRITE_WORD16(output_left, 2, fstate.row[1]);
		WRITE_WORD16(output_left, 4, fstate.row[2]);
		WRITE_WORD16(output_left, 6, fstate.row[3]);
		#endif
	}
	
	
	/* Generate the C0 output block by going backward "before"
	 * rounds from the forking point */
	state = forkskinny64_decrypt_rounds
			(state, tks1, tks2, FORKSKINNY_ROUNDS_BEFORE, 0);
	
	/* Convert host-endian back into little-endian in the output buffer */
	#if SKINNY_64BIT && SKINNY_LITTLE_ENDIAN
	WRITE_WORD64(output_right, 0, state.llrow);
	#elif SKINNY_LITTLE_ENDIAN
	WRITE_WORD32(output_right, 0, state.lrow[0]);
	WRITE_WORD32(output_right, 4, state.lrow[1]);
	#else
	WRITE_WORD16(output_right, 0, state.row[0]);
	WRITE_WORD16(output_right, 2, state.row[1]);
	WRITE_WORD16(output_right, 4, state.row[2]);
	WRITE_WORD16(output_right, 6, state.row[3]);
	#endif
}
