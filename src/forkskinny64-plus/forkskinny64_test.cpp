#include <cassert>
#include "utils/forkskinny64-datatypes.h"
#include "utils/slicing64.h"
#include "keyschedule/keyschedule64.h"
#include "forkskinny64.h"

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

bool test_forkskinny64(KeySchedule64Sliced_t schedule, ) {
	
	// Ensure correct test vectors
	auto ciphertext1 = forkskinny64_encrypt(schedule, &sliced_state, 'b');
	auto decryption1 = forkskinny64_decrypt(schedule, &ciphertext1, '1', 'i');
	assert(ciphertext1.C0.raw[0].value == 0x502A9310B9F164FF);
	assert(ciphertext1.C1.raw[0].value == 0x55520D27354ECF3);
	
	// 1) Decryption of encryption results back in M
	assert(ciphertext1.M.raw[0].value == state.values[0].raw);
	
	// 2) E⁻¹(C0, 1) -> {M, C1}
	auto ciphertext2 = forkskinny64_encrypt(schedule, &sliced_state, '1');
	auto decryption2 = forkskinny64_decrypt(schedule, &ciphertext2, '1', 'i');
}

bool test_forkskinny_64_64() {
	// Calculate TK schedule
	auto keyschedule = forkskinny_64_init_tk23(sliced_TK1, sliced_TK2, sliced_TK3);
}

bool test_forkskinny_64_128() {

}

bool test_forkskinny_64_192() {

}

int main() {
	test_forkskinny_64_64();
	test_forkskinny_64_128();
	test_forkskinny_64_192();
}
