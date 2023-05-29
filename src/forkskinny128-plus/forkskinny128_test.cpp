#include <cassert>
#include "utils/forkskinny128-datatypes.h"
#include "utils/slicing128_outsourced.h"
#include "utils/slicing128.h"
#include "../forkskinny64-plus/utils/slicing64-internal.h"

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

//	b = M_rand_64(36);
	auto res = slice_internal(b);
	State128Sliced_t res1;
	slice128_outsourced(&b, &res1, false);
	
	Blocks128_t unsliced;
	unslice128_outsourced(&res1, &unsliced, false);
	auto unsliced2 = unslice_accelerated(res1);
	
	for (int i = 0; i < slice_size; ++i)
		assert(res.raw[i].value == res1.raw[i].value);

	for (int i = 0; i < slice_size; ++i) {
		assert(unsliced2.values[i].raw[0] == unsliced.values[i].raw[0]);
		assert(unsliced2.values[i].raw[1] == unsliced.values[i].raw[1]);
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
//	auto schedule = forkskinny_128_keyschedule_tk2(TK1, TK2);
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
//void test_forkskinny_128_384() {
//	std::cout << "\n\nforkskinny128-384\n";
//	auto M = slice_internal(M_128());
//	auto TK1 = slice_internal(TK1_128());
//	auto TK2 = slice_internal(TK2_128());
//	auto TK3 = slice_internal(TK3_128());
//	auto original_pt = unslice_accelerated(M);
//
//	auto schedule = forkskinny_128_fixsliced_init_tk23(TK1, TK2, TK3);
//
//	// fixsliced,           vanilla,            Erik
//	// 0x9AC99F33632C5A7B,  0x9AC99F33632C5A77, 0x9AC99F33632C5A77
//	// 0xACE46A4946F70F9C,  0x6AF595E4AC0D4945, 0x6AF595E4AC0D4945
//	// 0xB9BAB9D01F6E5007,  0xB96C01BAB952D018, 0xB96C01BAB952D018
//	auto rtk0 = unslice_accelerated({.halves = {schedule.keys[0], {}}}).values[0].raw[0];
//	auto rtk1 = unslice_accelerated({.halves = {schedule.keys[1], {}}}).values[0].raw[0];
//	auto rtk2 = unslice_accelerated({.halves = {schedule.keys[2], {}}}).values[0].raw[0];
//
//	// Ensure correct test vectors
//	auto ct = forkskinny128_encrypt(&schedule, &M, 'b', FORKSKINNY_128_384_ROUNDS_BEFORE,
//	                                FORKSKINNY_128_384_ROUNDS_AFTER);
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
//		assert(result_c0.values[i].raw[0] == 0x72D8874177DC8C16);
//		assert(result_c0.values[i].raw[1] == 0x266DA48EA11FD273);
//		assert(result_c1.values[i].raw[0] == 0x4F318CE8A6A22F06);
//		assert(result_c1.values[i].raw[1] == 0x09E2DD8ECD1C6945);
//	}
//
//	auto pt = forkskinny128_decrypt(&schedule, &ct, '1', 'b', FORKSKINNY_128_384_ROUNDS_BEFORE,
//	                                FORKSKINNY_128_384_ROUNDS_AFTER);
//	auto result_M = unslice_accelerated(pt.M);
//	auto result_C0 = unslice_accelerated(pt.C0);
//
//	// test C1 -> (C0, M)
//	for (int i = 0; i < slice_size; ++i) {
//		assert(result_M.values[i].raw[0] == 0xEC4AFF517369C667);
//		assert(result_M.values[i].raw[1] == 0x80);
//		assert(result_C0.values[i].raw[0] == 0x72D8874177DC8C16);
//		assert(result_C0.values[i].raw[1] == 0x266DA48EA11FD273);
//	}
//}
//

int main() {
//	test_forkskinny_128_256();
//	test_forkskinny_128_384();
	test();
}