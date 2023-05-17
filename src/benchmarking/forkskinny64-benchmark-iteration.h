#ifndef FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H
#define FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H

#include <iostream>
#include <x86intrin.h>
#include "../config.h"
#include "../test_vectors.h"
#include "../forkskinny64-plus/keyschedule/fixsliced-keyschedule64.h"
#include "../forkskinny64-plus/forkskinny64.h"
#include "../AEAD/PAEF.h"
#include <benchmark/benchmark.h>

static inline void benchmark_forkskinny64_192_sb(ULL *slice_timing, ULL *schedule_timing,
                                                 ULL *encryption_timing, ULL *unslice_timing) {
	auto unsliced_test_M = M_64();
	auto unsliced_test_TK1 = TK1_64();
	auto unsliced_test_TK2 = TK2_64();
	auto unsliced_test_TK3 = TK3_64();
	
	// --- SLICING ---
	State64Sliced_t test_M, test_TK1, test_TK2, test_TK3;
	auto before_slicing = _rdtsc();
	slice_accelerated(&unsliced_test_M, &test_M);
	slice_accelerated(&unsliced_test_TK1, &test_TK1);
	slice_accelerated(&unsliced_test_TK2, &test_TK2);
	slice_accelerated(&unsliced_test_TK3, &test_TK3);
	*slice_timing = _rdtsc() - before_slicing;
	
	// --- KEY SCHEDULE ---
	KeySchedule64Sliced_t schedule;
	auto schedule_before = _rdtsc();
	forkskinny64_192_precompute_key_schedule(&test_TK1, &test_TK2, &test_TK3, &schedule);
	*schedule_timing = _rdtsc() - schedule_before;
	
	// --- ENCRYPTION ---
	State64Sliced_t C0, C1;
	auto encryption_before = _rdtsc();
	forkskinny64_encrypt(&schedule, &test_M, 'b', &C0, &C1);
	*encryption_timing = _rdtsc() - encryption_before;
	
	// --- UNSLICING ---
	Blocks64_t unsliced_C0, unsliced_C1;
	auto unslicing_before = _rdtsc();
	unslice_accelerated(&C0, &unsliced_C0);
	unslice_accelerated(&C1, &unsliced_C1);
	*unslice_timing = _rdtsc() - unslicing_before;
	
	// Prevent dead-code elimination caused by optimizations!
	for (int i = 0; i < slice_size; ++i) {
		assert(unsliced_C0.values[i].raw == 0x502A9310B9F164FF);
		assert(unsliced_C1.values[i].raw == 0x55520D27354ECF3);
	}
}

static inline void benchmark_forkskinny64_128_sb(ULL *slice_timing, ULL *schedule_timing,
                                                 ULL *encryption_timing, ULL *unslice_timing) {
	auto unsliced_test_M = M_64();
	auto unsliced_test_TK1 = TK1_64();
	auto unsliced_test_TK2 = TK2_64();
	
	// --- SLICING ---
	State64Sliced_t test_M, test_TK1, test_TK2;
	auto before_slicing = _rdtsc();
	slice_accelerated(&unsliced_test_M, &test_M);
	slice_accelerated(&unsliced_test_TK1, &test_TK1);
	slice_accelerated(&unsliced_test_TK2, &test_TK2);
	*slice_timing = _rdtsc() - before_slicing;
	
	// --- KEY SCHEDULE ---
	KeySchedule64Sliced_t schedule;
	auto schedule_before = _rdtsc();
	forkskinny64_192_precompute_key_schedule(&test_TK1, &test_TK2, &schedule);
	*schedule_timing = _rdtsc() - schedule_before;
	
	// --- ENCRYPTION ---
	State64Sliced_t C0, C1;
	auto encryption_before = _rdtsc();
	forkskinny64_encrypt(&schedule, &test_M, 'b', &C0, &C1);
	*encryption_timing = _rdtsc() - encryption_before;
	
	// --- UNSLICING ---
	Blocks64_t unsliced_C0, unsliced_C1;
	auto unslicing_before = _rdtsc();
	unslice_accelerated(&C0, &unsliced_C0);
	unslice_accelerated(&C1, &unsliced_C1);
	*unslice_timing = _rdtsc() - unslicing_before;
	
	// Prevent dead-code elimination caused by optimizations!
	for (int i = 0; i < slice_size; ++i) {
		assert(unsliced_C0.values[i].raw == 0x9674fd60578adac8);
		assert(unsliced_C1.values[i].raw == 0x6a66ddc835c86a94);
	}
}

#endif //FORKSKINNYPLUS_FORKSKINNY64_BENCHMARK_ITERATION_H
