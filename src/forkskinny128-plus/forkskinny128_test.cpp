#include <cassert>
#include "utils/forkskinny128-datatypes.h"
#include "utils/slicing128.h"
#include "forkskinny128_384.h"
#include "keyschedule/keyschedule128.h"

auto state = Blocks128_t{.values = {{.bytes = {
		0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}};
auto sliced_state = slice(state);
//	auto unsliced_idk0 = unslice(sliced_state).values[0].raw[0];
//	auto unsliced_idk1 = unslice(sliced_state).values[0].raw[1];
//	int appel = 1;
//
// Set TK1
auto tk1_blocks = Blocks128_t{.values = {{.bytes =
		{0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}};
auto sliced_TK1_128 = slice(tk1_blocks);

// Set TK2
auto tk2_blocks = Blocks128_t{.values = {{.bytes = {
		0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a, 0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0x80, 0x01}}}};
auto sliced_TK2_128 = slice(tk2_blocks);

// Set TK3
auto tk3_blocks = Blocks128_t{.values = {{.bytes = {
		0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}};
auto sliced_TK3_128 = slice(tk3_blocks);

void test_forkskinny_128_256() {
	auto schedule = forkskinny_128_init_tk2(sliced_TK1_128, sliced_TK2_128);
	
	// Ensure correct test vectors
	auto ct = forkskinny128_384_encrypt(schedule, &sliced_state, 'b');
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	// verify test vectors
	assert(result_c0.raw[0] == 0xDB2F4D218B72BEBE);
	assert(result_c0.raw[1] == 0xA878DE85ED40AF80);
	assert(result_c1.raw[0] == 0x3C5ED24A76F67EA8);
	assert(result_c1.raw[1] == 0xB81CBB3358BB6381);
	
	auto pt = forkskinny128_decrypt(schedule, &ct, '1', 'b');
	auto result_M = unslice(pt.M).values[0];
	auto result_C0 = unslice(pt.C0).values[0];
	
	// test C1 -> (C0, M)
	assert(result_M.raw[0] == 0xEC4AFF517369C667);
	assert(result_M.raw[1] == 0x80);
	assert(result_C0.raw[0] == 0xDB2F4D218B72BEBE);
	assert(result_C0.raw[1] == 0xA878DE85ED40AF80);

}

void test_forkskinny_128_384() {
	auto schedule = forkskinny_128_init_tk23(sliced_TK1_128, sliced_TK2_128, sliced_TK3_128);
	
	// Ensure correct test vectors
	auto ct = forkskinny128_384_encrypt(schedule, &sliced_state, 'b');
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	// verify test vectors
	assert(result_c0.raw[0] == 0x72D8874177DC8C16);
	assert(result_c0.raw[1] == 0x266DA48EA11FD273);
	assert(result_c1.raw[0] == 0x4F318CE8A6A22F06);
	assert(result_c1.raw[1] == 0x9E2DD8ECD1C6945);
	
	auto pt = forkskinny128_decrypt(schedule, &ct, '1', 'b');
	auto result_M = unslice(pt.M).values[0];
	auto result_C0 = unslice(pt.C0).values[0];
	
	// test C1 -> (C0, M)
	assert(result_M.raw[0] == 0xEC4AFF517369C667);
	assert(result_M.raw[1] == 0x80);
	assert(result_C0.raw[0] == 0x72D8874177DC8C16);
	assert(result_C0.raw[1] == 0x266DA48EA11FD273);
}


int main() {
	//test_forkskinny_128_256();
	test_forkskinny_128_384();
}