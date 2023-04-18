#include <cassert>
#include <iostream>
#include "utils/forkskinny64-datatypes.h"
#include "utils/slicing64.h"
#include "forkskinny64.h"
#include "keyschedule/fixsliced-keyschedule64.h"
#include "keyschedule/keyschedule64.h"
#include "../test_vectors.h"

//bool test_pt64(){
//	auto original_pt = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
//	auto sliced_state = slice(original_pt);
//
//	auto s_output = State64Sliced_t();
//	PT64_4(sliced_state, s_output)
//	auto output = unslice(s_output).values[0].raw;
//
//
//	auto original_pt2 = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
//	auto sliced_state2 = slice(original_pt);
//	auto s_output2 = permute(permute(permute(permute(sliced_state2))));
//	auto output2 = unslice(s_output2).values[0].raw;
//
//	int appel = 1;
//
//}

void test_forkskinny64_192() {
	auto M = M_64();
	auto TK1 = TK1_64();
	auto TK2 = TK2_64();
	auto TK3 = TK3_64();
	auto original_pt = unslice(M).values[0].raw;
	
	auto schedule = forkskinny_64_init_tk23(TK1, TK2, TK3);
	
	// 0x EE00 FDE0
	// 0x 099B 203B
	// 0x 0EE2 40B2
	auto rtk0 = unslice({.halves = {schedule.keys[0], {}}}).values[0].raw;
	auto rtk1 = unslice({.halves = {schedule.keys[1], {}}}).values[0].raw;
	auto rtk2 = unslice({.halves = {schedule.keys[2], {}}}).values[0].raw;
	
	// Ensure correct test vectors
	auto ct = forkskinny64_encrypt(schedule, &M, 'b');
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	std::cout << "M: ";
	print_block(unslice(M).values[0].bytes, 8);
	
	std::cout << "\nC0: ";
	print_block(result_c0.bytes, 8);
	
	std::cout << "\nC1: ";
	print_block(result_c1.bytes, 8);
	
	
	assert(result_c0.raw == 0x502A9310B9F164FF);
	assert(result_c1.raw == 0x55520D27354ECF3);
	
	auto pt = forkskinny64_decrypt(schedule, &ct, '1', 'b');
	auto result_M = unslice(pt.M).values[0].raw;
	auto result_C0 = unslice(pt.C0).values[0].raw;
	
	assert(result_M == original_pt);
	assert(result_C0 == 0x502A9310B9F164FF);
}

int main() {
	test_forkskinny64_192();
	std::cout << "\n\nSuccess!";
}
