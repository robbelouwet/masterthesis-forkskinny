#include <iostream>
#include <x86intrin.h>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../AEAD/PAEF.h"
#include "forkskinny64-benchmark-iteration.h"
#include <benchmark/benchmark.h>

static inline void benchmark_PAEF_forkskinny64_192() {
	auto bytes = slice_size * 8;
	std::cout << "PAEF - forkskinny64-192, " << std::dec << bytes << " bytes AD + " << bytes << " bytes M" << std::endl;
	std::cout << slice_size << " blocks in parallel" << std::endl;
	auto iterations = 5000;
	
	u64 tag;
	
	ULL m_slice_timings[iterations], ad_slice_timings[iterations], m_encrypt_timings[iterations],
			ad_encrypt_timings[iterations];
	for (int i = 0; i < iterations; ++i) {
		PAEF_forkskinny64_192(
				ad_slice_timings + i,
				ad_encrypt_timings + i,
				m_slice_timings + i,
				m_encrypt_timings + i,
				&tag);
	}
	
	// sort the timings
	qsort(m_slice_timings, iterations, sizeof(ULL), compare);
	qsort(m_encrypt_timings, iterations, sizeof(ULL), compare);
	qsort(ad_slice_timings, iterations, sizeof(ULL), compare);
	qsort(ad_encrypt_timings, iterations, sizeof(ULL), compare);
	
	// the encryption samples use the same keys and plaintext and so are deterministic. Any noise that slows it down
	// comes from side channels, so we can just assume the fastest one
	double cycles_M_block_slice = m_slice_timings[0] / (slice_size * 8);
	double cycles_M_block_encrypt = m_encrypt_timings[0] / (slice_size * 8);
	double cycles_AD_block_slice = ad_slice_timings[0] / (slice_size * 8);
	double cycles_AD_block_encrypt = ad_encrypt_timings[0] / (slice_size * 8);
	
	std::cout << cycles_M_block_slice + cycles_M_block_encrypt << " cpb for M (" << cycles_M_block_slice
	          << " cpb for slicing, " << cycles_M_block_encrypt << " cpb for encryption)" << std::endl;
	std::cout << cycles_AD_block_slice + cycles_AD_block_encrypt << " cpb for AD (" << cycles_AD_block_slice
	          << " cpb for slicing, " << cycles_AD_block_encrypt << " cpb for encryption)" << std::endl;
	std::cout << "Tag: " << std::hex << tag << std::endl;
	std::cout << "++++++++" << std::endl;
}

static inline void benchmark_forkskinny64_128() {
	std::cout << "\nFORKSKINNY64-128 (s=0)\n";
	auto iterations = 5000;
	auto rounds_before = FORKSKINNY_ROUNDS_BEFORE;
	auto rounds_after = FORKSKINNY_ROUNDS_AFTER;
	auto rounds = rounds_before + rounds_after;
	auto slicing_label = FAST_SLICING ? "Fast slicing" : "Naive slicing";
	auto ks_label = IMPROVED_KEYSCHEDULE ? "Improved KS" : "Original ks";
	
	std::cout << std::dec << slice_size << " blocks in parallel\n";
	std::cout << rounds << " rounds per primitive call\n--------\n";
	
	ULL slice_timings[iterations], schedule_timings[iterations], encryption_timings[iterations],
			decryption_timings[iterations], unslice_timings[iterations];
	for (int i = 0; i < iterations; ++i)
		benchmark_forkskinny64_128_iteration(
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
	double cycles_per_byte = total_per_block / 8;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double decryption_per_primitive = cycles_decryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single block (" << slicing_label << ")\n";
	std::cout << encryption_per_primitive
	          << " cycles on encryption alone per single block (slicing excluded)\n";
	std::cout << decryption_per_primitive
	          << " cycles on decryption alone per single block (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE (" << ks_label << ")\n";
	std::cout << "-->" << cycles_per_byte << " cycles per byte (without decryption)\n";
	std::cout << (cycles_per_byte / rounds) * 36 << " cycles per byte per 36 rounds\n";
	std::cout << cycles_per_round << " cycles per round";
}

static inline void benchmark_forkskinny64_192() {
	std::cout << "\nFORKSKINNY64-192 (s=0)\n";
	auto iterations = 5000;
	auto rounds_before = FORKSKINNY_ROUNDS_BEFORE;
	auto rounds_after = FORKSKINNY_ROUNDS_AFTER;
	auto rounds = rounds_before + rounds_after;
	auto slicing_label = FAST_SLICING ? "Fast slicing" : "Naive slicing";
	auto ks_label = IMPROVED_KEYSCHEDULE ? "Improved KS" : "Original ks";
	
	std::cout << std::dec << slice_size << " blocks in parallel\n";
	std::cout << rounds << " rounds per primitive call\n--------\n";
	
	ULL slice_timings[iterations], schedule_timings[iterations], encryption_timings[iterations],
			decryption_timings[iterations], unslice_timings[iterations];
	for (int i = 0; i < iterations; ++i)
		benchmark_forkskinny64_192_iteration(
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
	double cycles_per_byte = total_per_block / 8;
	
	double slicing_per_primitive = (cycles_unslicing_per_pack + cycles_slicing_per_pack) / slice_size;
	double encryption_per_primitive = cycles_encryption_per_pack / slice_size;
	double decryption_per_primitive = cycles_decryption_per_pack / slice_size;
	double schedule_per_primitive = cycles_schedule_per_pack / slice_size;
	std::cout << slicing_per_primitive << " spent on slicing per single block (" << slicing_label << ")\n";
	std::cout << encryption_per_primitive
	          << " cycles on encryption alone per single block (slicing excluded)\n";
	std::cout << decryption_per_primitive
	          << " cycles on decryption alone per single block (slicing excluded)\n";
	std::cout << schedule_per_primitive << " cycles spent on key schedule alone PER PRIMITIVE (" << ks_label << ")\n";
	std::cout << "-->" << cycles_per_byte << " cycles per byte (without decryption)\n";
	std::cout << (cycles_per_byte / rounds) * 36 << " cycles per byte per 36 rounds (encryption)\n";
	std::cout << cycles_per_round << " cycles per round (encryption)";
}

//BENCHMARK(run_benchmark_fs64);
//BENCHMARK_MAIN();

