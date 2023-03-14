#include <cstdint>
#include <iostream>
#include <vector>
#include "src/forkskinny-plus/64bit_64_blocks/utils/skinny64_datatypes.h"

// expand every bit of the value to a 64bit slice_t
// if bit == 0: return 0⁶⁴
// if bit == 1: return 1⁶⁴
static inline std::vector<uint64_t> to_slices(uint64_t value, uint8_t width) {
	auto res = std::vector<uint64_t>();
	for (uint8_t i = 0; i < width; ++i) {
		if ((value & (1 << i)) >> i) res.push_back(0xFFFFFFFFFFFFFFFF);
		else res.push_back(0x0);
	}
	
	return res;
}

void forkskinny64_branch_constant(){
	uint64_t bc = 0x18cef7b5ad639421;
	auto slices = to_slices(bc, 64);
	
	std::cout << "uint64_t branchconstant_64[64] = {\n\t";
	for (int i = 0; i < 64; ++i) {
		if (i % 4 == 0 && i != 0) {
			uint8_t val = ((bc & (0xFUL << (i - 4))) >> (i - 4));
			std::cout << " // " << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // " << unsigned(((bc & (0xFUL << (60))) >> (60))) << "\n};\n\n";
}

void forkskinny64_round_constants(){
	std::cout << "uint64_t forkskinny64_precomputed_round_constants[17 + 2 * 23][7] = {";
	uint8_t lfsr = 0;
	for (int i = 0; i < 17 + 2 * 23; ++i) {
		auto rc6 = (lfsr & 0b1000000) >> 6;
		auto rc5 = (lfsr & 0b0100000) >> 5;
		lfsr = ((lfsr & 0b0111111) << 1) | (rc6 ^ rc5 ^ 1);
		
		auto res = to_slices(lfsr, 7);
		
		std::cout << "\n\t{";
		for (auto &value: res) {
			if (value == 0) std::cout << "ZER, ";
			else std::cout << "ONE, ";
		}
		std::cout << "}, // " << unsigned(lfsr);
	}
	
	std::cout << "\n};";
}

void skinny64_round_constants(){
	std::cout << "uint64_t skinny64_precomputed_round_constants[56][6] = {";
	uint8_t lfsr = 0;
	for (int i = 0; i < 56; ++i) { // 56 = max number of rounds, aka with skinny128-192
		auto rc5 = (lfsr & 0b100000) >> 5;
		auto rc4 = (lfsr & 0b010000) >> 4;
		lfsr = ((lfsr & 0b011111) << 1) | (rc5 ^ rc4 ^ 1);
		
		auto res = to_slices(lfsr, 6);
		
		std::cout << "\n\t{";
		for (auto &value: res) {
			if (value == 0) std::cout << "ZER, ";
			else std::cout << "ONE, ";
		}
		std::cout << "}, // " << unsigned(lfsr);
	}
	
	std::cout << "\n};";
}

int main() {
//	forkskinny64_branch_constant();
	
//	forkskinny64_round_constants();
	
	skinny64_round_constants();
	
}