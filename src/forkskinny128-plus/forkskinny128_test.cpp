#include <cassert>
#include "utils/forkskinny128-datatypes.h"
#include "utils/slicing128.h"
#include "forkskinny128.h"
#include "keyschedule/keyschedule128.h"


void test_forkskinny_128_256() {
	auto sliced_state = slice(Blocks128_t{.values = {{.bytes = {
			0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}});

// Set TK1
	auto sliced_TK1 = slice(Blocks128_t{.values = {{.bytes =
			{0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}});

// Set TK2
	auto sliced_TK2 = slice(Blocks128_t{.values = {{.bytes = {
			0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a, 0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0x80, 0x01}}}});

// Set TK3
	auto sliced_TK3 = slice(Blocks128_t{.values = {{.bytes = {
			0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}});
	
	auto schedule = forkskinny_128_keyschedule_tk2(sliced_TK1, sliced_TK2);
	
	
	// Ensure correct test vectors
	auto ct = forkskinny128_encrypt(schedule, &sliced_state, 'b', FORKSKINNY_128_256_ROUNDS_BEFORE,
	                                FORKSKINNY_128_256_ROUNDS_AFTER);
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	// verify test vectors
	assert(result_c0.raw[0] == 0x7701F1FC783BF8DC);
	assert(result_c0.raw[1] == 0x2DB6D34C76C0414D);
	assert(result_c1.raw[0] == 0x3E60F9E2B65E49CB);
	assert(result_c1.raw[1] == 0xD5CDDFBC9440EE51);
	
	auto pt = forkskinny128_decrypt(schedule, &ct, '1', 'b', FORKSKINNY_128_256_ROUNDS_BEFORE,
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
	auto sliced_state = slice(Blocks128_t{.values = {{.bytes = {
			0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}});

// Set TK1
	auto sliced_TK1 = slice(Blocks128_t{.values = {{.bytes =
			{0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}});

// Set TK2
	auto sliced_TK2 = slice(Blocks128_t{.values = {{.bytes = {
			0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a, 0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0x80, 0x01}}}});

// Set TK3
	auto sliced_TK3 = slice(Blocks128_t{.values = {{.bytes = {
			0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}});
	
	auto schedule = forkskinny_128_keyschedule_tk23(sliced_TK1, sliced_TK2, sliced_TK3);
	
	// Ensure correct test vectors
	auto ct = forkskinny128_encrypt(schedule, &sliced_state, 'b', FORKSKINNY_128_384_ROUNDS_BEFORE,
	                                FORKSKINNY_128_384_ROUNDS_AFTER);
	auto result_c0 = unslice(ct.C0).values[0];
	auto result_c1 = unslice(ct.C1).values[0];
	
	// verify test vectors
	assert(result_c0.raw[0] == 0x72D8874177DC8C16);
	assert(result_c0.raw[1] == 0x266DA48EA11FD273);
	assert(result_c1.raw[0] == 0x4F318CE8A6A22F06);
	assert(result_c1.raw[1] == 0x9E2DD8ECD1C6945);
	
	auto pt = forkskinny128_decrypt(schedule, &ct, '1', 'b', FORKSKINNY_128_384_ROUNDS_BEFORE,
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
	test_forkskinny_128_256();
	test_forkskinny_128_384();
}