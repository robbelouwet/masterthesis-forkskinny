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
	//auto blended = _mm256_blend_epi16(sleft, internal_state.vec, 8);
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


State64v_t lsfr_64_tk2_4bit_simd(State64v_t state) {
	// for every 16-bit lane in the 256-bit SIMD register:
	// a³a²a¹a⁰ | z³z²z¹z⁰ | y³y²y¹y⁰ | x³x²x¹x⁰
	
	ulong last_cp = _rdtsc();
	
	/// First shift every lane left in zeros
	// a²a¹a⁰z³ | z²z¹z⁰y³ | y²y¹y⁰x³  | x²x¹x⁰0
	__m128i shifted = _mm_slli_epi16(state.hvec[0], 1);
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_slli_epi16\n";
//	last_cp = _rdtsc() - last_cp;
	
	/// remove the overflown bit by masking every nibble with 1110 (0xE)
	// a²a¹a⁰0 | z²z¹z⁰0 | y²y¹y⁰0 | x²x¹x⁰0
	__m128i removed_overflown_bit = _mm_and_si128(shifted, _mm_set1_epi16((short) 0xEEEE));
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_and_si128\n";
//	last_cp = _rdtsc() - last_cp;
	
	/// isolate the 4th bit of every nibble by masking with 1000 (0x8)
	// a³000 | z³000 | y³000 | x³000
	__m128i fourth_bit = _mm_and_si128(state.hvec[0], _mm_set1_epi16((short) 0x8888));
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_and_si128\n";
//	last_cp = _rdtsc() - last_cp;
	
	/// shift right to LSB
	// 000a³ | 000z³ | 000y³ | 000x³
	__m128i isolated_fourth_bit = _mm_srli_epi16(fourth_bit, 3);
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_srli_epi16\n";
//	last_cp = _rdtsc() - last_cp;
	
	
	/// isolate the 3rd bit of every nibble
	// 0a²00 | 0z²00 | 0y²00 | 0x²00
	__m128i third_bit = _mm_and_si128(state.hvec[0], _mm_set1_epi16((short) 0x4444));
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_and_si128\n";
//	last_cp = _rdtsc() - last_cp;
	
	
	/// shift it right to LSB
	__m128i isolated_third_bit = _mm_srli_epi16(third_bit, 2);
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_srli_epi16\n";
//	last_cp = _rdtsc() - last_cp;
	
	
	/// XOR third and fourth bit
	__m128i xored_third_fourth = _mm_xor_si128(isolated_fourth_bit, isolated_third_bit);
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_xor_si128\n";
//	last_cp = _rdtsc() - last_cp;
	
	
	/// put this XORED bit at the end of the nibble
	state.hvec[0] = _mm_or_si128(removed_overflown_bit, xored_third_fourth);
//	last_cp = _rdtsc() - last_cp;
//	std::cout << last_cp << " cycles on _mm_or_si128\n";
//	last_cp = _rdtsc() - last_cp;
	
	return state;
}

uint32_t lsfr_64_tk2_4bit_sequential(uint32_t state) {
	return (
			       (state << 1)
			       & 0xEEEEEEEEU
	       )
	       ^
	       (
			       ((state >> 3) ^ (state >> 2))
			       & 0x11111111U
	       );
}
