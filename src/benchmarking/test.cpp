#include "../SIMD_utils.h"
#include "../AEAD/PAEF.h"

int main() {
//	auto res128 = mm_rotr_si128(
//			_mm_set_epi64x(
//					0xFFFFFFFFFFFFFFBFULL,
//					0xFFFFFFFFFFFFFFAFULL),
//			8 + 64);
//
//	auto res256 = mm256_rotr_si256(
//			_mm256_set_epi64x(
//					0x0000000000000000,
//					0x0000000000000000,
//					0x0000000000000000,
//					0x0000000000000001),
//			256-64);
//
////	auto res512 = mm512_rotr_si512(
////			_mm512_set_epi64(
////					0xFFFFFFFFFFFFFFFFULL,
////					0xFFFFFFFFFFFFFFEFULL,
////					0xFFFFFFFFFFFFFFDFULL,
////					0xFFFFFFFFFFFFFFCFULL,
////					0xFFFFFFFFFFFFFFBFULL,
////					0xFFFFFFFFFFFFFFAFULL,
////					0xFFFFFFFFFFFFFF9FULL,
////					0xFFFFFFFFFFFFFF8FULL),
////			64 + 4);
//
//	int appel = 1;

	paef_forkskinny64_192_encrypt();
}