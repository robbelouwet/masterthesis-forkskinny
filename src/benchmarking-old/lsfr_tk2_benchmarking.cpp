//#include <x86intrin.h>
#include "../headers/full-state-slicing.h"
#include "../forkskinny-plus/headers/forkskinny64-plus.h"
//#include <immintrin.h>
#include <cassert>
#include <vector>
#include <random>

static inline uint32_t skinny64_LFSR2(uint32_t x) {
	return ((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U);
}

static inline u64 skinny64_LFSR2_full(u64 x) {
	return ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
}

u64 sequential_abcde(u64 state) {
	state = ((state << 1) & 0xEEEEEEEEEEEEEEEE)
	        ^
	        (((state >> 3) ^ (state >> 2)) & 0x1111111111111111);
	return state;
}

u64 sliced_fghi(u64 state) {
	
	state = ((state & 0xFFFF000000000000) >> 0x30) ^ (state << 0x10);
	state ^= (state & 0xFFFF000000000000) >> 0x30;
//	sliced2.slices[0] ^= sliced2.slices[3];
	
	return state;
}

// <editor-fold desc="lsfr-benchmark">
/*State64Sliced_16_t sliced_simd_jklm(State64Sliced_16_t state) {
	// to make the shuffle perform a rol, indices 3210 go to 2103; 2103 is 10 01 00 11 in binary
	state.m64state = _mm_shuffle_pi16(state.m64state, 0b10010011);
	state.slices[0] ^= state.slices[3];
	return state;
}*/
//</editor-fold>

int main(int argc, char **argv) {
	// create state stochastically to prevent compiler optimizing the state to a constant
	u64 state = ((long long)random() << 32) | random(); // 2**64 - 1
	
	// slice_t the state
	auto sliced_state = State64Sliced_16_t();
	sliced_state.state = slice(state);
	
	// perform LSFRs
	//auto sliced_simd_res = sliced_simd_jklm(sliced_state);
	auto sliced_res = sliced_fghi(sliced_state.state);
	auto sequential_res = sequential_abcde(state);
	
	// unslice
	//auto unsliced_simd_res = unslice_accelerated(sliced_simd_res.state);
	auto unsliced_res = unslice_accelerated(sliced_res);
	
	// double check LSFRs were computed correctly
	//assert(unsliced_res == unsliced_simd_res);
	assert(unsliced_res == sequential_res);
	
	
	// unimportant
	// prevent compiler from optimizing previous function calls due to dead code or unused function output
	auto rres = std::vector<u64>();
	if (unsliced_res != 0) rres.push_back(unsliced_res);
	//if (unsliced_simd_res != 0) rres.push_back(unsliced_simd_res);
	if (sequential_res != 0) rres.push_back(sequential_res);
	
	return rres.size();
}
