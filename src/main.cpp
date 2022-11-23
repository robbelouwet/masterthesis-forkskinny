#include <iostream>
#include "immintrin.h"
#include "headers/LSFR.h"
#include "cmath"
#include <functional> // for std::greater, std::less
#include <algorithm>
#include <vector>
#include "tuple"
#include "slicing/full-state-slicing.h"
#include <chrono>

using namespace std::chrono;

static inline uint32_t skinny64_LFSR2(uint32_t x) {
	return ((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U);
}

static inline uint64_t skinny64_LFSR2_full(uint64_t x) {
	return ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
}

int main() {
	auto state = State64_t();
	state.llrow = 0xEEEEEEEEEEEEEEEE;
	
	auto t = slice(state.llrow);
	auto sliced = State64Sliced_8_t();
	sliced.state = t;
	auto before = _rdtsc();
	auto temp = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = temp;
	sliced.slices[0] ^= sliced.slices[3];
	
	temp = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = temp;
	sliced.slices[0] ^= sliced.slices[3];
	
	temp = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = temp;
	sliced.slices[0] ^= sliced.slices[3];
	
	temp = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = temp;
	sliced.slices[0] ^= sliced.slices[3];
	
	temp = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = temp;
	sliced.slices[0] ^= sliced.slices[3];
	auto after = _rdtsc();
	
	auto before1 = _rdtsc();
	skinny64_LFSR2_full(state.llrow);
	skinny64_LFSR2_full(state.llrow);
	skinny64_LFSR2_full(state.llrow);
	skinny64_LFSR2_full(state.llrow);
	skinny64_LFSR2_full(state.llrow);
	auto after1 = _rdtsc();
	
	std::cout << "Bit sliced LSFR on 8 cells: " << after - before << " cycles, " << "\n";
	std::cout << "Sequential LSFR on 8 cells: " << after1 - before1 << " cycles, ";
	
	auto unsliced = unslice(t);
	//assert(unsliced == state.lrow[0]);
	auto appel = 1;
}

void shl_benchmark() {
	uint iterations = 1000;
	
	auto simd_results = std::vector<ulong>();
	auto seq_results = std::vector<ulong>();
	
	for (uint i = 0; i < iterations; ++i) {
		auto state = State64v_t();
		for (int j = 0; j <= 15; ++j) {
			state.cells16[j] = 15;
		}
		
		auto[simd, seq] = benchmark(state);
		simd_results.push_back(simd);
		seq_results.push_back(seq);
		
	}
	
	std::sort(simd_results.begin(), simd_results.end());
	std::sort(seq_results.begin(), seq_results.end());
	
	std::cout << "SIMD: on median " << simd_results[simd_results.size() / 2] << " cycles spent";
	std::cout << "\nSEQ: on median " << seq_results[seq_results.size() / 2] << " cycles spent";
}
