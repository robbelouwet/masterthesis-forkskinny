#include <cassert>
#include <iostream>
#include "utils/forkskinny128-datatypes.h"
#include "utils/slicing128.h"
#include "forkskinny128.h"
#include "keyschedule/keyschedule128.h"
#include "keyschedule/fixsliced-keyschedule128.h"
#include "../test_vectors.h"

//bool test_pt128(){
//	auto original_pt = Blocks128_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
//	auto sliced_state = slice(original_pt);
//
//	auto s_output = State128Sliced_t();
//	PT128_4(sliced_state, s_output)
//	auto output = unslice(s_output).values[0].raw[0];
//
//
//	auto original_pt2 = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
//	auto sliced_state2 = slice(original_pt);
//	auto s_output2 = permute(permute(permute(permute(sliced_state2))));
//	auto output2 = unslice(s_output2).values[0].raw[0];
//
//	int appel = 1;
//
//}

void test_forkskinny_128_256() {
	std::cout << "\nforkskinny128-256\n";
	auto M = slice(M_128());
	auto TK1 = slice(TK1_128());
	auto TK2 = slice(TK2_128());
	auto original_pt = unslice(M);
	
	auto schedule = forkskinny_128_keyschedule_tk2(TK1, TK2);
	
	// Ensure correct test vectors
	auto ct = forkskinny128_encrypt(&schedule, &M, 'b', FORKSKINNY_128_256_ROUNDS_BEFORE,
	                                FORKSKINNY_128_256_ROUNDS_AFTER);
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	std::cout << "\nM: ";
	print_block(original_pt.values[0].bytes, 16);
	
	std::cout << "\nC0: ";
	print_block(result_c0.bytes, 16);
	
	std::cout << "\nC1: ";
	print_block(result_c1.bytes, 16);
	
	// verify test vectors
	assert(result_c0.raw[0] == 0x7701F1FC783BF8DC);
	assert(result_c0.raw[1] == 0x2DB6D34C76C0414D);
	assert(result_c1.raw[0] == 0x3E60F9E2B65E49CB);
	assert(result_c1.raw[1] == 0xD5CDDFBC9440EE51);
	
	auto pt = forkskinny128_decrypt(&schedule, &ct, '1', 'b', FORKSKINNY_128_256_ROUNDS_BEFORE,
	                                FORKSKINNY_128_256_ROUNDS_AFTER);
	auto result_M = unslice(pt.M).values[0];
	auto result_C0 = unslice(pt.C0).values[0];
	
	// test C1 -> (C0, M)
	assert(result_M.raw[0] == 0xEC4AFF517369C667);
	assert(result_M.raw[1] == 0x80);
	assert(result_C0.raw[0] == 0x7701F1FC783BF8DC);
	assert(result_C0.raw[1] == 0x2DB6D34C76C0414D);
	
}

void test_forkskinny_128_384() {
	std::cout << "\n\nforkskinny128-384\n";
	auto M = slice(M_128());
	auto TK1 = slice(TK1_128());
	auto TK2 = slice(TK2_128());
	auto TK3 = slice(TK3_128());
	auto original_pt = unslice(M);
	
	auto schedule = forkskinny_128_fixsliced_init_tk23(TK1, TK2, TK3);
	
	// fixsliced,           vanilla,            Erik
	// 0x9AC99F33632C5A7B,  0x9AC99F33632C5A77, 0x9AC99F33632C5A77
	// 0xACE46A4946F70F9C,  0x6AF595E4AC0D4945, 0x6AF595E4AC0D4945
	// 0xB9BAB9D01F6E5007,  0xB96C01BAB952D018, 0xB96C01BAB952D018
//	auto rtk0 = unslice({.halves = {schedule.keys[0], {}}}).values[0].raw[0];
//	auto rtk1 = unslice({.halves = {schedule.keys[1], {}}}).values[0].raw[0];
//	auto rtk2 = unslice({.halves = {schedule.keys[2], {}}}).values[0].raw[0];
	
	// Ensure correct test vectors
	auto ct = forkskinny128_encrypt(&schedule, &M, 'b', FORKSKINNY_128_384_ROUNDS_BEFORE,
	                                FORKSKINNY_128_384_ROUNDS_AFTER);
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	std::cout << "\nM: ";
	print_block(original_pt.values[0].bytes, 16);
	
	std::cout << "\nC0: ";
	print_block(result_c0.bytes, 16);
	
	std::cout << "\nC1: ";
	print_block(result_c1.bytes, 16);
	
	// verify test vectors
	assert(result_c0.raw[0] == 0x72D8874177DC8C16);
	assert(result_c0.raw[1] == 0x266DA48EA11FD273);
	assert(result_c1.raw[0] == 0x4F318CE8A6A22F06);
	assert(result_c1.raw[1] == 0x09E2DD8ECD1C6945);

	auto pt = forkskinny128_decrypt(&schedule, &ct, '1', 'b', FORKSKINNY_128_384_ROUNDS_BEFORE,
	                                FORKSKINNY_128_384_ROUNDS_AFTER);
	auto result_M = unslice(pt.M).values[0];
	auto result_C0 = unslice(pt.C0).values[0];

	// test C1 -> (C0, M)
	assert(result_M.raw[0] == 0xEC4AFF517369C667);
	assert(result_M.raw[1] == 0x80);
	assert(result_C0.raw[0] == 0x72D8874177DC8C16);
	assert(result_C0.raw[1] == 0x266DA48EA11FD273);
}


int main() {
//	test_forkskinny_128_256();
	test_forkskinny_128_384();
//	test_pt128();
}