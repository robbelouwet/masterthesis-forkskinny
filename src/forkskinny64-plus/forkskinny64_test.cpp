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
auto sliced_TK3_128 = slice(tk3_blocks);

bool test_forkskinny64(KeySchedule64Sliced_t schedule, uint64_t c0, uint64_t c1) {
	
	// Ensure correct test vectors
	auto ct = forkskinny64_encrypt(schedule, &sliced_state, 'b');
	auto result_c0 = unslice(ct.C0).values[0].raw;
	auto result_c1 = unslice(ct.C1).values[0].raw;
	
	assert(result_c0 == c0);
	assert(result_c1 == c1);
	
	auto pt = forkskinny64_decrypt(schedule, &ct, '1', 'b');
	auto result_M = unslice(pt.M).values[0].raw;
	auto result_C0 = unslice(pt.C0).values[0].raw;
	
	assert(result_M == state.values[0].raw);
	assert(result_C0 == c0);
}

void test_forkskinny_64_192() {
	// Calculate TK schedule
	test_forkskinny64(
			forkskinny_64_init_tk23(sliced_TK1, sliced_TK2, sliced_TK3_128),
			0x502A9310B9F164FF,
			0x55520D27354ECF3
	);
}

int main() {
	test_forkskinny_64_192();
}
