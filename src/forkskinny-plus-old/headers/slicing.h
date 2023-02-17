#ifndef FORKSKINNYPLUS_SLICING_H
#define FORKSKINNYPLUS_SLICING_H

#include <cstdint>
#include "forkskinny64-plus.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline State64Sliced_t slice_significance(const uint64_t p_blocks[64], uint8_t significance) {
	uint64_t mask = 1 << significance;
	uint64_t slice = 0;
	
	for (int i = 0; i < 64; ++i) {
		slice |= (p_blocks[i] & mask) >> significance << i;
	}
}

#endif //FORKSKINNYPLUS_SLICING_H
