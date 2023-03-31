#include <cassert>
#include "utils/forkskinny64-datatypes.h"
#include "utils/slicing64.h"
#include "keyschedule/keyschedule64.h"
#include "forkskinny64.h"

bool test_forkskinny64_192() {
	auto original_pt = Blocks64_t{.values = {{.bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}}}};
	auto sliced_state = slice(original_pt);

	// Set TK1
	auto sliced_TK1 = slice(Blocks64_t{.values = {{.bytes = {0xf1, 0x11, 0x00, 0x22, 0x00, 0x33, 0x00, 0x44,}}}});

	// Set TK2
	auto sliced_TK2 = slice(Blocks64_t{.values = {{.bytes = {0xf2, 0x55, 0x00, 0x66, 0x00, 0x77, 0x00, 0x88,}}}});

	// Set TK3
	auto sliced_TK3 = slice(Blocks64_t{.values = {{.bytes = {0xf3, 0x99, 0x00, 0xaa, 0x00, 0xbb, 0x00, 0xcc}}}});
	
	auto schedule = forkskinny_64_init_tk23(sliced_TK1, sliced_TK2, sliced_TK3);
	
	// Ensure correct test vectors
	auto ct = forkskinny64_encrypt(schedule, &sliced_state, 'b');
	auto result_c0 = unslice(ct.C0).values[0].raw;
	auto result_c1 = unslice(ct.C1).values[0].raw;
	
	assert(result_c0 == 0x502A9310B9F164FF);
	assert(result_c1 == 0x55520D27354ECF3);
	
	auto pt = forkskinny64_decrypt(schedule, &ct, '1', 'b');
	auto result_M = unslice(pt.M).values[0].raw;
	auto result_C0 = unslice(pt.C0).values[0].raw;
	
	assert(result_M == original_pt.values[0].raw);
	assert(result_C0 == 0x502A9310B9F164FF);
}

int main() {
	test_forkskinny64_192();
}
