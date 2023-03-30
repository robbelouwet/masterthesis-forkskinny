#include <cstdio>
#include <iostream>
#include "utils/slicing128.h"
#include "keyschedule/keyschedule128.h"
#include "forkskinny128_384.h"
#include "keyschedule/fixslicing/fixslicing128-keyschedule.h"

void print_block(uint8_t *block, unsigned int n) {
	for (unsigned int i = 0; i < n; i++)
		printf("%02x", block[i]);
}

void forkskinny128_test(unsigned char tk_mode) {
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
	auto sliced_TK1 = slice(tk1_blocks);
	
	// Set TK2
	auto tk2_blocks = Blocks128_t{.values = {{.bytes = {
			0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a, 0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0x80, 0x01}}}};
	auto sliced_TK2 = slice(tk2_blocks);
	
	// Set TK3
	auto tk3_blocks = Blocks128_t{.values = {{.bytes = {
			0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d}}}};
	auto sliced_TK3 = slice(tk3_blocks);
	
	// Calculate TK schedule
	// should be:
	// 0x9AC99F33632C5A77, 0x6AF595E4AC0D4945, 0xB96C01BAB952D018
	auto keyschedule = KeySchedule128Sliced_t();
	forkskinny_128_init_tk23(sliced_TK1, sliced_TK2, sliced_TK3);
//	auto testk0 = unslice({.halves = {keyschedule.keys[0], {}}}).values[0].raw[0];
//	auto testk1 = unslice({.halves = {keyschedule.keys[1], {}}}).values[0].raw[0];
//	auto testk2 = unslice({.halves = {keyschedule.keys[2], {}}}).values[0].raw[0];
	
	// encrypt Forkskinny-128-192 with s=b mode
	auto ciphertext = forkskinny128_384_encrypt(keyschedule, &sliced_state, 'b');
	
	std::cout << "Message: ";
	print_block(state.values[0].bytes, 16);
	
	// 168cdc774187d87273d21fa18ea46d26
	auto C0 = unslice(ciphertext.C0).values[0];
	std::cout << "\nC0: ";
	print_block(C0.bytes, 16);
	
	// 062fa2a6e88c314f45691ccd8edde209
	auto C1 = unslice(ciphertext.C1).values[0];
	std::cout << "\nC1: ";
	print_block(C1.bytes, 16);
	
	auto decryption = forkskinny128_decrypt(keyschedule, &ciphertext, '1', 'b');
	
	// 67c6697351ff4aec8000000000000000
	auto recovered_M = unslice(decryption.M).values[0];
	
	auto recovered_C0 = unslice(decryption.C0).values[0];
	auto recovered_C1 = unslice(decryption.C1).values[0];
	
	std::cout << "\n\nrecovered M: ";
	print_block(recovered_M.bytes, 16);
	
	std::cout << "\nrecovered C0: ";
	print_block(recovered_C0.bytes, 16);
	
	std::cout << "\nrecovered C1: ";
	print_block(recovered_C1.bytes, 16);
	
	//int appel = 1;
}

int main() {
	forkskinny128_test();
}