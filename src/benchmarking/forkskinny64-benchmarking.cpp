#include <iostream>
#include <x86intrin.h>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include <benchmark/benchmark.h>

void benchmark_forkskinny64_192() {
	#define ITERATIONS 100
	#define ROUNDS_BEFORE FORKSKINNY_ROUNDS_BEFORE
	#define ROUNDS_AFTER FORKSKINNY_ROUNDS_AFTER
	
	std::cout << slice_size << " blocks in parallel\n";
	std::cout << (ROUNDS_BEFORE + 2 * ROUNDS_AFTER) << " rounds per primitive call\n--------\n";
	
	Blocks64_t unsliced_test_M[ITERATIONS];
	Blocks64_t unsliced_test_TK1[ITERATIONS];
	Blocks64_t unsliced_test_TK2[ITERATIONS];
	Blocks64_t unsliced_test_TK3[ITERATIONS];
	
	State64Sliced_t test_M[ITERATIONS];
	State64Sliced_t test_TK1[ITERATIONS];
	State64Sliced_t test_TK2[ITERATIONS];
	State64Sliced_t test_TK3[ITERATIONS];
	
	// Generate test vectors
	for (int i = 0; i < ITERATIONS; ++i) {
		unsliced_test_M[i] = M_64();
		unsliced_test_TK1[i] = TK1_64();
		unsliced_test_TK2[i] = TK2_64();
		unsliced_test_TK3[i] = TK3_64();
	}
	
	auto before0 = _rdtsc();
	for (int i = 0; i < ITERATIONS; ++i) {
		test_M[i] = slice(unsliced_test_M[i]);
		test_TK1[i] = slice(unsliced_test_TK1[i]);
		test_TK2[i] = slice(unsliced_test_TK2[i]);
		test_TK3[i] = slice(unsliced_test_TK3[i]);
	}
	auto after0 = _rdtsc();
	
	auto slicing_per_primitive = (after0 - before0) / (ITERATIONS * slice_size);
	std::cout << slicing_per_primitive << " spent on slicing per single PRIMITIVE call\n";
	
	auto before = _rdtsc();
	for (int i = 0; i < ITERATIONS; ++i) {
		auto schedule = forkskinny_64_fixsliced_init_tk23(test_TK1[i], test_TK2[i], test_TK3[i]);
		
		auto pt_block = test_M[i];
		
		auto ct = forkskinny64_encrypt(schedule, &pt_block, 'b');
		
		auto volatile res = unslice(ct.M);
	}
	auto after = _rdtsc();
	
	auto total = after - before;
	auto cycles_per_primitive = total / (ITERATIONS * slice_size);
	auto cycles_per_round = cycles_per_primitive / (ROUNDS_BEFORE + 2 * ROUNDS_AFTER);
	auto cycles_per_byte = cycles_per_primitive / 16;
	
	std::cout << cycles_per_primitive + slicing_per_primitive
	          << " total cycles per single PRIMITIVE call (slicing included)\n";
	std::cout << cycles_per_byte << " cycles per byte\n";
	std::cout << cycles_per_round << " cycles per round";
}

Blocks64_t benchmark_single_forkskinny64_192(Blocks64_t unsliced_m, Blocks64_t unsliced_tk1, Blocks64_t unsliced_tk2,
                                Blocks64_t unsliced_tk3) {
	
	#define ROUNDS_BEFORE FORKSKINNY_ROUNDS_BEFORE
	#define ROUNDS_AFTER FORKSKINNY_ROUNDS_AFTER
	
	// SLICE
	auto test_M = slice(unsliced_m);
	auto test_TK1 = slice(unsliced_tk1);
	auto test_TK2 = slice(unsliced_tk2);
	auto test_TK3 = slice(unsliced_tk3);
	
	// PRIMITIVE
	auto schedule = forkskinny_64_fixsliced_init_tk23(test_TK1, test_TK2, test_TK3);
	auto ct = forkskinny64_encrypt(schedule, &test_M, 'b');
	
	// UNSLICE
	return unslice(ct.M);
}

void run_benchmark_fs64(benchmark::State& state){
	for (auto _ : state){
		benchmark_single_forkskinny64_192(M_64(), TK1_64(), TK2_64(), TK3_64());
	}
}

int main() {
	benchmark_forkskinny64_192();
}
//BENCHMARK(run_benchmark_fs64);
//BENCHMARK_MAIN();

