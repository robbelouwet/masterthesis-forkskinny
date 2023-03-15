#ifndef FORKSKINNYPLUS_SLICING_H
#define FORKSKINNYPLUS_SLICING_H

#include <cstdint>
#include <x86intrin.h>
#include "skinny64_datatypes.h"

/**
 *
 * @param p_blocks
 * @param significance LSB = 0, MSB = 63
 * @return
 */
static inline Slice64_t slice_significance(const Blocks64_t blocks, uint8_t significance) {
	uint64_t mask = 1ULL << significance;
	auto slice = Slice64_t();
	
	#if slice_size == 256
	
	
	#elif slice_size == 512
	
	#else
	
	#endif
	
	return slice;
}

static inline State64Sliced_t *slice(Blocks64_t blocks) {
	// make it 32-byte aligned for fast memory access
	auto *result = (State64Sliced_t *) aligned_alloc(32, sizeof(State64Sliced_t));
	
	// align the blocks
	for (uint i = 1; i < slice_size; ++i) {
		#if SKINNY_64BIT
		blocks.values[i].raw = _rotr(blocks.values[i].raw, i);
		#else
		// TODO
		#endif
	}
	
	for (int i = 0; i < 64; ++i) {
		auto slice = Slice64_t();
		
		#if slice_size == 64
		uint64_t mask = 0x8000000000000000ULL >> i;
		int j = 0;
		for (; j < slice_size; ++j) {
			slice |= (blocks.values[i].raw & mask);
			mask = _rotr(mask, 1);
		}
		result->raw[j].value = _rotl(slice, i);
		#elif slice_size == 256
		uint64_t mask = 0x8000000000000000ULL >> i;
		int j = 0;
		for (; j < 64; ++j) {
			slice.segments[0] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		for (; j < 128; ++j) {
			slice.segments[1] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		for (; j < 192; ++j) {
			slice.segments[2] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		for (; j < 256; ++j) {
			slice.segments[3] |= blocks.values[i].raw & mask;
			mask = _rotr(mask, 1);
		}
		
		#else
		#endif
		
	}
	
	return result;
}

/**
 *
 * @param slice
 * @param blocks
 * @param sb_index the index of the slice_t, what 'significance' are we talking about w.r.t. the slice.
 * 					E.g. the very first slice contains the *least* significant bits of 64 states
 */
static inline void unslice_significance(const Slice64_t slice, Blocks64_t *blocks, uint8_t sb_index) {
#if slice_size == 256

#elif slice_size == 512

#else

#endif
	
}

static inline Blocks64_t unslice(State64Sliced_t state) {
	Blocks64_t unsliced = Blocks64_t();
	for (int i = 0; i < 64; ++i) {
		unslice_significance(state.raw[i], &unsliced, i);
	}
	
	return unsliced;
}

#endif //FORKSKINNYPLUS_SLICING_H
