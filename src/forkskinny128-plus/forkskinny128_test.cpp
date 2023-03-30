#include <cassert>
#include "utils/forkskinny128-datatypes.h"
#include "utils/slicing128.h"

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
auto sliced_TK3 = slice(tk3_blocks);

bool test_forkskinny_128_128() {

}

bool test_forkskinny_128_192() {

}

bool test_forkskinny_128_256() {

}

bool test_forkskinny_128_320() {

}

bool test_forkskinny_128_384() {

}


int main() {
	assert(test_forkskinny_128_128());
	assert(test_forkskinny_128_192());
	assert(test_forkskinny_128_256());
	assert(test_forkskinny_128_320());
	assert(test_forkskinny_128_384());
}