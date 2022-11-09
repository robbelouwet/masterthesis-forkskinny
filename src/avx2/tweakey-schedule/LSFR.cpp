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


std::tuple<ulong, ulong> lsfr_64_tk2(State64Sliced_t state) {
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
		state.cells[i] <<= 1;
	}
	ulong after2 = _rdtsc();
	results[1] = after2 - before2;
	std::cout << "SIMD: " << after - before;
	std::cout << "\nSEQ: " << after2 - before2 << "\n##################\n";
	
	return {after - before, after2 - before2}; // [ simd, seq ]
}
