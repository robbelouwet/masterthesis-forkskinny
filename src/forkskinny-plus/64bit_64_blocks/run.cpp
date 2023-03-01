#include "utils/slicing.h"
#include "keyschedule/keyschedule64.h"
#include "skinny64_64_blocks.h"

int main() {
	// Set 64 state blocks, with only the first one containing A's
	auto state = Blocks64_t();
	state.values[0] = 0xAAAAAAAAAAAAAAAA;
	auto sliced_state = slice(state);
	
	// Set TK1
	auto tk1_blocks = Blocks64_t();
	tk1_blocks.values[0] = 0xBBBBBBBBBBBBBBBB;
	auto sliced_TK1 = slice(tk1_blocks);
	
	// Set TK2
	auto tk2_blocks = Blocks64_t();
	tk2_blocks.values[0] = 0xCCCCCCCCCCCCCCCC;
	auto sliced_TK2 = slice(tk2_blocks);
	
	// Set TK3
	auto tk3_blocks = Blocks64_t();
	tk3_blocks.values[0] = 0xDDDDDDDDDDDDDDDD;
	auto sliced_TK3 = slice(tk3_blocks);
	
	// Calculate TK schedule
	auto keyschedule = precompute_64_key_schedules(sliced_TK1, sliced_TK2, sliced_TK3);
	auto round_key0 = unslice({.halves = {keyschedule.keys[0], {}}});
	auto round_key1 = unslice({.halves = {keyschedule.keys[1], {}}});
	auto round_key2 = unslice({.halves = {keyschedule.keys[2], {}}});
	
	
	
	// encrypt Forkskinny-64-192 with s=b mode
	auto ciphertext = forkskinny64_encrypt_64_blocks(keyschedule, &sliced_state);
	
	// unslice ciphertext
	auto left = unslice(ciphertext.left);
	auto right = unslice(ciphertext.right);
	
	int appel = 1;
}