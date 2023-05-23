#include <cstdint>
#include <x86intrin.h>
#include <valarray>
#include <vector>
#include "../forkskinny-plus/headers/forkskinny64-plus.h"
#include "../headers/full-state-slicing.h"

// 3*9 + 8
State64Sliced_16_t sliced_sbox(State64Sliced_16_t x) {
	x.slices[0] ^= ~(x.slices[3] | x.slices[2]);
	x.state = _lrotl(x.state, 16);
	
	x.slices[0] ^= ~(x.slices[3] | x.slices[2]);
	x.state = _lrotl(x.state, 16);
	
	x.slices[0] ^= ~(x.slices[3] | x.slices[2]);
	x.state = _lrotl(x.state, 16);
	
	x.slices[0] ^= ~(x.slices[3] | x.slices[2]);
	
	return x;
}

// 24
auto sliced_circuit_sbox(const uint16_t slices[4]) {
	/// because our state is bit sliced, we can apply the logical operations from
	/// the S box's circuit in the paper directly on the slices themselves
	/// cfr. fig. 2: https://eprint.iacr.org/2016/660.pdf
	asm("nop");
	asm("nop");
	asm("nop");
	
	uint16_t y3 = slices[0] ^ ~(slices[3] | slices[2]);
	uint16_t y2 = slices[3] ^ ~(slices[2] | slices[1]);
	uint16_t y1 = slices[2] ^ ~(slices[1] | y3);
	uint16_t y0 = slices[1] ^ ~(y3 | y2);
	
	auto res = {y0, y1, y2, y3};
	asm("nop");
	asm("nop");
	asm("nop");
	
	return res;
}

// +- 34
u64 old_sbox(u64 x) {
	/* Splitting the bits out individually gives better performance on
	   64-bit platforms because we have more spare registers to work with.
	   This doesn't work as well on 32-bit platforms because register
	   spills start to impact performance.  See below. */
	u64 bit0 = ~x;
	u64 bit1 = bit0 >> 1;
	u64 bit2 = bit0 >> 2;
	u64 bit3 = bit0 >> 3;
	bit0 ^= bit3 & bit2;
	bit3 ^= bit1 & bit2;
	bit2 ^= bit1 & bit0;
	bit1 ^= bit0 & bit3;
	x = ((bit0 << 3) & 0x8888888888888888ULL) |
	    (bit1 & 0x1111111111111111ULL) |
	    ((bit2 << 1) & 0x2222222222222222ULL) |
	    ((bit3 << 2) & 0x4444444444444444ULL);
	auto res = ~x;
	return res;
}

int main(int argc, char **argv) {
	// create state stochastically to prevent compiler optimizing the state to a constant
	u64 state;
	if (argc == 0) state = 0xFFEEFFEEFFEEFF;
	else state = 0xABCDABCDABCDABCD;
	
	// slice_t the state
	auto sliced_state = State64Sliced_16_t();
	sliced_state.state = slice(state);
	
	// perform SBOXs
	auto old_sbox_res = old_sbox(state);
	auto sliced_circuit_sbox_res = State64Sliced_16_t();
	// BROKEN sliced_circuit_sbox_res.slices = sliced_circuit_sbox(sliced_state.slices);
	auto sliced_sbox_res = sliced_sbox(sliced_state).state;
	
	// unslice_internal
	auto unsliced_circuit_sbox_res = unslice_accelerated(sliced_circuit_sbox_res.state);
	auto unsliced_sbox_res = unslice_accelerated(sliced_sbox_res);
	
	// double check LSFRs were computed correctly
	assert(old_sbox_res == unsliced_circuit_sbox_res);
	assert(old_sbox_res == unsliced_sbox_res);
	
	
	// unimportant
	// prevent compiler from optimizing previous function calls due to dead code or unused function output
	auto rres = std::vector<u64>();
	if (old_sbox_res != 0) rres.push_back(old_sbox_res);
	if (unsliced_sbox_res != 0) rres.push_back(unsliced_sbox_res);
	if (unsliced_circuit_sbox_res != 0) rres.push_back(unsliced_circuit_sbox_res);
	
	return rres.size();
}