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

void PAEF_forkskinny64_192(double *timing_AD_slice, double *timing_AD_encrypt,
                           double *timing_M_slice, double *timing_M_encrypt, u64 *tag) {
	/// Nonce N
	int nonce_bit_length = 96;
	Block64_t nonce[3] = {{.raw = 0xFEDCBA9876543210},
	                      {.raw = 0xAAAAAAAA}};
	
	/// 1 sliced AD state
	auto AD = M_rand_64(1);
	
	/// Sample 1 sliced AD state
	u64 ad_tag;
	SlicedCiphertext64_t temp_AD; // we don't need the ciphertext, just the tag generated from it
	paef_forkskinny64_192_encrypt_AD(&AD, 1, nonce, nonce_bit_length,
	                                 &temp_AD, &ad_tag, timing_AD_slice, timing_AD_encrypt);
	
	/// 1 sliced M state
	auto M_state = M_rand_64(2);
	
	/// Sample 1 sliced M state
	u64 m_tag;
	SlicedCiphertext64_t temp_M;
	paef_forkskinny64_192_encrypt_M(&M_state, 1, nonce, nonce_bit_length,
	                                &temp_M, &m_tag, timing_M_slice, timing_M_encrypt);
	*tag = m_tag ^ ad_tag;
}

#endif //FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H
