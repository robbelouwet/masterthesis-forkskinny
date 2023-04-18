
#include <iostream>
#include <x86intrin.h>
#include "../forkskinny128-plus/utils/forkskinny128-datatypes.h"
#include "../forkskinny128-plus/keyschedule/fixsliced-keyschedule128.h"
#include "../forkskinny128-plus/forkskinny128.h"
#include "../test_vectors.h"
#include "../forkskinny128-plus/keyschedule/keyschedule128.h"

void benchmark_forkskinny128_384() {
	#define ITERATIONS 1000
	#define ROUNDS_BEFORE FORKSKINNY_128_384_ROUNDS_BEFORE
	#define ROUNDS_AFTER FORKSKINNY_128_384_ROUNDS_AFTER
	
	std::cout << slice_size << " blocks in parallel\n";
	std::cout << (ROUNDS_BEFORE + 2 * ROUNDS_AFTER) << " rounds per primitive call\n--------\n";
	
	auto before = _rdtsc();
	for (int i = 0; i < ITERATIONS; ++i) {
		auto schedule = forkskinny_128_fixsliced_init_tk23(TK1_128(), TK2_128(), TK3_128());
		
		auto pt_block = M_128();
		
		auto ct = forkskinny128_encrypt(schedule, &pt_block, 'b', ROUNDS_BEFORE,
		                      ROUNDS_AFTER);
		
		auto volatile res = unslice(ct.M);
	}
	auto after = _rdtsc();
	
	auto total = after - before;
	auto cycles_per_primitive = total / (ITERATIONS * slice_size);
	auto cycles_per_round = cycles_per_primitive / (ROUNDS_BEFORE + 2 * ROUNDS_AFTER);
	auto cycles_per_byte = cycles_per_primitive / 16;
	
	std::cout << cycles_per_primitive << " cycles per PRIMITIVE\n" ;
	std::cout << cycles_per_byte << " cycles per byte\n";
	std::cout << cycles_per_round << " cycles per round";
}

int main() {
	benchmark_forkskinny128_384();
}