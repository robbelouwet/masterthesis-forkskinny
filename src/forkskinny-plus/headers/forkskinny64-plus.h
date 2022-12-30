#ifndef FORKSKINNYPLUS_FORKSKINNY64_PLUS_H
#define FORKSKINNYPLUS_FORKSKINNY64_PLUS_H

#define FORKSKINNY64_BLOCK_SIZE 8

#define FORKSKINNY_64_192_ROUNDS_BEFORE 17

#define FORKSKINNY_64_192_ROUNDS_AFTER 23

#define FORKSKINNY64_MAX_ROUNDS (FORKSKINNY_64_192_ROUNDS_BEFORE + 2*FORKSKINNY_64_192_ROUNDS_AFTER)

#include <mmintrin.h>
#include <immintrin.h>
#include <cstdint>

/**
 * Union that describes a 64-bit 4x4 array of cells16.
 */
typedef union {
	uint16_t row[4];        /// Stored as 4x 16-bit uint partitions (cccc cccc cccc cccc) [c = 4-bit state cell]
	uint32_t lrow[2];        /// Stored as 2x 32-bit uint partitions  (cccccccc cccccccc)
	uint64_t llrow;            /// Stored as 1x 64-bit uint 			  (cccccccccccccccc)
} State64_t;

/**
 * Union that describes a 32-bit 2x4 array of cells16.
 */
typedef union {
	uint8_t pairs[4];      /// Stored as 4x 8-bit  uint partitions (cc cc cc cc)
	uint16_t row[2];       /// Stored as 2x 16-bit uint partitions (cccc cccc)
	uint32_t lrow;            /// Stored as 1x 32-bit uint			    (cccccccc)
	
} HalfState64_t;

/**
 * Represents a 64-bit bit-sliced_fghi state
 *
 * a bit sliced_fghi representation of 2 4-bit cells would be: [x³, y³], [x², y²], [x¹, y¹], [x⁰, y⁰].
 */
typedef union {
	uint16_t slices[4];  // [p³o³...b³a³, ... , p⁰o⁰...b⁰a⁰], where there are 16 4-bit cells named a through p
	uint64_t state;
	__m64 m64state;  // SIMD 64-bit register
} State64Sliced_16_t;

/**
 * Represents half a 64-bit bit-sliced_fghi state
 *
 * a bit sliced_fghi representation of 2 4-bit cells would be: [x³, y³], [x², y²], [x¹, y¹], [x⁰, y⁰].
 */
typedef union {
	uint8_t slices[4];  // [p³o³...b³a³, ... , p⁰o⁰...b⁰a⁰], where there are 8 4-bit cells named a through p
	uint32_t state;
} State64Sliced_8_t;

/**
 * Key schedule for Forkskinny-64-192
 */
typedef struct {
	/** All words of the key schedule */
	State64Sliced_16_t schedule[FORKSKINNY64_MAX_ROUNDS];
	
} KeySchedule64_t; /// because the 'key' part of

static inline State64Sliced_8_t lsfr_64_tk2_4bit(State64Sliced_8_t state) {
	auto x0 = state.slices[0];
	
	state.slices[0] = state.slices[3];
	state.slices[3] = state.slices[2];
	state.slices[2] = state.slices[1];
	state.slices[1] = x0;
	state.slices[0] ^= state.slices[3];
	
	return state;
}

static inline State64Sliced_8_t lsfr_64_tk2_4bit_inverse(State64Sliced_8_t state) {
	auto prev_x3 = state.slices[0] ^ state.slices[3];
	
	state.slices[0] = state.slices[1];
	state.slices[1] = state.slices[2];
	state.slices[2] = state.slices[3];
	state.slices[3] = prev_x3;
	
	return state;
}

static inline State64Sliced_16_t lsfr_64_tk2_4bit(State64Sliced_16_t state) {
	auto x0 = state.slices[0];
	
	state.slices[0] = state.slices[3];
	state.slices[3] = state.slices[2];
	state.slices[2] = state.slices[1];
	state.slices[1] = x0;
	state.slices[0] ^= state.slices[3];
	
	return state;
}

static inline State64Sliced_16_t lsfr_64_tk2_4bit_inverse(State64Sliced_16_t state) {
	auto prev_x3 = state.slices[0] ^ state.slices[3];
	
	state.slices[0] = state.slices[1];
	state.slices[1] = state.slices[2];
	state.slices[2] = state.slices[3];
	state.slices[3] = prev_x3;
	
	return state;
}

/**
 * Pre-computes the key schedule for Forkskinny-64-192 for TK1
 * ks:
 * key:       pointer to key bytes; reads FORKSKINNY64_BLOCK_SIZE bytes from the key
 * nb_rounds: the number of rounds of the key schedule
 */
void forkskinny_c_64_192_init_tk1(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds);
/**
 * Pre-computes the key schedule for Forkskinny-64-192 for TK2 AND TK3
 * ks:
 * key:       pointer to key bytes; reads 2*FORKSKINNY64_BLOCK_SIZE bytes from the key
 * nb_rounds: the number of rounds of the key schedule
 */
void forkskinny_c_64_192_init_tk2_tk3(KeySchedule64_t *ks, const uint8_t *key, unsigned nb_rounds);

/**
 * Computes the forward direction of Forkskinny-64-192.
 * tks1:           key schedule for TK1 (see forkskinny_c_64_192_init_tk1)
 * tks2:           key schedule for TK2 and TK3 (see forkskinny_c_64_192_init_tk2_tk3)
 * output_left:   if NULL, the left leg is not computed, else pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the left output leg of the forkcipher
 * output_right:  pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the right output leg of the forkcipher
 * input:         pointer to FORKSKINNY64_BLOCK_SIZE byte; input to the forkcipher
 */
void forkskinny_c_64_192_encrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right,
                                 unsigned long *cycles_spent);

/**
 * Computes the inverse direction of Forkskinny-64-192.
 * tks1:           key schedule for TK1 (see forkskinny_c_64_192_init_tk1)
 * tks2:           key schedule for TK2 and TK3 (see forkskinny_c_64_192_init_tk2_tk3)
 * output_left:   if NULL, the left leg is not computed, else pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the left output leg of the forkcipher (i.e. mode 'o')
 * output_right:  pointer to FORKSKINNY64_BLOCK_SIZE byte; will contain the inverted input of the forkcipher (i.e. mode 'i')
 * input_right:   pointer to FORKSKINNY64_BLOCK_SIZE byte; input to the inverse forkcipher
 */
void forkskinny_c_64_192_decrypt(const KeySchedule64_t *tks1, const KeySchedule64_t *tks2,
                                 uint8_t *output_left, uint8_t *output_right, const uint8_t *input_right);

#endif //FORKSKINNYPLUS_FORKSKINNY64_PLUS_H
