#ifndef FORKSKINNYPLUS_SKINNY64_DATATYPES_H
#define FORKSKINNYPLUS_FORKSKINNY64_PLUS_H

#define FORKSKINNY64_BLOCK_SIZE 8

#define FORKSKINNY_ROUNDS_BEFORE 17

#define FORKSKINNY_ROUNDS_AFTER 23

#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_ROUNDS_BEFORE + 2*FORKSKINNY_ROUNDS_AFTER)

#include <cstdint>


typedef union {
	uint8_t pairs[4];
	uint16_t row[2];
	uint32_t lrow;
	
} HalfState64_t;

typedef union {
	uint64_t cell[4];
	__m256i simd_cell;
} Cell64_t;

typedef union {
	Cell64_t cells[16];
	Cell64_t half_cells[8][2];
	uint64_t raw[64];
} State64Sliced_t;

typedef union {
	uint64_t cells[8][16];
	uint64_t raw[128];
	__m512 simd_cells[16];
} State128Sliced_t;

/**
 * Key keys for Forkskinny-64-192
 */
typedef struct {
	/** All words of the key keys */
	State64Sliced_t keys[FORKSKINNY64_MAX_ROUNDS];
	
} KeySchedule64Sliced_t;

///*static inline State64Sliced_8_t lsfr_64_tk2_4bit(State64Sliced_8_t state) {
//	auto x0 = state.slices[0];
//
//	state.slices[0] = state.slices[3];
//	state.slices[3] = state.slices[2];
//	state.slices[2] = state.slices[1];
//	state.slices[1] = x0;
//	state.slices[0] ^= state.slices[3];
//
//	return state;
//}
//
//static inline State64Sliced_8_t lsfr_64_tk2_4bit_inverse(State64Sliced_8_t state) {
//	auto prev_x3 = state.slices[0] ^ state.slices[3];
//
//	state.slices[0] = state.slices[1];
//	state.slices[1] = state.slices[2];
//	state.slices[2] = state.slices[3];
//	state.slices[3] = prev_x3;
//
//	return state;
//}*/
//
///*static inline State64Sliced_16_t lsfr_64_tk2_4bit(State64Sliced_16_t state) {
//	auto x0 = state.slices[0];
//
//	state.slices[0] = state.slices[3];
//	state.slices[3] = state.slices[2];
//	state.slices[2] = state.slices[1];
//	state.slices[1] = x0;
//	state.slices[0] ^= state.slices[3];
//
//	return state;
//}*/
//
///*static inline State64Sliced_16_t lsfr_64_tk2_4bit_inverse(State64Sliced_16_t state) {
//	auto prev_x3 = state.slices[0] ^ state.slices[3];
//
//	state.slices[0] = state.slices[1];
//	state.slices[1] = state.slices[2];
//	state.slices[2] = state.slices[3];
//	state.slices[3] = prev_x3;
//
//	return state;
//}*/
//
///**
// * Pre-computes the key keys for Forkskinny-64-192 for TK1
// * ks:
// * key:       pointer to key bytes; reads FORKSKINNY64_BLOCK_SIZE bytes from the key
// * nb_rounds: the number of rounds of the key keys
// */
//void forkskinny_c_64_192_init_tk1(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds);
//
///**
// * Pre-computes the key keys for Forkskinny-64-192 for TK2 AND TK3
// * ks:
// * key:       pointer to key bytes; reads 2*FORKSKINNY64_BLOCK_SIZE bytes from the key
// * nb_rounds: the number of rounds of the key keys
// */
//void forkskinny_c_64_192_init_tk2_tk3(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds);
//
///**
// * Computes the forward direction of Forkskinny-64-192.
// * tks1:           key keys for TK1 (see forkskinny_c_64_192_init_tk1)
// * tks2:           key keys for TK2 and TK3 (see forkskinny_c_64_192_init_tk2_tk3)
// * output_left:   if NULL, the C1 leg is not computed, else pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the C1 output leg of the forkcipher
// * output_right:  pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the C0 output leg of the forkcipher
// * input:         pointer to FORKSKINNY64_BLOCK_SIZE byte; input to the forkcipher
// */
//void forkskinny_c_64_192_encrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
//                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right,
//                                 unsigned long *cycles_spent);
//
///**
// * Computes the inverse direction of Forkskinny-64-192.
// * tks1:           key keys for TK1 (see forkskinny_c_64_192_init_tk1)
// * tks2:           key keys for TK2 and TK3 (see forkskinny_c_64_192_init_tk2_tk3)
// * output_left:   if NULL, the C1 leg is not computed, else pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the C1 output leg of the forkcipher (i.e. mode 'o')
// * output_right:  pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the inverted input of the forkcipher (i.e. mode 'i')
// * input_right:   pointer to FORKSKINNY64_BLOCK_SIZE byte; input to the inverse forkcipher
// */
//void forkskinny_c_64_192_decrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
//                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right);
//
#endif //FORKSKINNYPLUS_SKINNY64_DATATYPES_H
