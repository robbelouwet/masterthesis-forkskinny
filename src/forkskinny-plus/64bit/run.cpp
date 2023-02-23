#include "slicing.h"
#include "keyschedule/keyschedule64.h"

int main() {
	auto blocks = Blocks64();
	srand(1);
	for (int i = 0; i < 64; ++i) {
		if (i == 0)blocks.values[i] = 0xAAAAAAAAAAAAAAAA;
		else blocks.values[i] = 0x0;
	}
	
	auto sliced = slice(blocks);
	auto zero_tk2 = State64Sliced_t();
	auto zero_tk3 = State64Sliced_t();
	auto keyschedule = precompute_64_key_schedules(sliced, zero_tk2, zero_tk3);
	
	
}