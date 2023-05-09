#include <iostream>
#include <x86intrin.h>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../AEAD/PAEF.h"
#include "../forkskinny64-plus/utils/slicing64-accelerated.h"
#include "../forkskinny64-plus/keyschedule/keyschedule64.h"
#include <benchmark/benchmark.h>

void benchmark_PAEF_forkskinny64_192() {
	
	#define SEGMENTS 100
	
	std::cout << "PAEF 64 - 192; SAMPLING " << SEGMENTS * slice_size << " BLOCKS (half AD, half M)" << std::endl;
	std::cout << slice_size << " blocks in parallel" << std::endl;
	
	Blocks64_t M_blocks[SEGMENTS];
	Blocks64_t AD_blocks[SEGMENTS];
	for (int i = 0; i < SEGMENTS; i += 2) {
		M_blocks[i] = M_rand_64(i);
		AD_blocks[i] = M_rand_64(i + 1);
	}
	
	/// Nonce N
	Block64_t nonce[2] = {{.raw = 0xFEDCBA9876543210},
	                      {.raw = 0xAAAAAAAA}};
	int nonce_bit_length = 96;
	
	/// Associated Data
	// 1 full and 1 partial AD segment, last AD segment has last_AD_block blocks
	auto size_AD = SEGMENTS;
	auto last_AD_block = (slice_size >> 1) + 5;
	for (int i = slice_size - 1; i > last_AD_block; --i) AD_blocks[1].values[i].raw = 0;
	
	/// Message
	// 1 full and 1 partial AD segment, last AD segment has last_AD_block blocks
	auto size_M = SEGMENTS;
	auto last_M_block = (slice_size >> 1) - 9;
	for (int i = slice_size - 1; i > last_M_block; --i) M_blocks[1].values[i].raw = 0;
	
	Blocks64_t ct[SEGMENTS];
	auto before_AD = _rdtsc();
	auto const AD_tag = paef_forkskinny64_192_encrypt_AD(
			AD_blocks, last_AD_block, size_AD, nonce, nonce_bit_length);
	auto after_AD = _rdtsc();
	
	auto before_M = _rdtsc();
	auto const M_tag = paef_forkskinny64_192_encrypt_M(
			M_blocks, last_M_block, size_M, nonce, nonce_bit_length, ct);
	auto after_M = _rdtsc();
	
	auto tag = AD_tag ^ M_tag;
	
	auto cycles_per_AD_block = (after_AD - before_AD) / (SEGMENTS * slice_size);
	std::cout << cycles_per_AD_block << " cycles per PRIMITIVE (AD - "
	          << FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER << " rounds)" << std::endl;
	std::cout << cycles_per_AD_block / 8 << " cycles per byte (AD)" << std::endl;
	
	auto cycles_per_M_block = (after_M - before_M) / (SEGMENTS * slice_size);
	std::cout << cycles_per_M_block << " cycles PRIMITIVE (M - " << FORKSKINNY64_MAX_ROUNDS << " rounds)" << std::endl;
	std::cout << cycles_per_M_block / 8 << " cycles per byte (M)" << std::endl;
	
	std::cout << "Tag: " << std::hex << tag << std::endl;
	std::cout << "C (first 8 bytes): " << std::hex << ct[0].values[0].raw << std::endl;
}

void benchmark_forkskinny64_192() {
	#define ITERATIONS 199
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
	
	unsigned long long slice_timings[ITERATIONS];
	for (int i = 0; i < ITERATIONS; ++i) {
		auto before = _rdtsc();
		test_M[i] = slice_accelerated(unsliced_test_M[i]);
		test_TK1[i] = slice_accelerated(unsliced_test_TK1[i]);
		test_TK2[i] = slice_accelerated(unsliced_test_TK2[i]);
		test_TK3[i] = slice_accelerated(unsliced_test_TK3[i]);
		auto after = _rdtsc();
		slice_timings[i] = after - before;
	}
	
	KeySchedule64Sliced_t schedules[ITERATIONS];
	unsigned long long schedule_timings[ITERATIONS];
	for (int i = 0; i < ITERATIONS; ++i) {
		auto before = _rdtsc();
		schedules[i] = forkskinny_64_init_tk23(test_TK1 + i, test_TK2 + i, test_TK3 + i);
		schedule_timings[i] = _rdtsc() - before;
	}
	
	SlicedCiphertext64_t cts[ITERATIONS];
	unsigned long long encryption_timings[ITERATIONS];
	for (int i = 0; i < ITERATIONS; ++i) {
		auto before = _rdtsc();
		
		auto pt_block = test_M[i];
		
		cts[i] = forkskinny64_encrypt(schedules + i, &pt_block, 'b');
		auto after = _rdtsc();
		encryption_timings[i] = after - before;
	}
	
	Blocks64_t unsliced_cts[ITERATIONS];
	
	unsigned long long unslice_timings[ITERATIONS];
	for (int i = 0; i < ITERATIONS; ++i) {
		auto before = _rdtsc();
		unsliced_cts[i] = unslice_accelerated(cts[i].C0);
		auto after = _rdtsc();
		unslice_timings[i] = 0;//after - before;
	}
	
	// sort the timings
	qsort(slice_timings, ITERATIONS, sizeof(unsigned long long), cmp_dbl);
	qsort(schedule_timings, ITERATIONS, sizeof(unsigned long long), cmp_dbl);
	qsort(encryption_timings, ITERATIONS, sizeof(unsigned long long), cmp_dbl);
	qsort(unslice_timings, ITERATIONS, sizeof(unsigned long long), cmp_dbl);
	
	double cycles_slicing_per_pack = slice_timings[0];
	double cycles_schedule_per_pack = schedule_timings[0];
	double cycles_encryption_per_pack = encryption_timings[0];
	double cycles_unslicing_per_pack = unslice_timings[0];
	
	double total_per_block =
			(cycles_unslicing_per_pack + cycles_slicing_per_pack + cycles_encryption_per_pack +
			 cycles_schedule_per_pack) / slice_size;
	double total_per_primitive = cycles_encryption_per_pack / slice_size;
	double cycles_per_round = total_per_block / (ROUNDS_BEFORE + 2 * ROUNDS_AFTER);
	double cycles_per_byte = total_per_block / 8;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single PRIMITIVE call\n";
	std::cout << encryption_per_primitive + schedule_per_primitive
	          << " cycles per single PRIMITIVE call (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE\n";
	std::cout << cycles_per_byte << " cycles per byte\n";
	std::cout << ((cycles_per_byte / (ROUNDS_BEFORE + 2 * ROUNDS_AFTER))) * 36 << " cycles per byte per 36 rounds\n";
	std::cout << cycles_per_round << " cycles per round";
	
	for (int i = 0; i < ITERATIONS; ++i) {
		assert(unsliced_cts[i].values[0].raw == 0x502A9310B9F164FF);
	}
	
	std::cout << "\nSuccess!";
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
	auto ct = forkskinny64_encrypt(&schedule, &test_M, 'b');
	
	// UNSLICE
	return unslice_accelerated(ct.M);
}

void run_benchmark_fs64(benchmark::State &state) {
	for (auto _: state) {
		benchmark_single_forkskinny64_192(M_64(), TK1_64(), TK2_64(), TK3_64());
	}
}

int main() {
	benchmark_forkskinny64_192();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
//	benchmark_PAEF_forkskinny64_192();
}
//BENCHMARK(run_benchmark_fs64);
//BENCHMARK_MAIN();

