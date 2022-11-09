#include <iostream>
#include "x86intrin.h"
#include "headers/LSFR.h"
#include "cmath"
#include <functional> // for std::greater, std::less
#include <algorithm>
#include <vector>
#include "tuple"

// https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=sl&ig_expand=6523,6501,6501&techs=AVX,AVX2
int main() {
	uint iterations = 1000;
	
	auto simd_results = std::vector<ulong>();
	auto seq_results = std::vector<ulong>();
	
	for (uint i = 0; i < iterations; ++i) {
		auto state = State64Sliced_t();
		for (int j = 0; j <= 15; ++j) {
			state.cells[j] = 15;
		}
		
		auto[simd, seq] = lsfr_64_tk2(state);
		simd_results.push_back(simd);
		seq_results.push_back(seq);
		
	}
	
	std::sort(simd_results.begin(), simd_results.end());
	std::sort(seq_results.begin(), seq_results.end());
	
	std::cout << "SIMD: on median " << simd_results[simd_results.size()/2] << " cycles spent";
	std::cout << "\nSEQ: on median " << seq_results[seq_results.size()/2] << " cycles spent";
}
