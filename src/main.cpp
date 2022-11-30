#include <x86intrin.h>
#include <cassert>
#include <vector>
#include "headers/full-state-slicing.h"
#include "forkskinny-plus/headers/forkskinny-plus.h"
#include <immintrin.h>
#include <iostream>

static inline uint32_t skinny64_LFSR2(uint32_t x) {
	return ((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U);
}

static inline uint64_t skinny64_LFSR2_full(uint64_t x) {
	return ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
}

// <editor-fold desc="lsfr-benchmark">
std::vector<uint64_t> benchmark_lsfr(uint64_t state) {
	auto t = slice(state);
	auto sliced = State64Sliced_16_t();
	sliced.state = t;

//	std::cout << "Version 3\n";
	//
	asm("nop");
	asm("nop");
	sliced.m64state = _mm_shuffle_pi16(sliced.m64state,
	                                   0b10010011);  //indices 3210 go to 2103; 2103 is 10 01 00 11 in binary
	sliced.slices[0] ^= sliced.slices[3];
	asm("nop");
	asm("nop");
	
	
	// Bit sliced LSFR, only the 3rd and 2nd significant slices are XOR'd
	auto sliced2 = State64Sliced_16_t();
	sliced2.state = t;
	asm("nop");
	asm("nop");
	asm("nop");
	sliced2.state = _lrotl(sliced2.state, 0x10);
	sliced2.state ^= (sliced2.state & 0xFFFF000000000000) >> 0x30;
//	sliced2.slices[0] ^= sliced2.slices[3];
	asm("nop");
	asm("nop");
	asm("nop");
	
	
	// Sequential LSFR (no bit slicing or simd)
	auto x = state;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	x = (
			    (x << 1)
			    &
			    (uint64_t) 0xEEEEEEEEEEEEEEEE)
	    ^
	    (
			    (
					    (x >> 3) ^ (x >> 2))
			    & (uint64_t) 0x1111111111111111);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	
	
	auto unsliced = unslice(sliced.state);
	auto unsliced2 = unslice(sliced2.state);
	assert(unsliced2 != 0 || unsliced != 0);
	assert(unsliced2 == unsliced);
	assert(unsliced2 == x);
	
	// instead of 'return null' or something, make it difficult for the compiler to optimize out the previous code due to
	// dead-code or unused variables optimizations
	// otherwise, the previous code won't be present in the binary
	auto res = std::vector<uint64_t>();
	if (sliced2.state != 0) res.push_back(unsliced);
	if (sliced.state != 0) res.push_back(unsliced2);
	if (x != 0) res.push_back(x);
	
	return res;
}
//</editor-fold>

int main(int argc, char **argv) {
//	if (argc == 0) return benchmark_lsfr(0xABCDABCDABCDABCD).size();
//	if (argc == 1) return benchmark_lsfr(0xEFFEEFFEEFFEEFFE).size();
	
	auto res = unslice(0x1111000011110000);
	int appel = 1;

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
