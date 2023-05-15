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
//#include "keyschedule/fixsliced-keyschedule64.h"

//bool test_pt64(){
//	auto original_pt = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
//	auto sliced_state = slice(original_pt);
//
//	auto s_output = State64Sliced_t();
//	PT64_4(sliced_state, s_output)
//	auto output = unslice_accelerated(s_output).values[0].raw;
//
//
//	auto original_pt2 = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
//	auto sliced_state2 = slice(original_pt);
//	auto s_output2 = permute(permute(permute(permute(sliced_state2))));
//	auto output2 = unslice_accelerated(s_output2).values[0].raw;
//
//	int appel = 1;
//
//}

void test(){
	
	Blocks64_t b = {.values = {
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,

			/*// 64
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			0xAAAAAAAAAAAAAAAA,0xBBBBBBBBBBBBBBBB,0xCCCCCCCCCCCCCCCC,0xDDDDDDDDDDDDDDDD,
			//128
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,
			0x6666666666666666,0xBBBBBBBBBBBBBBBB,0x9999999999999999,0x5555555555555555,*/
	}};
	
//	b = M_rand_64(9);
	//auto res = slice(b);
	auto res1 = slice_accelerated(&b, true);
	auto unsliced = unslice_accelerated(res1, true);
	int appel = 1;
	
//	for (int i = 0; i < slice_size; ++i)
//		assert(res.raw[i].value == res1.raw[i].value);
	
	for (int i = 0; i < slice_size; ++i)
		assert(b.values[i].raw == unsliced.values[i].raw);
}


void test_forkskinny64_192() {
	auto uM = M_64();
	auto M = slice_accelerated(&uM);
	auto uTK1 = TK1_64();
	auto TK1 = slice_accelerated(&uTK1);
	auto uTK2 = TK2_64();
	auto TK2 = slice_accelerated(&uTK2);
	auto uTK3 = TK3_64();
	auto TK3 = slice_accelerated(&uTK3);
	auto original_pt = unslice_accelerated(M);
//
//	auto schedule = KeySchedule64Sliced_t();
//	auto schedule2 = KeySchedule64Sliced_t(); auto fTK1 = TK1; auto fTK2 = TK2; auto fTK3 = TK3;
//	forkskinny_64_init_tk23_internal(&TK1, &TK2, &TK3, &schedule);
//
//	forkskinny_64_init_tk23_fixsliced_internal(&fTK1, &fTK2, &fTK3, &schedule2);
//
//	for (int i = 0; i < FORKSKINNY64_MAX_ROUNDS; ++i) {
//		auto a = unslice_accelerated({.halves = {schedule.keys[i], {}}}).values[0].raw;
//		auto b = unslice_accelerated({.halves = {schedule2.keys[i], {}}}).values[0].raw;
//		std::cout << std::hex << a << std::endl;
//		assert(a == b);
//	}

	auto schedule = KeySchedule64Sliced_t();
	forkskinny64_192_precompute_key_schedule(&TK1, &TK2, &TK3, &schedule);

	auto rtk0 = unslice_accelerated({ .halves = {schedule.keys[0], {}}}).values[0].raw; // 0x EE00 FDE0
	auto rtk1 = unslice_accelerated({ .halves = {schedule.keys[1], {}}}).values[0].raw; // 0x 099B 203B
	auto rtk2 = unslice_accelerated({ .halves = {schedule.keys[2], {}}}).values[0].raw; // 0x 0EE2 40B2
	auto rtk3 = unslice_accelerated({ .halves = {schedule.keys[3], {}}}).values[0].raw; // 0x 7000 2967
	auto rtk4 = unslice_accelerated({ .halves = {schedule.keys[4], {}}}).values[0].raw; // 0x 4090 EE14
	auto rtk5 = unslice_accelerated({ .halves = {schedule.keys[5], {}}}).values[0].raw; // 0x 732 50D0
	auto rtk6 = unslice_accelerated({ .halves = {schedule.keys[6], {}}}).values[0].raw; // 0x 247F 6010
	auto rtk7 = unslice_accelerated({ .halves = {schedule.keys[7], {}}}).values[0].raw; // 0x 0010 FDD6

	// Ensure correct test vectors
	auto ct = forkskinny64_encrypt(&schedule, &M, 'b');
	auto result_c0 = unslice_accelerated(ct.C0);
	auto result_c1 = unslice_accelerated(ct.C1);

	std::cout << "\nM->C0 [0]: ";
	print_block(result_c0.values[0].bytes, 8);

	std::cout << "\nM->C1 [0]: ";
	print_block(result_c1.values[0].bytes, 8);

	for (int i = 0; i < slice_size; ++i) {
		assert(result_c0.values[i].raw == 0x502A9310B9F164FF);
		assert(result_c1.values[i].raw == 0x55520D27354ECF3);
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
		assert(result_C0.values[i].raw == 0x502A9310B9F164FF);
	}
	int appel = 1;
}

int main() {
	test_forkskinny64_192();
//	test();
	std::cout << "\n\nSuccess!";
}
