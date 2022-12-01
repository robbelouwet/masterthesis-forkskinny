#include <cstdint>
#include <x86intrin.h>
#include <valarray>
#include <vector>
#include "../forkskinny-plus/headers/forkskinny-plus.h"
#include "../headers/full-state-slicing.h"

State64Sliced_16_t  sliced_rol_sbox(State64Sliced_16_t x) {

	
}

uint64_t sliced_circuit_sbox(uint64_t x) {
	asm("nop");
	asm("nop");
	asm("nop");
	uint16_t x3 = (x & 0xffff000000000000) >> 48;
	uint16_t x2 = (x & 0x0000ffff00000000) >> 32;
	uint16_t x1 = (x & 0x00000000ffff0000) >> 16;
	uint16_t x0 = (x & 0x000000000000ffff);
	
	
	/// because our state is bit sliced, we can apply the logical operations from
	/// the S box's circuit in the paper directly on the slices themselves
	/// cfr.: fig. 2: https://eprint.iacr.org/2016/660.pdf
	uint16_t y3 = x0 ^ ~(x3 | x2);
	uint16_t y2 = x3 ^ ~(x2 | x1);
	uint16_t y1 = x2 ^ ~(x1 | y3);
	uint16_t y0 = x1 ^ ~(y3 | y2);
	
	auto res = (uint64_t) y0 | ((uint64_t) y1 << 16) | ((uint64_t) y2 << 32) | ((uint64_t) y3 << 48);
	
	asm("nop");
	asm("nop");
	asm("nop");
	return res;
}

uint64_t old_sbox(uint64_t x) {
	/* Splitting the bits out individually gives better performance on
	   64-bit platforms because we have more spare registers to work with.
	   This doesn't work as well on 32-bit platforms because register
	   spills start to impact performance.  See below. */
	asm("nop");
	asm("nop");
	asm("nop");
	uint64_t bit0 = ~x;
	uint64_t bit1 = bit0 >> 1;
	uint64_t bit2 = bit0 >> 2;
	uint64_t bit3 = bit0 >> 3;
	bit0 ^= bit3 & bit2;
	bit3 ^= bit1 & bit2;
	bit2 ^= bit1 & bit0;
	bit1 ^= bit0 & bit3;
	x = ((bit0 << 3) & 0x8888888888888888ULL) |
	    (bit1 & 0x1111111111111111ULL) |
	    ((bit2 << 1) & 0x2222222222222222ULL) |
	    ((bit3 << 2) & 0x4444444444444444ULL);
	auto res = ~x;
	asm("nop");
	asm("nop");
	asm("nop");
	
	return res;
}

int main(int argc, char **argv) {
	// create state stochastically to prevent compiler optimizing the state to a constant
	uint64_t state;
	if (argc == 0) state = 0xFFEEFFEEFFEEFF;
	else state = 0xABCDABCDABCDABCD;
	
	// slice the state
	auto sliced_state = State64Sliced_16_t();
	sliced_state.state = slice(state);
	
	// perform SBOXs
	auto old_sbox_res = old_sbox(state);
	auto sliced_sbox_res = sliced_circuit_sbox(sliced_state.state);
	
	// unslice
	auto unsliced_sbox_res = unslice(sliced_sbox_res);
	
	// double check LSFRs were computed correctly
	assert(old_sbox_res == unsliced_sbox_res);
	
	
	// unimportant
	// prevent compiler from optimizing previous function calls due to dead code or unused function output
	auto rres = std::vector<uint64_t>();
	if (old_sbox_res != 0) rres.push_back(old_sbox_res);
	if (unsliced_sbox_res != 0) rres.push_back(unsliced_sbox_res);
	
	return rres.size();
}