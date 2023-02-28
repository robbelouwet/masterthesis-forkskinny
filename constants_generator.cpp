#include <cstdint>
#include <iostream>
#include <vector>
#include "src/forkskinny-plus/64bit_64_blocks/utils/skinny64_datatypes.h"

// expand every bit of the value to a 64bit slice
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

int main() {
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
	
	std::cout << "uint64_t precomputed_round_constants[FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER][7] = {";
	uint8_t lfsr = 0;
	for (int i = 0; i < FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER; ++i) {
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