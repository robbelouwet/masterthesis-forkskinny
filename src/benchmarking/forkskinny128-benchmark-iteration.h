#ifndef FORKSKINNYPLUS_FORKSKINNY128_BENCHMARK_ITERATION_H
#define FORKSKINNYPLUS_FORKSKINNY128_BENCHMARK_ITERATION_H

#include <cassert>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny128-plus/utils/slicing128.h"
#include "../forkskinny128-plus/keyschedule/keyschedule128.h"
#include "../forkskinny128-plus/forkskinny128.h"

static inline void benchmark_forkskinny128_384_iteration(ULL *slice_timing, ULL *schedule_timing,
                                                         ULL *encryption_timing, ULL *decryption_timing,
                                                         ULL *unslice_timing) {
	auto unsliced_test_M = M_128();
	auto unsliced_test_TK1 = TK1_128();
	auto unsliced_test_TK2 = TK2_128();
	auto unsliced_test_TK3 = TK3_128();
	
	// --- SLICING ---
	State128Sliced_t test_M, test_TK1, test_TK2, test_TK3;
	auto volatile before_slicing = _rdtsc();
	slice128(&unsliced_test_M, &test_M);
	slice128(&unsliced_test_TK1, &test_TK1);
	slice128(&unsliced_test_TK2, &test_TK2);
	slice128(&unsliced_test_TK3, &test_TK3);
	*slice_timing = _rdtsc() - before_slicing;
	
	// --- KEY SCHEDULE ---
	KeySchedule128Sliced_t schedule;
	auto volatile schedule_before = _rdtsc();
	forkskinny128_precompute_key_schedule(&test_TK1, &test_TK2, &test_TK3,
	                                      FORKSKINNY_128_384_ROUNDS_BEFORE + FORKSKINNY_128_384_ROUNDS_AFTER,
	                                      &schedule);
	*schedule_timing = _rdtsc() - schedule_before;
	
	// --- ENCRYPTION ---
	State128Sliced_t C0, C1;
	auto volatile encryption_before = _rdtsc();
	forkskinny128_encrypt(&schedule, &test_M, &C0, nullptr, '0',
	                      FORKSKINNY_128_384_ROUNDS_BEFORE, FORKSKINNY_128_384_ROUNDS_AFTER);
	*encryption_timing = _rdtsc() - encryption_before;
	SlicedCiphertext128_t wrapped_c0 = {.C0 = C0};
	
	// --- DECRYPTION ---
	SlicedCiphertext128_t recovered_pt;
	auto volatile decryption_before = _rdtsc();
	forkskinny128_decrypt(&schedule, &wrapped_c0, &recovered_pt, '0', 'i',
	                      FORKSKINNY_128_384_ROUNDS_BEFORE, FORKSKINNY_128_384_ROUNDS_AFTER);
	*decryption_timing = _rdtsc() - decryption_before;
	auto unsliced_recovered_pt = unslice128(&(recovered_pt.M));
	
	// --- UNSLICING ---
	Blocks128_t unsliced_C0, unsliced_C1;
	auto volatile unslicing_before = _rdtsc();
	unslice128(&C0, &unsliced_C0);
//	unslice128(&C1, &unsliced_C1);
	*unslice_timing = _rdtsc() - unslicing_before;
	
	// Prevent dead-code elimination caused by optimizations!
	for (int i = 0; i < slice_size; ++i) {
		assert(unsliced_C0.values[i].raw[0] == 0x72D8874177DC8C16);
		assert(unsliced_C0.values[i].raw[1] == 0x266DA48EA11FD273);
//		assert(unsliced_C1.values[i].raw[0] == 0x4f318ce8a6a22f06);
//		assert(unsliced_C1.values[i].raw[1] == 0x09e2dd8ecd1c6945);
		assert(unsliced_recovered_pt.values[i].raw[0] == unsliced_test_M.values[i].raw[0]);
		assert(unsliced_recovered_pt.values[i].raw[1] == unsliced_test_M.values[i].raw[1]);
	}
}

static inline void benchmark_forkskinny128_256_iteration(ULL *slice_timing, ULL *schedule_timing,
                                                         ULL *encryption_timing, ULL *decryption_timing,
                                                         ULL *unslice_timing) {
	auto unsliced_test_M = M_128();
	auto unsliced_test_TK1 = TK1_128();
	auto unsliced_test_TK2 = TK2_128();
	
	// --- SLICING ---
	State128Sliced_t test_M, test_TK1, test_TK2, test_TK3;
	auto volatile before_slicing = _rdtsc();
	slice128(&unsliced_test_M, &test_M);
	slice128(&unsliced_test_TK1, &test_TK1);
	slice128(&unsliced_test_TK2, &test_TK2);
	*slice_timing = _rdtsc() - before_slicing;
	
	// --- KEY SCHEDULE ---
	KeySchedule128Sliced_t schedule;
	auto volatile schedule_before = _rdtsc();
	forkskinny128_precompute_key_schedule(&test_TK1, &test_TK2,
	                                      FORKSKINNY_128_256_ROUNDS_BEFORE + FORKSKINNY_128_256_ROUNDS_AFTER,
	                                      &schedule);
	*schedule_timing = _rdtsc() - schedule_before;
	
	// --- ENCRYPTION ---
	State128Sliced_t C0, C1;
	auto volatile encryption_before = _rdtsc();
	forkskinny128_encrypt(&schedule, &test_M, &C0, nullptr, '0',
	                      FORKSKINNY_128_256_ROUNDS_BEFORE, FORKSKINNY_128_256_ROUNDS_AFTER);
	*encryption_timing = _rdtsc() - encryption_before;
	SlicedCiphertext128_t wrapped_c0 = {.C0 = C0};
	
	// --- DECRYPTION ---
	SlicedCiphertext128_t recovered_pt;
	auto volatile decryption_before = _rdtsc();
	forkskinny128_decrypt(&schedule, &wrapped_c0, &recovered_pt, '0', 'i',
	                      FORKSKINNY_128_256_ROUNDS_BEFORE, FORKSKINNY_128_256_ROUNDS_AFTER);
	*decryption_timing = _rdtsc() - decryption_before;
	auto unsliced_recovered_pt = unslice128(&(recovered_pt.M));
	
	// --- UNSLICING ---
	Blocks128_t unsliced_C0, unsliced_C1;
	auto volatile unslicing_before = _rdtsc();
	unslice128(&C0, &unsliced_C0);
//	unslice128(&C1, &unsliced_C1);
	*unslice_timing = _rdtsc() - unslicing_before;
	
	// Prevent dead-code elimination caused by optimizations!
	for (int i = 0; i < slice_size; ++i) {
		assert(unsliced_C0.values[i].raw[0] == 0x7701f1fc783bf8dc);
		assert(unsliced_C0.values[i].raw[1] == 0x2db6d34c76c0414d);
//		assert(unsliced_C1.values[i].raw[0] == 0x3e60f9e2b65e49cb);
//		assert(unsliced_C1.values[i].raw[1] == 0xd5cddfbc9440ee51);
		assert(unsliced_recovered_pt.values[i].raw[0] == unsliced_test_M.values[i].raw[0]);
		assert(unsliced_recovered_pt.values[i].raw[1] == unsliced_test_M.values[i].raw[1]);
	}
}

#endif //FORKSKINNYPLUS_FORKSKINNY128_BENCHMARK_ITERATION_H
