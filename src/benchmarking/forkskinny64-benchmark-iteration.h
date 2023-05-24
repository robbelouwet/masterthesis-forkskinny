#ifndef FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H
#define FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H

#include <iostream>
#include <x86intrin.h>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../AEAD/PAEF.h"
#include "../forkskinny64-plus/utils/slicing64.h"
#include <benchmark/benchmark.h>

static inline void benchmark_forkskinny64_192_sb(ULL *slice_timing, ULL *schedule_timing,
                                                 ULL *encryption_timing, ULL *decryption_timing,
                                                 ULL *unslice_timing) {
	auto unsliced_test_M = M_64();
	auto unsliced_test_TK1 = TK1_64();
	auto unsliced_test_TK2 = TK2_64();
	auto unsliced_test_TK3 = TK3_64();
	
	// --- SLICING ---
	State64Sliced_t test_M, test_TK1, test_TK2, test_TK3;
	auto before_slicing = _rdtsc();
	slice(&unsliced_test_M, &test_M);
	slice(&unsliced_test_TK1, &test_TK1);
	slice(&unsliced_test_TK2, &test_TK2);
	slice(&unsliced_test_TK3, &test_TK3);
	*slice_timing = _rdtsc() - before_slicing;
	
	// --- KEY SCHEDULE ---
	KeySchedule64Sliced_t schedule;
	auto schedule_before = _rdtsc();
	forkskinny64_192_precompute_key_schedule(&test_TK1, &test_TK2, &test_TK3, &schedule);
	*schedule_timing = _rdtsc() - schedule_before;
	
	// --- ENCRYPTION ---
	State64Sliced_t C0, C1;
	auto encryption_before = _rdtsc();
	forkskinny64_encrypt(&schedule, &test_M, '1', &C0, &C1);
	*encryption_timing = _rdtsc() - encryption_before;
	SlicedCiphertext64_t wrapped_c1 = {.C1 = C1};
	
	// --- DECRYPTION ---
	SlicedCiphertext64_t recovered_pt;
	auto decryption_before = _rdtsc();
	forkskinny64_decrypt(&schedule, &wrapped_c1, &recovered_pt, '1', 'i');
	*decryption_timing = _rdtsc() - decryption_before;
	auto unsliced_recovered_pt = unslice(&(recovered_pt.M));
	
	// --- UNSLICING ---
	Blocks64_t unsliced_C0, unsliced_C1;
	auto unslicing_before = _rdtsc();
	//unslice(&C0, &unsliced_C0);
	unslice(&C1, &unsliced_C1);
	*unslice_timing = _rdtsc() - unslicing_before;
	
	// Prevent dead-code elimination caused by optimizations!
	for (int i = 0; i < slice_size; ++i) {
		//assert(unsliced_C0.values[i].raw == 0x502A9310B9F164FF);
		assert(unsliced_C1.values[i].raw == 0x55520D27354ECF3);
		assert(unsliced_recovered_pt.values[i].raw == unsliced_test_M.values[i].raw);
	}
}

static inline void benchmark_forkskinny64_128_sb(ULL *slice_timing, ULL *schedule_timing,
                                                 ULL *encryption_timing, ULL *decryption_timing,
                                                 ULL *unslice_timing) {
	auto unsliced_test_M = M_64();
	auto unsliced_test_TK1 = TK1_64();
	auto unsliced_test_TK2 = TK2_64();
	
	// --- SLICING ---
	State64Sliced_t test_M, test_TK1, test_TK2;
	auto before_slicing = _rdtsc();
	slice(&unsliced_test_M, &test_M);
	slice(&unsliced_test_TK1, &test_TK1);
	slice(&unsliced_test_TK2, &test_TK2);
	*slice_timing = _rdtsc() - before_slicing;
	
	// --- KEY SCHEDULE ---
	KeySchedule64Sliced_t schedule;
	auto schedule_before = _rdtsc();
	forkskinny64_192_precompute_key_schedule(&test_TK1, &test_TK2, &schedule);
	*schedule_timing = _rdtsc() - schedule_before;
	
	// --- ENCRYPTION ---
	State64Sliced_t C0, C1;
	auto encryption_before = _rdtsc();
	forkskinny64_encrypt(&schedule, &test_M, '1', &C0, &C1);
	*encryption_timing = _rdtsc() - encryption_before;
	SlicedCiphertext64_t wrapped_c1 = {.C1 = C1};
	
	// --- DECRYPTION ---
	SlicedCiphertext64_t recovered_pt;
	auto decryption_before = _rdtsc();
	forkskinny64_decrypt(&schedule, &wrapped_c1, &recovered_pt, '1', 'i');
	*decryption_timing = _rdtsc() - decryption_before;
	auto unsliced_recovered_pt = unslice(&(recovered_pt.M));
	
	// --- UNSLICING ---
	Blocks64_t unsliced_C0, unsliced_C1;
	auto unslicing_before = _rdtsc();
	//unslice(&C0, &unsliced_C0);
	unslice(&C1, &unsliced_C1);
	*unslice_timing = _rdtsc() - unslicing_before;
	
	// Prevent dead-code elimination caused by optimizations!
	for (int i = 0; i < slice_size; ++i) {
		//assert(unsliced_C0.values[i].raw == 0x9674fd60578adac8);
		assert(unsliced_C1.values[i].raw == 0x6a66ddc835c86a94);
		assert(unsliced_recovered_pt.values[i].raw == unsliced_test_M.values[i].raw);
	}
}

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

#endif //FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H
