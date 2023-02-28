#include "utils/slicing.h"
#include "keyschedule/keyschedule64.h"
#include "skinny64_64_blocks.h"

int main() {
	// Set 64 state blocks, with only the first one containing A's
	auto state = Blocks64_t();
	state.values[0] = 0xAAAAAAAAAAAAAAAA;
	auto sliced_state = slice(state);
	
	// Set Tweakey
	auto tk0_blocks = Blocks64_t();
	tk0_blocks.values[0] = 0xBBBBBBBBBBBBBBBB;
	auto sliced_TK0 = slice(tk0_blocks);
	
	// Calculate TK schedule
	auto zero_tk2 = State64Sliced_t();
	auto zero_tk3 = State64Sliced_t();
	auto keyschedule = precompute_64_key_schedules(sliced_TK0, zero_tk2, zero_tk3);
	
	auto ciphertext = forkskinny64_encrypt_64_blocks(keyschedule, &sliced_state);
	
	auto left = unslice(ciphertext.left);
	auto right = unslice(ciphertext.right);
	
	int appel = 1;
}