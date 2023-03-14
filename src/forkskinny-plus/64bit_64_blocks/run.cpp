#include <cstdio>
#include <iostream>
#include "utils/slicing.h"
#include "keyschedule/keyschedule64.h"
#include "skinny64_64_blocks.h"
#include "forkskinny64_64_blocks.h"

void print_block(uint8_t *block, unsigned int n) {
	for (unsigned int i = 0; i < n; i++)
		printf("%02x", block[i]);
}

void forkskinny_test(){
	// Set 64 state blocks, with only the first one containing A's
	auto state = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
	auto sliced_state = slice(state);
	
	// Set TK1
	auto tk1_blocks = Blocks64_t();
	tk1_blocks.values[0].raw = 0x0;
	auto sliced_TK1 = slice(tk1_blocks);
	
	// Set TK2
	auto tk2_blocks = Blocks64_t{.values = {{.bytes = {0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe}}}};
	auto sliced_TK2 = slice(tk2_blocks);
	
	// Set TK3
	auto tk3_blocks = Blocks64_t();
	tk3_blocks.values[0].raw = 0x0;
	auto sliced_TK3 = slice(tk3_blocks);
	
	// Calculate TK schedule
	auto keyschedule = forkskinny_64_init_tk123(sliced_TK1, sliced_TK2, sliced_TK3);
	
	// encrypt Forkskinny-64-192 with s=b mode
	auto ciphertext = forkskinny64_encrypt_64_blocks(keyschedule, &sliced_state, 'b');
	
	auto C0 = unslice(ciphertext.C0).values[0];
	std::cout << "\nC0: ";
	print_block(C0.bytes, 8);
	
	auto C1 = unslice(ciphertext.C1).values[0];
	std::cout << "\nC1: ";
	print_block(C1.bytes, 8);
	
	auto recovered_pt = forkskinny64_decrypt_64_blocks(keyschedule, &ciphertext, '1', 'i');

	auto recovered_M = unslice(recovered_pt.M).values[0].raw;
	auto recovered_C0 = unslice(recovered_pt.C0).values[0].raw;
	auto recovered_C1 = unslice(recovered_pt.C1).values[0].raw;
	
	int appel = 1;
}

//void skinny_test(){
//	// Set 64 state blocks, with only the first one containing A's
//	auto state = Blocks64_t();
//	state.values[0].raw = 0x530c61d35e8663c3; // 0x530c61d35e8663c3, 0x3c3668e53d16c035
//	auto sliced_state = slice(state);
//
//	// Set TK1
//	auto tk1_blocks = Blocks64_t();
//	tk1_blocks.values[0].raw = 0xed00c85b120d6861; // 0xed00c85b120d6861, 0x1686d021b58c00de
//	auto sliced_TK1 = slice(tk1_blocks);
//
//	// Set TK2
//	auto tk2_blocks = Blocks64_t();
//	tk2_blocks.values[0].raw = 0x8753e24bfd908f60; // 0x8753e24bfd908f60, 0x06f809dfb42e3578
//	auto sliced_TK2 = slice(tk2_blocks);
//
//	// Set TK3
//	auto tk3_blocks = Blocks64_t();
//	tk3_blocks.values[0].raw = 0xb2dbb41b422dfcd0; // 0xb2dbb41b422dfcd0, 0x0dcfd224b14bbd2b
//	auto sliced_TK3 = slice(tk3_blocks);
//
//	// Calculate TK schedule
//	auto keyschedule = forkskinny_64_init_tk123(sliced_TK1, sliced_TK2, sliced_TK3);
//	auto round_key0 = unslice({.halves = {keyschedule.keys[0], {}}});
//	auto round_key1 = unslice({.halves = {keyschedule.keys[1], {}}});
//	auto round_key2 = unslice({.halves = {keyschedule.keys[2], {}}});
//
//	// encrypt Forkskinny-64-192 with s=b mode
//	skinny64_encrypt_64_blocks(keyschedule, &sliced_state);
//
//	// should be: 0xdd2cf1a8f330303c
//	auto ciphertext = unslice(sliced_state).values[0];
//	int appel = 1;
//}

int main() {
	forkskinny_test();
}