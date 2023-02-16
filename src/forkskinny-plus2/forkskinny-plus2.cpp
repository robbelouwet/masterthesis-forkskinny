#include "slicing.h"

int main() {
	auto blocks = Blocks();
	
	for (uint i = 0; i < 64; ++i) {
		if (i % 2 == 0)
			blocks.values[i] = 0xAAAAAAAAAAAAAAAA;
		else blocks.values[i] = 0x5555555555555555;
	}
	
	auto sliced = slice(blocks);
	auto blocks_result = unslice(sliced);
	
	auto appel = 1;
}