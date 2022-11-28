//#ifndef FORKSKINNY_C_FORKSKINNY64_CIPHER_H
//#define FORKSKINNY_C_FORKSKINNY64_CIPHER_H
//
//#include <stdint.h>
//#include <stddef.h>
//#include "x86intrin.h"
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//#define FORKSKINNY64_BLOCK_SIZE 8
//
//#define FORKSKINNY_64_192_ROUNDS_BEFORE 17
//
//#define FORKSKINNY_64_192_ROUNDS_AFTER 23
//
//#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_64_192_ROUNDS_BEFORE + 2*FORKSKINNY_64_192_ROUNDS_AFTER)
//
///**
// * Union that describes a 64-bit 4x4 array of cells16.
// */
//typedef union {
//	uint16_t row[4];        /// Stored as 4x 16-bit uint partitions (cccc cccc cccc cccc) [c = 4-bit state cell]
//	uint32_t lrow[2];        /// Stored as 2x 32-bit uint partitions  (cccccccc cccccccc)
//	uint64_t llrow;            /// Stored as 1x 64-bit uint 			  (cccccccccccccccc)
//} State64_t;
//
//
//
//
///**
// * Represents a normal State64, but vectorized for SIMD.
// * i.e.: 4-bit cells are expanded to smallest SIMD type, 16 bits
// */
//typedef union {
//	uint16_t cells16[16];   ///  000c¹⁵ | 000c¹⁴ | ... | 000c¹ | 000c⁰ (c = 4-bit cell)
//	uint32_t rows[4];       /// 000c¹⁵000c¹⁴ | 000c¹²000c¹¹ | ... | 000c¹000c⁰
//
//	// __m128i and __m256i are SIMD lane datatypes
//	__m128i hvec[2];
//	__m256i vec;
//
//} State64v_t;
//
///**
// * Union that describes a 32-bit 2x4 array of cells16.
// */
//typedef union {
//	uint8_t pairs[4];      /// Stored as 4x 8-bit  uint partitions (cc cc cc cc)
//	uint16_t row[2];       /// Stored as 2x 16-bit uint partitions (cccc cccc)
//	uint32_t lrow;            /// Stored as 1x 32-bit uint			    (cccccccc)
//	float flrow;            /// Stored as 1x 32-bit uint			    (cccccccc)
//
//} HalfState64_t;
//
///**
// * Key schedule for Forkskinny-64-192
// */
//typedef struct {
//	/** All words of the key schedule */
//	HalfState64_t schedule[FORKSKINNY64_MAX_ROUNDS];
//
//} KeySchedule64_t; /// because the 'key' part of
//
///**
// * Pre-computes the key schedule for Forkskinny-64-192 for TK1
// * ks:
// * key:       pointer to key bytes; reads FORKSKINNY64_BLOCK_SIZE bytes from the key
// * nb_rounds: the number of rounds of the key schedule
// */
//void forkskinny_c_64_192_init_tk1(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds);
///**
// * Pre-computes the key schedule for Forkskinny-64-192 for TK2 AND TK3
// * ks:
// * key:       pointer to key bytes; reads 2*FORKSKINNY64_BLOCK_SIZE bytes from the key
// * nb_rounds: the number of rounds of the key schedule
// */
//void forkskinny_c_64_192_init_tk2_tk3(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds);
//
///**
// * Computes the forward direction of Forkskinny-64-192.
// * tks1:           key schedule for TK1 (see forkskinny_c_64_192_init_tk1)
// * tks2:           key schedule for TK2 and TK3 (see forkskinny_c_64_192_init_tk2_tk3)
// * output_left:   if NULL, the left leg is not computed, else pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the left output leg of the forkcipher
// * output_right:  pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the right output leg of the forkcipher
// * input:         pointer to FORKSKINNY64_BLOCK_SIZE byte; input to the forkcipher
// */
//void forkskinny_c_64_192_encrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
//                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right,
//                                 unsigned long *cycles_spent);
//
///**
// * Computes the inverse direction of Forkskinny-64-192.
// * tks1:           key schedule for TK1 (see forkskinny_c_64_192_init_tk1)
// * tks2:           key schedule for TK2 and TK3 (see forkskinny_c_64_192_init_tk2_tk3)
// * output_left:   if NULL, the left leg is not computed, else pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the left output leg of the forkcipher (i.e. mode 'o')
// * output_right:  pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the inverted input of the forkcipher (i.e. mode 'i')
// * input_right:   pointer to FORKSKINNY64_BLOCK_SIZE byte; input to the inverse forkcipher
// */
//void forkskinny_c_64_192_decrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
//                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right);
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif // FORKSKINNY_C_FORKSKINNY64_CIPHER_H
