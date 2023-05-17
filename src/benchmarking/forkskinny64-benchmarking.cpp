#include <iostream>
#include <x86intrin.h>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../AEAD/PAEF.h"
#include "forkskinny64-benchmark-iteration.h"
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

void benchmark_forkskinny64_128() {
	std::cout << "\nFORKSKINNY64-128\n";
	auto iterations = 5000;
	auto rounds_before = FORKSKINNY_ROUNDS_BEFORE;
	auto rounds_after = FORKSKINNY_ROUNDS_AFTER;
	
	std::cout << slice_size << " blocks in parallel\n";
	std::cout << (rounds_before + 2 * rounds_after) << " rounds per primitive call\n--------\n";
	
	ULL slice_timings[iterations], schedule_timings[iterations], encryption_timings[iterations], unslice_timings[iterations];
	for (int i = 0; i < iterations; ++i)
		benchmark_forkskinny64_128_sb(
				slice_timings + i,
				schedule_timings + i,
				encryption_timings + i,
				unslice_timings + i
		);
	
	// sort the timings
	qsort(slice_timings, iterations, sizeof(ULL), compare);
	qsort(schedule_timings, iterations, sizeof(ULL), compare);
	qsort(encryption_timings, iterations, sizeof(ULL), compare);
	qsort(unslice_timings, iterations, sizeof(ULL), compare);
	
	// the encryption samples use the same keys and plaintext and so are deterministic. Any noise that slows it down
	// comes from side channels, so we can just assume the fastest one
	double cycles_slicing_per_pack = slice_timings[0];
	double cycles_schedule_per_pack = schedule_timings[0];
	double cycles_encryption_per_pack = encryption_timings[0];
	double cycles_unslicing_per_pack = unslice_timings[0];
	
	double total_per_block =
			(cycles_unslicing_per_pack + cycles_slicing_per_pack + cycles_encryption_per_pack +
			 cycles_schedule_per_pack) / slice_size;
	double cycles_per_round = total_per_block / (rounds_before + 2 * rounds_after);
	double cycles_per_byte = total_per_block / 8;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single PRIMITIVE call\n";
	std::cout << encryption_per_primitive + schedule_per_primitive
	          << " cycles per single PRIMITIVE call (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE\n";
	std::cout << cycles_per_byte << " cycles per byte\n";
	std::cout << ((cycles_per_byte / (rounds_before + 2 * rounds_after))) * 36 << " cycles per byte per 36 rounds\n";
	std::cout << cycles_per_round << " cycles per round";
}

void benchmark_forkskinny64_192() {
	std::cout << "\nFORKSKINNY64-192\n";
	auto iterations = 5000;
	auto rounds_before =  FORKSKINNY_ROUNDS_BEFORE;
	auto rounds_after = FORKSKINNY_ROUNDS_AFTER;
	
	std::cout << slice_size << " blocks in parallel\n";
	std::cout << (rounds_before + 2 * rounds_after) << " rounds per primitive call\n--------\n";
	
	ULL slice_timings[iterations], schedule_timings[iterations], encryption_timings[iterations], unslice_timings[iterations];
	for (int i = 0; i < iterations; ++i)
		benchmark_forkskinny64_192_sb(
				slice_timings + i,
				schedule_timings + i,
				encryption_timings + i,
				unslice_timings + i
		);
	
	// sort the timings
	qsort(slice_timings, iterations, sizeof(ULL), compare);
	qsort(schedule_timings, iterations, sizeof(ULL), compare);
	qsort(encryption_timings, iterations, sizeof(ULL), compare);
	qsort(unslice_timings, iterations, sizeof(ULL), compare);
	
	// the encryption samples use the same keys and plaintext and so are deterministic. Any noise that slows it down
	// comes from side channels, so we can just assume the fastest one
	double cycles_slicing_per_pack = slice_timings[0];
	double cycles_schedule_per_pack = schedule_timings[0];
	double cycles_encryption_per_pack = encryption_timings[0];
	double cycles_unslicing_per_pack = unslice_timings[0];
	
	double total_per_block =
			(cycles_unslicing_per_pack + cycles_slicing_per_pack + cycles_encryption_per_pack +
			 cycles_schedule_per_pack) / slice_size;
	double cycles_per_round = total_per_block / (rounds_before + 2 * rounds_after);
	double cycles_per_byte = total_per_block / 8;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single PRIMITIVE call\n";
	std::cout << encryption_per_primitive + schedule_per_primitive
	          << " cycles per single PRIMITIVE call (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE\n";
	std::cout << cycles_per_byte << " cycles per byte\n";
	std::cout << ((cycles_per_byte / (rounds_before + 2 * rounds_after))) * 36 << " cycles per byte per 36 rounds\n";
	std::cout << cycles_per_round << " cycles per round";
}

int main() {
	benchmark_forkskinny64_192();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++";
	benchmark_forkskinny64_128();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
//	benchmark_PAEF_forkskinny64_192();
}
//BENCHMARK(run_benchmark_fs64);
//BENCHMARK_MAIN();

