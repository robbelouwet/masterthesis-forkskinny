#include "../SIMD_utils.h"

int main() {
	auto res256 = mm256_rotr_si256(
			_mm256_set_epi64x(
					0xFFFFFFFFFFFFFFDFULL,
					0xFFFFFFFFFFFFFFCFULL,
					0xFFFFFFFFFFFFFFBFULL,
					0xFFFFFFFFFFFFFFAFULL),
			4+64);
	
	auto res128 = mm_rotr_si128(
			_mm_set_epi64x(
					0xFFFFFFFFFFFFFFBFULL,
					0xFFFFFFFFFFFFFFAFULL),
			68);
	int appel = 1;
}