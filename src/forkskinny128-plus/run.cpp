#include <cstdio>
#include <iostream>
#include "utils/slicing.h"
#include "keyschedule/keyschedule.h"
#include "forkskinny128_384.h"
#include "keyschedule/fixslicing/fixsliced-keyschedule.h"

void print_block(uint8_t *block, unsigned int n) {
	for (unsigned int i = 0; i < n; i++)
		printf("%02x", block[i]);
}

void forkskinny128_test() {
	// Set 128 state blocks, with only the first one containing A's
	auto state = Blocks_t{.values = {{.bytes = {
			0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}};
	auto sliced_state = slice(state);
	
	// Set TK1
	auto tk1_blocks = Blocks_t{.values = {{.bytes =
			{0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d,}}}};
	auto sliced_TK1 = slice(tk1_blocks);
	
	// Set TK2
	auto tk2_blocks = Blocks_t{.values = {{.bytes = {
			0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a, 0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0x80, 0x01,}}}};
	auto sliced_TK2 = slice(tk2_blocks);
	
	// Set TK3
	auto tk3_blocks = Blocks_t{.values = {{.bytes = {
			0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}};
	auto sliced_TK3 = slice(tk3_blocks);
	
	// Calculate TK schedule
	auto keyschedule = forkskinny_128_init_tk23(sliced_TK1, sliced_TK2, sliced_TK3);
	
	// encrypt Forkskinny-128-192 with s=b mode
	auto ciphertext = forkskinny128_384_encrypt(keyschedule, &sliced_state, 'b');
	
	auto C0 = unslice(ciphertext.C0).values[0];
	std::cout << "\nC0: ";
	print_block(C0.bytes, 8);
	
	auto C1 = unslice(ciphertext.C1).values[0];
	std::cout << "\nC1: ";
	print_block(C1.bytes, 8);
	
	auto recovered_pt = forkskinny128_decrypt(keyschedule, &ciphertext, '1', 'i');
	
	auto recovered_M = unslice(recovered_pt.M).values[0];
	
	auto recovered_C0 = unslice(recovered_pt.C0).values[0].raw;
	
	auto recovered_C1 = unslice(recovered_pt.C1).values[0].raw;
	
	std::cout << "\nrecovered M: ";
	print_block(recovered_M.bytes, 8);
	
	int appel = 1;
}

int main() {
	forkskinny128_test();
}