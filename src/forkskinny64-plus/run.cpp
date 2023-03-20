#include <cstdio>
#include <iostream>
#include "utils/slicing.h"
#include "keyschedule/keyschedule64.h"
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
	auto tk1_blocks = Blocks64_t{.values = {{.bytes = {0xf1, 0x11, 0x00, 0x22, 0x00, 0x33, 0x00, 0x44,}}}};
	auto sliced_TK1 = slice(tk1_blocks);
	
	// Set TK2
	auto tk2_blocks = Blocks64_t{.values = {{.bytes = {0xf2, 0x55, 0x00, 0x66, 0x00, 0x77, 0x00, 0x88,}}}};
	auto sliced_TK2 = slice(tk2_blocks);
	
	// Set TK3
	auto tk3_blocks = Blocks64_t{.values = {{.bytes = {0xf3, 0x99, 0x00, 0xaa, 0x00, 0xbb, 0x00, 0xcc}}}};
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

	auto recovered_M = unslice(recovered_pt.M).values[0];
	
	// ff64f1b910932a50
	auto recovered_C0 = unslice(recovered_pt.C0).values[0].raw;
	
	// f3ec5473d2205505
	auto recovered_C1 = unslice(recovered_pt.C1).values[0].raw;
	
	std::cout << "\nrecovered M: ";
	print_block(recovered_M.bytes, 8);
	
	int appel = 1;
}

int main() {
	forkskinny_test();
}