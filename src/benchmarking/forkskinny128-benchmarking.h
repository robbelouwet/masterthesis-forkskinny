#include <iostream>
#include <x86intrin.h>
#include "../forkskinny128-plus/utils/forkskinny128-datatypes.h"
#include "../forkskinny128-plus/keyschedule/fixsliced-keyschedule128.h"
#include "../forkskinny128-plus/forkskinny128.h"
#include "../test_vectors.h"
#include "forkskinny128-benchmark-iteration.h"
#include <benchmark/benchmark.h>

static inline void benchmark_forkskinny128_256() {
	std::cout << "\nFORKSKINNY128-256 (s=0)\n";
	auto iterations = 5000;
	auto rounds_before = FORKSKINNY_128_256_ROUNDS_BEFORE;
	auto rounds_after = FORKSKINNY_128_256_ROUNDS_AFTER;
	auto rounds = rounds_before + rounds_after;
	
	std::cout << std::dec << slice_size << " blocks in parallel\n";
	std::cout << rounds << " rounds per primitive call\n--------\n";
	
	ULL slice_timings[iterations], schedule_timings[iterations], encryption_timings[iterations],
			decryption_timings[iterations], unslice_timings[iterations];
	
	for (int i = 0; i < iterations; ++i)
		benchmark_forkskinny128_256_iteration(
				slice_timings + i,
				schedule_timings + i,
				encryption_timings + i,
				decryption_timings + i,
				unslice_timings + i
		);
	
	// sort the timings
	qsort(slice_timings, iterations, sizeof(ULL), compare);
	qsort(schedule_timings, iterations, sizeof(ULL), compare);
	qsort(encryption_timings, iterations, sizeof(ULL), compare);
	qsort(decryption_timings, iterations, sizeof(ULL), compare);
	qsort(unslice_timings, iterations, sizeof(ULL), compare);
	
	// the encryption samples use the same keys and plaintext and so are deterministic. Any noise that slows it down
	// comes from side channels, so we can just assume the fastest one
	double cycles_slicing_per_pack = slice_timings[0];
	double cycles_schedule_per_pack = schedule_timings[0];
	double cycles_encryption_per_pack = encryption_timings[0];
	double cycles_decryption_per_pack = decryption_timings[0];
	double cycles_unslicing_per_pack = unslice_timings[0];
	
	double total_per_block =
			(cycles_unslicing_per_pack + cycles_slicing_per_pack + cycles_encryption_per_pack +
			 cycles_schedule_per_pack) / slice_size;
	double cycles_per_round = total_per_block / rounds;
	double cycles_per_byte = total_per_block / 16;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double decryption_per_primitive = cycles_decryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single block\n";
	std::cout << encryption_per_primitive
	          << " cycles on encryption alone per single block (slicing excluded)\n";
	std::cout << decryption_per_primitive
	          << " cycles on decryption alone per single block (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE\n";
	std::cout << "-->" << cycles_per_byte << " cycles per byte (without decryption)\n";
	std::cout << (cycles_per_byte / rounds) * 36 << " cycles per byte per 36 rounds (encryption)\n";
	std::cout << cycles_per_round << " cycles per round (encryption)";
}

static inline void benchmark_forkskinny128_384() {
	std::cout << "\nFORKSKINNY128-384 (s=0)\n";
	auto iterations = 5000;
	auto rounds_before = FORKSKINNY_128_384_ROUNDS_BEFORE;
	auto rounds_after = FORKSKINNY_128_384_ROUNDS_AFTER;
	auto rounds = rounds_before + rounds_after;
	
	std::cout << std::dec << slice_size << " blocks in parallel\n";
	std::cout << rounds << " rounds per primitive call\n--------\n";
	
	ULL slice_timings[iterations], schedule_timings[iterations], encryption_timings[iterations],
			decryption_timings[iterations], unslice_timings[iterations];
	
	for (int i = 0; i < iterations; ++i)
		benchmark_forkskinny128_384_iteration(
				slice_timings + i,
				schedule_timings + i,
				encryption_timings + i,
				decryption_timings + i,
				unslice_timings + i
		);
	
	// sort the timings
	qsort(slice_timings, iterations, sizeof(ULL), compare);
	qsort(schedule_timings, iterations, sizeof(ULL), compare);
	qsort(encryption_timings, iterations, sizeof(ULL), compare);
	qsort(decryption_timings, iterations, sizeof(ULL), compare);
	qsort(unslice_timings, iterations, sizeof(ULL), compare);
	
	// the encryption samples use the same keys and plaintext and so are deterministic. Any noise that slows it down
	// comes from side channels, so we can just assume the fastest one
	double cycles_slicing_per_pack = slice_timings[0];
	double cycles_schedule_per_pack = schedule_timings[0];
	double cycles_encryption_per_pack = encryption_timings[0];
	double cycles_decryption_per_pack = decryption_timings[0];
	double cycles_unslicing_per_pack = unslice_timings[0];
	
	double total_per_block =
			(cycles_unslicing_per_pack + cycles_slicing_per_pack + cycles_encryption_per_pack +
			 cycles_schedule_per_pack) / slice_size;
	double cycles_per_round = total_per_block / rounds;
	double cycles_per_byte = total_per_block / 16;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double decryption_per_primitive = cycles_decryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single block\n";
	std::cout << encryption_per_primitive
	          << " cycles on encryption alone per single block (slicing excluded)\n";
	std::cout << decryption_per_primitive
	          << " cycles on decryption alone per single block (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE\n";
	std::cout << "-->" << cycles_per_byte << " cycles per byte (without decryption)\n";
	std::cout << (cycles_per_byte / rounds) * 36 << " cycles per byte per 36 rounds (encryption)\n";
	std::cout << cycles_per_round << " cycles per round (encryption)";
}

