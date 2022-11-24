#include <iostream>
#include "headers/LSFR.h"
#include "slicing/full-state-slicing.h"

static inline uint32_t skinny64_LFSR2(uint32_t x) {
	return ((x << 1) & 0xEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x11111111U);
}

static inline uint64_t skinny64_LFSR2_full(uint64_t x) {
	return ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
}

int main() {
	auto state = State64_t();
	state.llrow = 0xEEEEEEEEEEEEEEEE;
	
	auto t = slice(state.llrow);
	auto sliced = State64Sliced_16_t();
	sliced.state = t;
	auto before = _rdtsc();
	
	char appel[] = "Appel";
	
	auto x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	
	x0 = sliced.slices[0];
	sliced.slices[0] = sliced.slices[3];
	sliced.slices[3] = sliced.slices[2];
	sliced.slices[2] = sliced.slices[1];
	sliced.slices[1] = x0;
	sliced.slices[0] ^= sliced.slices[3];
	auto after = _rdtsc();
	
	auto x = state.llrow;
	auto before1 = _rdtsc();
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	x = ((x << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((x >> 3) ^ (x >> 2)) & 0x1111111111111111U);
	auto after1 = _rdtsc();
	
	
	auto unsliced = unslice(sliced.state);
	assert(x != 0 || unsliced != 0);
	assert(x == unsliced);
	
	std::cout << "Bit sliced LSFR: " << after - before << " cycles, " << sliced.state << "\n";
	std::cout << "Sequential LSFR: " << after1 - before1 << " cycles, " << x;
	
	return 0;
}
