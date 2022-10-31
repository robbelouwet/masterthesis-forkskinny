#include <x86intrin.h>
#include <iostream>

void test_simd() {
	auto before = _rdtsc();
	int rijke[16][16];
	
	// doesn't give any performance change, why?
	#pragma clang loop vectorize(assume_safety)
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			rijke[i][j] = i ^ j;
		}
	}
	
	auto after = _rdtsc();
	
	std::cout << after - before << " cycles spent";
}

int main() {
	test_simd();
	//test_benchmark();
}
