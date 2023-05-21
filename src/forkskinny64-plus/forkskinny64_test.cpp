//#include <cassert>
#include <iostream>
#include <cassert>
#include "utils/forkskinny64-datatypes.h"
#include "utils/slicing64-accelerated.h"
#include "../test_vectors.h"
#include "forkskinny64.h"
#include "keyschedule/fixsliced-keyschedule64.h"
#include "keyschedule/keyschedule64.h"
#include "utils/slicing64.h"

void test() {
	
	Blocks64_t b;
	for (int i = 0; i < slice_size; i += 4) {
		b.values[i].raw = 0xAAAAAAAAAAAAAAAA;
		b.values[i + 1].raw = 0xBBBBBBBBBBBBBBBB;
		b.values[i + 2].raw = 0xCCCCCCCCCCCCCCCC;
		b.values[i + 3].raw = 0xDDDDDDDDDDDDDDDD;
	}
	
	b = M_rand_64(36);
//	auto res = slice(b);
	auto res1 = slice_accelerated(&b);
	auto unsliced = unslice(res1);
	auto unsliced2 = unslice_accelerated(res1);
	int appel = 1;

//	for (int i = 0; i < slice_size; ++i)
//		assert(res.raw[i].value == res1.raw[i].value);

//	for (int i = 0; i < slice_size; ++i)
//		assert(unsliced2.values[i].raw == unsliced.values[i].raw);
}


void test_forkskinny64(int keysize, uint64_t test_C0, uint64_t test_C1) {
	std::cout << "\nFORKSKINNY64-" << keysize  << " (s=b)" << std::endl;
	auto uM = M_64();
	auto M = slice_accelerated(&uM);
	auto uTK1 = TK1_64();
	auto TK1 = slice_accelerated(&uTK1);
	auto uTK2 = TK2_64();
	auto TK2 = slice_accelerated(&uTK2);
	auto uTK3 = TK3_64();
	auto TK3 = slice_accelerated(&uTK3);
	auto original_pt = unslice_accelerated(M);
	
	auto schedule = KeySchedule64Sliced_t();
	if (keysize == 128) forkskinny64_192_precompute_key_schedule(&TK1, &TK2, &schedule);
	else if (keysize == 192) forkskinny64_192_precompute_key_schedule(&TK1, &TK2, &TK3, &schedule);
	
	auto rtk0 = unslice_accelerated({.halves = {schedule.keys[0], {}}}).values[0].raw; // 0x EE00 FDE0 (fs64-192, pre-computed AddConstant inside ks)
	auto rtk1 = unslice_accelerated({.halves = {schedule.keys[1], {}}}).values[0].raw; // 0x 099B 203B
	auto rtk2 = unslice_accelerated({.halves = {schedule.keys[2], {}}}).values[0].raw; // 0x 0EE2 40B2
	auto rtk3 = unslice_accelerated({.halves = {schedule.keys[3], {}}}).values[0].raw; // 0x 7000 2967
	auto rtk4 = unslice_accelerated({.halves = {schedule.keys[4], {}}}).values[0].raw; // 0x 4090 EE14
	auto rtk5 = unslice_accelerated({.halves = {schedule.keys[5], {}}}).values[0].raw; // 0x 732 50D0
	auto rtk6 = unslice_accelerated({.halves = {schedule.keys[6], {}}}).values[0].raw; // 0x 247F 6010
	auto rtk7 = unslice_accelerated({.halves = {schedule.keys[7], {}}}).values[0].raw; // 0x 0010 FDD6
	
	// Ensure correct test vectors
	State64Sliced_t C0;
	State64Sliced_t C1;
	forkskinny64_encrypt(&schedule, &M, 'b', &C0, &C1);
	SlicedCiphertext64_t ct = SlicedCiphertext64_t{.C1 = C1, .C0 = C0};
	
	auto result_c0 = unslice_accelerated(ct.C0);
	auto result_c1 = unslice_accelerated(ct.C1);
	
	std::cout << "\nM->C0 [0]: ";
	print_block(result_c0.values[0].bytes, 8);
	
	std::cout << "\nM->C1 [0]: ";
	print_block(result_c1.values[0].bytes, 8);
	
	for (int i = 0; i < slice_size; ++i) {
		assert(result_c0.values[i].raw == test_C0);
		assert(result_c1.values[i].raw == test_C1);
	}
	
	auto pt = forkskinny64_decrypt(&schedule, &ct, '1', 'b');
	auto result_M = unslice_accelerated(pt.M);
	auto result_C0 = unslice_accelerated(pt.C0);
	
	std::cout << "\n-----\nC1->M [0]: ";
	print_block(result_M.values[0].bytes, 8);
	
	std::cout << "\nC1->C0 [0]: ";
	print_block(result_C0.values[0].bytes, 8);
	
	for (int i = 0; i < slice_size; ++i) {
		assert(result_M.values[i].raw == original_pt.values[i].raw);
		assert(result_C0.values[i].raw == test_C0);
	}
	int appel = 1;
}

int main() {
	test_forkskinny64(128, 0x9674fd60578adac8, 0x6a66ddc835c86a94);
	std::cout << "\n---------------------------------\n";
	test_forkskinny64(192, 0x502A9310B9F164FF, 0x55520D27354ECF3);
//	test();
	std::cout << "\n\nSuccess!";
}
