#include <cassert>
#include <iostream>
#include "utils/forkskinny128-datatypes.h"
#include "utils/slicing128-accelerated.h"
#include "utils/slicing128-internal.h"
#include "../forkskinny64-plus/utils/slicing64-internal.h"
#include "../test_vectors.h"
#include "utils/slicing128.h"
#include "keyschedule/keyschedule128.h"

void test() {
	
	Blocks128_t b;
	for (int i = 0; i < slice_size; i += 4) {
		b.values[i].raw[0] =     0xAAAAAAAAAAAAAAA0;
		b.values[i + 1].raw[0] = 0xBBBBBBBBBBBBBBB0;
		b.values[i + 2].raw[0] = 0xCCCCCCCCCCCCCCC0;
		b.values[i + 3].raw[0] = 0xDDDDDDDDDDDDDDD0;
		// ----------------------------------------
		b.values[i].raw[1] =     0xAAAAAAAAAAAAAAA1;
		b.values[i + 1].raw[1] = 0xBBBBBBBBBBBBBBB1;
		b.values[i + 2].raw[1] = 0xCCCCCCCCCCCCCCC1;
		b.values[i + 3].raw[1] = 0xDDDDDDDDDDDDDDD1;
	}

	b = M_rand_128(21);
	auto res = slice_internal(&b);
	auto res1 = slice_accelerated_internal(&b);
	
	auto unsliced = unslice_internal(&res);
	auto unsliced1 = unslice_accelerated_internal(&res1);
	
	for (int i = 0; i < 128; ++i)
		assert(res.raw[i].value == res1.raw[i].value);

	for (int i = 0; i < slice_size; ++i) {
		assert(unsliced1.values[i].raw[0] == unsliced.values[i].raw[0]);
		assert(unsliced1.values[i].raw[1] == unsliced.values[i].raw[1]);
	}
}
//
//void test_forkskinny_128_256() {
//	std::cout << "\nforkskinny128-256\n";
//	auto M = slice_internal(M_128());
//	auto TK1 = slice_internal(TK1_128());
//	auto TK2 = slice_internal(TK2_128());
//	auto original_pt = unslice_accelerated(M);
//
//	auto schedule = forkskinny_128_init_tk2_internal(TK1, TK2);
//
//	// Ensure correct test vectors
//	auto ct = forkskinny128_encrypt(&schedule, &M, 'b', FORKSKINNY_128_256_ROUNDS_BEFORE,
//	                                FORKSKINNY_128_256_ROUNDS_AFTER);
//	auto result_c0 = unslice_accelerated(ct.C0);
//	auto result_c1 = unslice_accelerated(ct.C1);
//
//	std::cout << "\nM [0]: ";
//	print_block(original_pt.values[0].bytes, 16);
//
//	std::cout << "\nC0 [0]: ";
//	print_block(result_c0.values[0].bytes, 16);
//
//	std::cout << "\nC1 [0]: ";
//	print_block(result_c1.values[0].bytes, 16);
//
//	// verify test vectors
//	for (int i = 0; i < slice_size; ++i) {
//		assert(result_c0.values[i].raw[0] == 0x7701F1FC783BF8DC);
//		assert(result_c0.values[i].raw[1] == 0x2DB6D34C76C0414D);
//		assert(result_c1.values[i].raw[0] == 0x3E60F9E2B65E49CB);
//		assert(result_c1.values[i].raw[1] == 0xD5CDDFBC9440EE51);
//	}
//
//	auto pt = forkskinny128_decrypt(&schedule, &ct, '1', 'b', FORKSKINNY_128_256_ROUNDS_BEFORE,
//	                                FORKSKINNY_128_256_ROUNDS_AFTER);
//	auto result_M = unslice_accelerated(pt.M);
//	auto result_C0 = unslice_accelerated(pt.C0);
//
//	// test C1 -> (C0, M)
//	for (int i = 0; i < slice_size; ++i) {
//		assert(result_M.values[i].raw[0] == 0xEC4AFF517369C667);
//		assert(result_M.values[i].raw[1] == 0x80);
//		assert(result_C0.values[i].raw[0] == 0x7701F1FC783BF8DC);
//		assert(result_C0.values[i].raw[1] == 0x2DB6D34C76C0414D);
//	}
//
//}
//
void test_forkskinny_128_384(int keysize, uint64_t test_C0[2], uint64_t test_C1[2]) {
	std::cout << "\nFORKSKINNY128-" << keysize << " (s=b)" << std::endl;

	/// PT Blocks
	auto uM = M_128();
	auto M = slice128(&uM);
	auto uTK1 = TK1_128();
	auto TK1 = slice128(&uTK1);
	auto uTK2 = TK2_128();
	auto TK2 = slice128(&uTK2);
	auto uTK3 = TK3_128();
	auto TK3 = slice128(&uTK3);
	auto original_pt = unslice128(&M);
	
	/// Key schedule
	auto schedule = KeySchedule128Sliced_t();
	if (keysize == 256) forkskinny128_precompute_key_schedule(&TK1, &TK2, &schedule);
	else if (keysize == 384) forkskinny128_precompute_key_schedule(&TK1, &TK2, &TK3, &schedule);
	
	// @formatter:off
	/// Round keys (fs128-384, AddConstant inside ks)
	auto rtk0 = unslice128({.halves = {schedule.keys[0], {}}}).values[0].raw[0]; // 0x 9AC9 9F33 632C 5A77
	auto rtk1 = unslice128({.halves = {schedule.keys[1], {}}}).values[0].raw[0]; // 0x 6AF5 95E4 AC0D 4945
	auto rtk2 = unslice128({.halves = {schedule.keys[2], {}}}).values[0].raw[0]; // 0x B96C 01BA B952 D018
	auto rtk3 = unslice128({.halves = {schedule.keys[3], {}}}).values[0].raw[0]; // 0x 7235 1AB8 DC7B DB65
	auto rtk4 = unslice128({.halves = {schedule.keys[4], {}}}).values[0].raw[0]; // 0x BA2E 41B8 FC14 4EF3
	auto rtk5 = unslice128({.halves = {schedule.keys[5], {}}}).values[0].raw[0]; // 0x 7F64 17EE 77F6 D4B3
	auto rtk6 = unslice128({.halves = {schedule.keys[6], {}}}).values[0].raw[0]; // 0x B039 D9C2 D3FB 1B0F
	auto rtk7 = unslice128({.halves = {schedule.keys[7], {}}}).values[0].raw[0]; // 0x 75B1 84A2 92F3 A118
	// @formatter:on
	
	int appel = 1;


//	// test C1 -> (C0, M)
//	for (int i = 0; i < slice_size; ++i) {
//		assert(result_M.values[i].raw[0] == 0xEC4AFF517369C667);
//		assert(result_M.values[i].raw[1] == 0x80);
//		assert(result_C0.values[i].raw[0] == 0x72D8874177DC8C16);
//		assert(result_C0.values[i].raw[1] == 0x266DA48EA11FD273);
//	}
}


int main() {
//	test_forkskinny_128_256();
	test_forkskinny_128_384(384, nullptr, nullptr);
//	test();
}