#include "slicing.h"
#include "./roundfunction/lfsr.h"
#include "./fixsliced-keyschedule.h"

int main() {
	auto blocks = Blocks64();
	srand(1);
	for (unsigned long &value: blocks.values) {
		value = ((long long) random() << 32) | random();
	}
	
	auto sliced = slice(blocks);
//	tk2_lfsr_simd(&sliced);
//	auto blocks_result = unslice(sliced).values[0];
//
//	auto appel = 1;
//	return 0;
	
	auto zero_tk2 = State64Sliced_t();
	auto zero_tk3 = State64Sliced_t();
	auto keyschedule = precompute_64_key_schedules(&sliced, &zero_tk2, &zero_tk3);
	
	auto appel = 1;
}