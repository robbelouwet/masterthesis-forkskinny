#include <iostream>
#include <x86intrin.h>
#include <cassert>
#include <algorithm>
#include <vector>
#include "headers/full-state-slicing.h"
#include "forkskinny-plus/headers/forkskinny-plus.h"

static inline uint32_t skinny64_LFSR2(uint32_t x) {
	return ((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U);
}

static inline uint64_t skinny64_LFSR2_full(uint64_t x) {
	return ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
}

// <editor-fold desc="lsfr-benchmark">
std::vector<ulong> benchmark_lsfr() {
	uint64_t state = 0xEEEEEEEEEEEEEEEE;
	
	auto t = slice(state);
	auto sliced = State64Sliced_16_t();
	sliced.state = t;

//	std::cout << "Version 3\n";
	auto before = _rdtsc();
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state, 0b10010011);
	sliced.slices[0] ^= sliced.slices[3];
	auto after = _rdtsc();
	
	auto sliced2 = State64Sliced_16_t();
	sliced2.state = slice(state);
	auto before1 = _rdtsc();
	auto x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	
	x0 = sliced2.slices[0];
	sliced2.slices[0] = sliced2.slices[3];
	sliced2.slices[3] = sliced2.slices[2];
	sliced2.slices[2] = sliced2.slices[1];
	sliced2.slices[1] = x0;
	sliced2.slices[0] ^= sliced2.slices[3];
	auto after1 = _rdtsc();
	
	auto x = state;
	auto before2 = _rdtsc();
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	
	auto after2 = _rdtsc();
	
	
	auto unsliced = unslice(sliced.state);
	auto unsliced2 = unslice(sliced2.state);
	assert(unsliced2 != 0 || unsliced != 0);
	assert(unsliced2 == unsliced);
	assert(unsliced2 == x);
	
	std::cout << "Bit sliced + SIMD LSFR:   " << after - before << " cycles\n";
	std::cout << "Bit sliced only LSFR:     " << after1 - before1 << " cycles, " << sliced2.state << "\n";
	std::cout << "Sequential LSFR:          " << after2 - before2 << " cycles, " << x << "\n\n";
	
	
	return {after - before, after1 - before1, after2 - before2};
}
//</editor-fold>

int main() {
	benchmark_lsfr();
//	std::vector<ulong> bitsliced_simd = std::vector<ulong>();
//	std::vector<ulong> bitsliced = std::vector<ulong>();
//	std::vector<ulong> sequential = std::vector<ulong>();
//
//	for (int i = 0; i < 10; ++i) {
//		auto res = benchmark_lsfr();
//
//		bitsliced_simd.push_back(res.at(0));
//		bitsliced.push_back(res.at(1));
//		sequential.push_back(res.at(2));
//	}
//
//	std::sort(bitsliced.begin(), bitsliced.end());
//	std::sort(bitsliced_simd.begin(), bitsliced_simd.end());
//	std::sort(sequential.begin(), sequential.end());
//
//	std::cout << "Bit sliced + SIMD LSFR:   " << bitsliced_simd.at(bitsliced_simd.size() / 2) << " cycles\n";
//	std::cout << "Bit sliced only LSFR:     " << bitsliced.at(bitsliced.size() / 2) << " cycles\n";
//	std::cout << "Sequential LSFR:          " << sequential.at(sequential.size() / 2) << " cycles";
	
	return 0;
}
