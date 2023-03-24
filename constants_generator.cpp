#include <cstdint>
#include <iostream>
#include <vector>

// expand every bit of the raw to a 64bit slice_t
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
	uint64_t bc = 0x81ec7f5bda364912;
	auto slices = to_slices(bc, 64);
	
	std::cout << "uint64_t branchconstant_64[64] = {\n\t";
	for (int i = 0; i < 64; ++i) {
		if (i % 4 == 0 && i != 0) {
			uint8_t val = ((bc & (0xFUL << (i - 4))) >> (i - 4));
			std::cout << " // " << std::hex << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // " << std::hex << unsigned(((bc & (0xFUL << (60))) >> (60))) << "\n};\n\n";
}

void forkskinny128_branch_constant(){
	uint64_t bc[2] = {0x4182102004080102, 0x448851a21428050a};
	auto slices = to_slices(bc[0], 64);
	
	std::cout << "uint64_t branchconstant_128[128] = {\n\t";
	
	for (int i = 0; i < 64; ++i) {
		if (i % 8 == 0 && i != 0) {
			uint8_t val = ((bc[0] & (0xFFUL << (i - 8))) >> (i - 8));
			std::cout << " // " << std::hex << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // " << std::hex << unsigned(((bc[0] & (0xFFUL << 56)) >> 56)) << "\n\t";
	
	slices = to_slices(bc[1], 64);
	for (int i = 0; i < 64; ++i) {
		if (i % 8 == 0 && i != 0) {
			uint8_t val = ((bc[1] & (0xFFUL << (i - 8))) >> (i - 8));
			std::cout << " // " << std::hex << unsigned(val) << "\n\t";
		}
		
		if (slices.at(i) == 0) std::cout << "ZER, ";
		else std::cout << "ONE, ";
	}
	std::cout << " // " << std::hex << unsigned(((bc[1] & (0xFFUL << 56)) >> 56)) << "\n};\n\n";
}

void forkskinny_round_constants(){
	std::cout << "uint64_t forkskinny_precomputed_round_constants[88][7] = {";
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
		std::cout << "}, // " << std::hex << unsigned(lfsr);
	}
	
	std::cout << "\n};";
}

void skinny64_round_constants(){
	std::cout << "uint64_t skinny64_precomputed_round_constants[56][6] = {";
	uint8_t lfsr = 0;
	for (int i = 0; i < 87; ++i) { // 56 = max number of rounds, aka with skinny128-192
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

	forkskinny128_branch_constant();
	
//	forkskinny_round_constants();
	
//	skinny64_round_constants();
	
}