#include "slicing.h"
#include "./fixsliced-keyschedule64.h"

int main() {
	auto blocks = Blocks64();
	srand(1);
	for (unsigned long &value: blocks.values) {
		value = ((long long) random() << 32) | random();
	}
	
	auto sliced = slice(blocks);
	auto zero_tk2 = State64Sliced_t();
	auto zero_tk3 = State64Sliced_t();
	auto keyschedule = precompute_64_key_schedules(&sliced, &zero_tk2, &zero_tk3);
	
	auto appel = 1;
}