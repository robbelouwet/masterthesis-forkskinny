#include <iostream>
#include "immintrin.h"
#include "headers/LSFR.h"
#include "cmath"
#include <functional> // for std::greater, std::less
#include <algorithm>
#include <vector>
#include "tuple"

// https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=sl&ig_expand=6523,6501,6501&techs=AVX,AVX2
int main() {
	auto state = State64v_t();
	for (int j = 0; j <= 15; ++j) {
		state.cells16[j] = 65535;
	}
	
	ulong before = _rdtsc();
	lsfr_64_tk2_4bit_simd(state);
	ulong after = _rdtsc();
	
	std::cout << "semi-SIMD: " << after - before << " cycles";
	
	auto state2 = State64_t ();
	state2.llrow = 18446744073709551615;
	
	ulong before1 = _rdtsc();
	auto res = _mm_shuffle_epi8({}, {});
	//lsfr_64_tk2_4bit_sequential(state2.lrow[0]);
	ulong after1 = _rdtsc();
	std::cout << "Sequential: " << after1 - before1 << " cycles";
	
//	auto stat1 = HalfState64_t();
//	for (int j = 0; j <= 15; ++j) {
//		state.cells16[j] = 65535;
//	}
//	ulong before1 = _rdtsc();
//	lsfr_64_tk2_4bit_sequential()

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
	
	std::cout << "SIMD: on median " << simd_results[simd_results.size()/2] << " cycles spent";
	std::cout << "\nSEQ: on median " << seq_results[seq_results.size()/2] << " cycles spent";
}
