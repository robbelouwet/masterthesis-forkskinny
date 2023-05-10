
#include <iostream>
#include <x86intrin.h>
#include "../forkskinny128-plus/utils/forkskinny128-datatypes.h"
#include "../forkskinny128-plus/keyschedule/fixsliced-keyschedule128.h"
#include "../forkskinny128-plus/forkskinny128.h"
#include "../test_vectors.h"
#include <benchmark/benchmark.h>

void benchmark_forkskinny128_384() {
	#define ITERATIONS 100
	#define ROUNDS_BEFORE FORKSKINNY_128_384_ROUNDS_BEFORE
	#define ROUNDS_AFTER FORKSKINNY_128_384_ROUNDS_AFTER

	std::cout << slice_size << " blocks in parallel\n";
	std::cout << (ROUNDS_BEFORE + 2 * ROUNDS_AFTER) << " rounds per primitive call\n--------\n";

	Blocks128_t unsliced_test_M[ITERATIONS];
	Blocks128_t unsliced_test_TK1[ITERATIONS];
	Blocks128_t unsliced_test_TK2[ITERATIONS];
	Blocks128_t unsliced_test_TK3[ITERATIONS];

	State128Sliced_t test_M[ITERATIONS];
	State128Sliced_t test_TK1[ITERATIONS];
	State128Sliced_t test_TK2[ITERATIONS];
	State128Sliced_t test_TK3[ITERATIONS];

	// Generate test vectors
	for (int i = 0; i < ITERATIONS; ++i) {
		unsliced_test_M[i] = M_128();
		unsliced_test_TK1[i] = TK1_128();
		unsliced_test_TK2[i] = TK2_128();
		unsliced_test_TK3[i] = TK3_128();
		
//		assert((uintptr_t) &(unsliced_test_M[i]) % 32 == 0);
	}

	auto before0 = _rdtsc();
	for (int i = 0; i < ITERATIONS; ++i) {
		test_M[i] = slice(unsliced_test_M[i]);
		test_TK1[i] = slice(unsliced_test_TK1[i]);
		test_TK2[i] = slice(unsliced_test_TK2[i]);
		test_TK3[i] = slice(unsliced_test_TK3[i]);
	}
	auto after0 = _rdtsc();
//
	auto slicing_per_primitive = (after0 - before0) / (ITERATIONS * slice_size);
	std::cout << slicing_per_primitive << " spent on slicing per single PRIMITIVE call\n";

	auto before = _rdtsc();
	for (int i = 0; i < ITERATIONS; ++i) {
		auto schedule = forkskinny_128_fixsliced_init_tk23(test_TK1[i], test_TK2[i], test_TK3[i]);

		auto pt_block = test_M[i];

		auto ct = forkskinny128_encrypt(&schedule, &pt_block, 'b', ROUNDS_BEFORE,
		                                ROUNDS_AFTER);

		auto volatile res = unslice_accelerated(ct.M);
	}
	auto after = _rdtsc();

	auto total = (after - before) + (after0 - before0);
	auto cycles_per_primitive = total / (ITERATIONS * slice_size);
	auto cycles_per_round = cycles_per_primitive / (ROUNDS_BEFORE + 2 * ROUNDS_AFTER);
	auto cycles_per_byte = cycles_per_primitive / 16;

	std::cout << cycles_per_primitive + slicing_per_primitive << " total cycles per single PRIMITIVE call (slicing included)\n" ;
	std::cout << cycles_per_byte << " cycles per byte\n";
	std::cout << cycles_per_round << " cycles per round";
}

int main(){
	benchmark_forkskinny128_384();
}

//SlicedCiphertext128_t benchmark_fs128_encryption() {
//	#define ROUNDS_BEFORE FORKSKINNY_128_384_ROUNDS_BEFORE
//	#define ROUNDS_AFTER FORKSKINNY_128_384_ROUNDS_AFTER
//	auto m = slice(M_128());
//	auto tk1 = slice(TK1_128());
//	auto tk2 = slice(TK2_128());
//	auto tk3 = slice(TK3_128());
//
//	auto schedule = forkskinny_128_fixsliced_init_tk23(tk1, tk2, tk3);
//	auto ct = forkskinny128_encrypt(&schedule, &m, 'b', ROUNDS_BEFORE, ROUNDS_AFTER);
//
//	return ct;
//}
//
//void run_benchmark_fs128(benchmark::State &state) {
//	for (auto _: state) {
//		auto ct = benchmark_fs128_encryption();
//		auto volatile res = unslice_accelerated(ct.M);
//	}
//}
//
//// 75589 - 8945
//BENCHMARK(run_benchmark_fs128);
//
//BENCHMARK_MAIN();