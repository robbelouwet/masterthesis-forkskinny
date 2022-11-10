#include <iostream>
#include "../../headers/LSFR.h"
#include "../../headers/forkskinny64-cipher.h"
#include "immintrin.h"
#include "tuple"
#include "cmath"

/*
 *
 * lrotl // rotl a long
 *
 * rotating a byte within an AVX register
 * https://stackoverflow.com/a/36989455
 */


std::tuple<ulong, ulong> benchmark(State64v_t state) {
	auto *results = new ulong();
	
	// perform through SIMD
	ulong before = _rdtsc();
	
	// SHL: 0000 0000 0000 abcd -> 0000 0000 000a bcd0
	auto sleft = _mm256_slli_epi16(state.vec, 1);
	
	// blend according to control mask
	//auto mask = _mm256_set1_epi16(1); // broadcast value to every value in the lane
	//auto blended = _mm256_blend_epi16(sleft, state.vec, 8);
	ulong after = _rdtsc();
	results[0] = after - before;
	
	// perform through sequential loop
	ulong before2 = _rdtsc();
	for (int i = 0; i < 15; ++i) {
		state.cells16[i] <<= 1;
	}
	ulong after2 = _rdtsc();
	results[1] = after2 - before2;
	std::cout << "SIMD: " << after - before;
	std::cout << "\nSEQ: " << after2 - before2 << "\n##################\n";
	
	return {after - before, after2 - before2}; // [ simd, seq ]
}



void lsfr_64_tk2_4bit_simd(State64v_t *state) {
	// for every 16-bit lane in the 256-bit SIMD register:
	// a³a²a¹a⁰ | z³z²z¹z⁰ | y³y²y¹y⁰ | x³x²x¹x⁰
	
	/// First shift every lane left in zeros
	// a²a¹a⁰z³ | z²z¹z⁰y³ | y²y¹y⁰x³  | x²x¹x⁰0
	__m256i shifted = _mm256_slli_epi16(state->vec, 1);
	
	/// remove the overflown bit by masking every nibble with 1110 (0xE)
	// a²a¹a⁰0 | z²z¹z⁰0 | y²y¹y⁰0 | x²x¹x⁰0
	__m256i removed_overflown_bit = _mm256_and_si256(shifted, _mm256_set1_epi16((short) 0xEEEE));
	
	
	
	/// isolate the 4th bit of every nibble by masking with 1000 (0x8)
	// a³000 | z³000 | y³000 | x³000
	__m256i fourth_bit = _mm256_and_si256(state->vec, _mm256_set1_epi16((short) 0x8888));
	
	/// shift right to LSB
	// 000a³ | 000z³ | 000y³ | 000x³
	__m256i isolated_fourth_bit = _mm256_srli_epi16(fourth_bit, 3);
	
	/// isolate the 3rd bit of every nibble
	// 0a²00 | 0z²00 | 0y²00 | 0x²00
	__m256i third_bit = _mm256_and_si256(state->vec, _mm256_set1_epi16((short) 0x4444));
	
	/// shift it right to LSB
	__m256i isolated_third_bit = _mm256_srli_epi16(third_bit, 2);
	
	
	
	/// XOR third and fourth bit
	__m256i xored_third_fourth = _mm256_xor_si256(isolated_fourth_bit, isolated_third_bit);
	
	/// put this XORED bit at the end of the nibble
	state->vec = _mm256_or_si256(removed_overflown_bit, xored_third_fourth);
}

void lsfr_64_tk2_4bit(State64v_t *state) {
	((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U)
}
