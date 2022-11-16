#include <forkskinny-c/headers/forkskinny128-cipher.h>
#include <forkskinny-c/headers/forkskinny64-cipher.h>
#include <skinny-plus/headers/internal-skinny-plus.h>
#include " forkskinny-plus.h"

void test() {
	uint8_t key[3 * FORKSKINNY128_BLOCK_SIZE] = {0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8,
	                                            0x1b, 0xe8, 0xe7, 0x8d, 0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a};
	uint8_t message[FORKSKINNY128_BLOCK_SIZE] = {0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec};
	
	// Pre-compute key schedule
	ForkSkinny128Key_t tk1, tk23;
	forkskinny_c_128_384_init_tk1(&tk1, key, FORKSKINNY128_MAX_ROUNDS);
	forkskinny_c_128_384_init_tk2(&tk23, key + FORKSKINNY128_BLOCK_SIZE, FORKSKINNY128_MAX_ROUNDS);
	forkskinny_c_128_384_init_tk3(&tk23, key + FORKSKINNY128_BLOCK_SIZE, FORKSKINNY128_MAX_ROUNDS);
	
	
	forkskinny_c_128_384_encrypt()
	
	// encrypt a 128-bit block with skinny-128-384+
	skinny_plus_encrypt()

}
