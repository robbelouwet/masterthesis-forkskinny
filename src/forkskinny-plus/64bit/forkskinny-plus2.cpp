#include "slicing.h"
#include "./roundfunction/lfsr.h"

int main() {
	auto blocks = Blocks64();
	
	for (uint i = 0; i < 64; ++i) {
		blocks.values[i] = 0xAAAAAAAAAAAAAAAA;
	}
	
	auto sliced = slice(blocks);
	tk2_lfsr_simd(&sliced);
	auto blocks_result = unslice(sliced).values[0];
	
	auto appel = 1;
	return 0;
}