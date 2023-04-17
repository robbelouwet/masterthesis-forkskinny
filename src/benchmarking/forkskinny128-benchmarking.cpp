
#include <iostream>
#include <x86intrin.h>
#include "../forkskinny128-plus/utils/forkskinny128-datatypes.h"
#include "../forkskinny128-plus/keyschedule/fixsliced-keyschedule128.h"
#include "../forkskinny128-plus/forkskinny128.h"
#include "../test_vectors.h"
#include "../forkskinny128-plus/keyschedule/keyschedule128.h"

void benchmark_forkskinny128_384() {
	#define ITERATIONS 10000
	
	State128Sliced_t test_states[ITERATIONS];
	State128Sliced_t test_tk1[ITERATIONS];
	State128Sliced_t test_tk2[ITERATIONS];
	State128Sliced_t test_tk3[ITERATIONS];
	
	for (int i = 0; i < ITERATIONS; ++i) {
		test_states[i] = M_128();
		test_tk1[i] = TK1_128();
		test_tk2[i] = TK2_128();
		test_tk3[i] = TK3_128();
	}
	
	auto before = _rdtsc();
	for (int i = 0; i < ITERATIONS; ++i) {
		auto state = test_states[i];
		auto const schedule = forkskinny_128_init_tk23(test_tk1[i], test_tk2[i],test_tk3[i]);
		forkskinny128_encrypt(schedule, &state, 'b', FORKSKINNY_128_384_ROUNDS_BEFORE, FORKSKINNY_128_384_ROUNDS_AFTER);
	}
	auto after = _rdtsc();
	
	auto total = after - before;
	auto total_per_primitive = total / slice_size;
	auto cycles_per_round = total_per_primitive / (FORKSKINNY_128_384_ROUNDS_BEFORE + FORKSKINNY_128_384_ROUNDS_AFTER);
	
	std::cout << slice_size << " blocks in parallel\n";
	std::cout << (FORKSKINNY_128_384_ROUNDS_BEFORE + FORKSKINNY_128_384_ROUNDS_AFTER) << " rounds per primitive call";
	std::cout << "on average " << cycles_per_round << " cycles per round";
}

int main(){
	benchmark_forkskinny128_384();
}