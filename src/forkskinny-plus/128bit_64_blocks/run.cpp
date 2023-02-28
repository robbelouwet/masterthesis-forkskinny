#include "slicing.h"
#include "./fixsliced-keyschedule128.h"

int main() {
	auto blocks = Blocks128();
	srand(1);
	for (int i = 0; i < 64; ++i) {
		if (i % 2 == 0){
			blocks.values[i][0] = 0xAAAAAAAAAAAAAAAA;
			blocks.values[i][1] = 0xAAAAAAAAAAAAAAAA;
		} else {
			blocks.values[i][0] = 0x0;
			blocks.values[i][1] = 0x0;
		}
	}
	
	auto sliced = slice(blocks);
	//auto res = unslice(sliced);
	auto zero_tk2 = State128Sliced_t();
	auto zero_tk3 = State128Sliced_t();
	auto keyschedule = precompute_64_key_schedules(&sliced, &zero_tk2, &zero_tk3);
	
	auto appel = 1;
}