#include <cstdint>
#include <iostream>
#include <vector>
#include "src/config.h"

std::vector<uint64_t> segment(std::vector<uint64_t> slices) {
	auto res = std::vector<uint64_t>();
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res.push_back(slices.at(16 * i + j));
			res.push_back(slices.at(16 * i + 4 + j));
			res.push_back(slices.at(16 * i + 8 + j));
			res.push_back(slices.at(16 * i + 12 + j));
		}
	}
	return res;
}

// expand64 every bit of the raw to a 64bit slice_t
// if bit == 0: return 0⁶⁴
// if bit == 1: return 1⁶⁴
static inline std::vector<u64> to_slices(u64 value, uint8_t width) {
	auto res = std::vector<u64>();
	for (uint8_t i = 0; i < width; ++i) {
		if ((value & (1ULL << i)) >> i) res.push_back(-1ULL);
		else res.push_back(0x0);
	}
	
	return res;
}

void forkskinny64_branch_constant(){
	u64 bc = 0x81ec7f5bda364912;
	auto slices = to_slices(bc, 64);
	
	std::cout << "State64Sliced_t const branch_constant64 = {\n\t";
	for (int i = 0; i < 64; ++i) {
		if (i % 4 == 0 && i != 0) {
			uint8_t val = ((bc & (0xFUL << (i - 4))) >> (i - 4));
			std::cout << " // 0x" << std::hex << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // 0x" << std::hex << unsigned(((bc & (0xFUL << (60))) >> (60))) << "\n};\n\n";
}

void forkskinny64_segmented_branch_constant(){
	u64 bc = 0x81ec7f5bda364912;
	auto slices = to_slices(bc, 64);
	
	slices = segment(slices);
	
	std::cout << "State64Sliced_t const segmented_branch_constant64 = {\n\t";
	for (int i = 0; i < 64; ++i) {
		if (i % 4 == 0 && i != 0) {
			uint8_t val = ((bc & (0xFUL << (i - 4))) >> (i - 4));
			std::cout << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << "\n};\n\n";
}

void forkskinny128_branch_constant(){
	u64 bc[2] = {0x8241201008040201ULL, 0x8844a25128140a05ULL};
	auto slices = to_slices(bc[0], 64);
	
	std::cout << "State128Sliced_t const branch_constant128 = {\n\t";
	
	for (int i = 0; i < 64; ++i) {
		if (i % 8 == 0 && i != 0) {
			uint8_t val = (bc[0] & (0xFFUL << (i-8))) >> (i-8);
			std::cout << " // 0x" << std::hex << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // 0x" << std::hex << unsigned(((bc[0] & (0xFFUL << 56)) >> 56)) << "\n\t";
	
	slices = to_slices(bc[1], 64);
	for (int i = 0; i < 64; ++i) {
		if (i % 8 == 0 && i != 0) {
			uint8_t val = ((bc[1] & (0xFFUL << (i - 8))) >> (i - 8));
			std::cout << " // 0x" << std::hex << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // 0x" << std::hex << unsigned(((bc[1] & (0xFFUL << 56)) >> 56)) << "\n};\n\n";
}

void forkskinny_round_constants(){
	std::cout << "u64 forkskinny_precomputed_round_constants[88][7] = {";
	uint8_t lfsr = 0;
	for (int i = 0; i < 88; i++) {
		auto rc6 = (lfsr & 0b1000000) >> 6;
		auto rc5 = (lfsr & 0b0100000) >> 5;
		lfsr = ((lfsr & 0b0111111) << 1) | (rc6 ^ rc5 ^ 1);
		auto res = to_slices(lfsr, 7);
		
		std::cout << "\n\t{";
		for (auto &value: res) {
			if (value == 0) std::cout << "ZER, ";
			else std::cout << "ONE, ";
		}
		std::cout << "}, // 0x" << std::hex << unsigned(lfsr);
	}
	
	std::cout << "\n};";
}

int main() {
	forkskinny64_branch_constant();
	forkskinny64_segmented_branch_constant();
//	forkskinny128_branch_constant();
//	forkskinny_round_constants();
}