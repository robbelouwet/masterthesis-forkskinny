#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdio>
#include "forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "forkskinny128-plus/utils/forkskinny128-datatypes.h"

#if AVX2_acceleration
#define S256(x) _mm256_set_epi64x(0, 0, x, 0)
auto TWO = _mm256_set_epi64x(-1ULL, 0, 0,0);
#endif

//<editor-fold desc="forkskinny64 sliced branch constant"
// When comparing to the bc in the paper, the cells inside consecutive pairs of cells are swapped with each other to account
// for a swapped order of significance of 2 nibbles within a single byte of plaintext.
// See the README or publication for more info on memory layout
slice_t const branch_constant64[64] = {
		// LSB         MSB
		slice_ZER, slice_ONE, slice_ZER, slice_ZER,  // 2
		slice_ONE, slice_ZER, slice_ZER, slice_ZER,  // 1
		slice_ONE, slice_ZER, slice_ZER, slice_ONE,  // 9
		slice_ZER, slice_ZER, slice_ONE, slice_ZER,  // 4
		slice_ZER, slice_ONE, slice_ONE, slice_ZER,  // 6
		slice_ONE, slice_ONE, slice_ZER, slice_ZER,  // 3
		slice_ZER, slice_ONE, slice_ZER, slice_ONE,  // A
		slice_ONE, slice_ZER, slice_ONE, slice_ONE,  // D
		slice_ONE, slice_ONE, slice_ZER, slice_ONE,  // B
		slice_ONE, slice_ZER, slice_ONE, slice_ZER,  // 5
		slice_ONE, slice_ONE, slice_ONE, slice_ONE,  // F
		slice_ONE, slice_ONE, slice_ONE, slice_ZER,  // 7
		slice_ZER, slice_ZER, slice_ONE, slice_ONE,  // C
		slice_ZER, slice_ONE, slice_ONE, slice_ONE,  // E
		slice_ONE, slice_ZER, slice_ZER, slice_ZER,  //  1
		slice_ZER, slice_ZER, slice_ZER, slice_ONE,  // 8
};
//</editor-fold>

//<editor-fold desc="forkskinny64 segmented branch constant">
State64Sliced_t const segmented_branch_constant64 = {.raw = {
		slice_ZER, slice_ONE, slice_ONE, slice_ZER,
		slice_ONE, slice_ZER, slice_ZER, slice_ZER,
		slice_ZER, slice_ZER, slice_ZER, slice_ONE,
		slice_ZER, slice_ZER, slice_ONE, slice_ZER,
		slice_ZER, slice_ONE, slice_ZER, slice_ONE,
		slice_ONE, slice_ONE, slice_ONE, slice_ZER,
		slice_ONE, slice_ZER, slice_ZER, slice_ONE,
		slice_ZER, slice_ZER, slice_ONE, slice_ONE,
		slice_ONE, slice_ONE, slice_ONE, slice_ONE,
		slice_ONE, slice_ZER, slice_ONE, slice_ONE,
		slice_ZER, slice_ONE, slice_ONE, slice_ONE,
		slice_ONE, slice_ZER, slice_ONE, slice_ZER,
		slice_ZER, slice_ZER, slice_ONE, slice_ZER,
		slice_ZER, slice_ONE, slice_ZER, slice_ZER,
		slice_ONE, slice_ONE, slice_ZER, slice_ZER,
		slice_ONE, slice_ONE, slice_ZER, slice_ONE}
};
//</editor-fold>

//<editor-fold desc="forkskinny128 sliced branch constant"
// the branch constant in forkskinny128 doesn't have 'swapped nibbles' within a byte like forkskinny64 has,
// because cells are already 8-bit and are definable datatypes with a specified order of significance.
// So we don't need to account for this and so, the order of branch constant cells is the same as in the paper
State128Sliced_t const branch_constant128 = {
		.raw = {
				slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,  // 0x1
				slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,  // 0x2
				slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,  // 0x4
				slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER,  // 0x8
				slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER,  // 0x10
				slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER,  // 0x20
				slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER,  // 0x41
				slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE,  // 0x82
				slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,  // 0x5
				slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER,  // 0xa
				slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER,  // 0x14
				slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER,  // 0x28
				slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER,  // 0x51
				slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE,  // 0xa2
				slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER,  // 0x44
				slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE,  // 0x88
		}
};
//</editor-fold>

//<editor-fold desc="88 pre-computed forkskinny sliced round constants"
// the first 88 states of the addconstant lfsr containing {rc⁰, rc¹, ..., rc⁶} each
slice_t forkskinny_precomputed_round_constants[88][7] = {
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,}, // 1
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,}, // 3
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER,}, // 7
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER,}, // 15
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER,}, // 31
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 63
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 126
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 125
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 123
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 119
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE,}, // 111
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 95
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 62
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 124
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 121
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 115
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE,}, // 103
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE,}, // 79
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER,}, // 30
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 61
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 122
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 117
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE,}, // 107
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE,}, // 87
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER,}, // 46
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 92
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 56
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 112
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE,}, // 97
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE,}, // 67
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER,}, // 6
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER,}, // 13
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER,}, // 27
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER,}, // 55
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE,}, // 110
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 93
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 58
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 116
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE,}, // 105
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE,}, // 83
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER,}, // 38
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE,}, // 76
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER,}, // 24
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER,}, // 49
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE,}, // 98
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE,}, // 69
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER,}, // 10
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER,}, // 21
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER,}, // 43
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE,}, // 86
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER,}, // 44
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 88
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER,}, // 48
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE,}, // 96
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE,}, // 65
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER,}, // 2
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER,}, // 5
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER,}, // 11
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER,}, // 23
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER,}, // 47
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 94
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 60
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE,}, // 120
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 113
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE,}, // 99
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE,}, // 71
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER,}, // 14
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER,}, // 29
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER,}, // 59
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE,}, // 118
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE,}, // 109
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 91
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER,}, // 54
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE,}, // 108
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE,}, // 89
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER,}, // 50
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE,}, // 100
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE,}, // 73
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER,}, // 18
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER,}, // 37
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE,}, // 74
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER,}, // 20
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER,}, // 41
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE,}, // 82
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER,}, // 36
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE,}, // 72
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER,}, // 16
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER,}, // 33
};
//</editor-fold>

#if AVX2_acceleration || AVX512_acceleration
//<editor-fold desc="88 pre-computed forkskinny sliced round constants"
// the first 88 states of the addconstant lfsr containing {rc⁰, rc¹, ..., rc⁶} each
// every rc slice_internal is now aligned to the 2nd cell within a segment-row
__m256i const forkskinny_precomputed_segmented_round_constants[88][7] = {
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 1
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 3
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 7
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 15
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 31
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 63
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 126
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 125
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 123
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 119
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 111
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 95
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 62
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 124
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 121
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 115
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 103
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 79
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 30
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 61
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 122
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 117
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 107
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 87
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 46
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 92
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 56
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 112
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 97
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 67
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 6
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 13
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 27
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 55
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 110
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 93
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 58
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 116
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 105
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 83
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 38
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 76
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 24
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 49
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 98
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 69
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 10
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 21
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 43
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 86
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 44
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 88
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 48
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 96
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 65
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 2
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 5
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 11
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 23
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 47
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 94
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 60
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 120
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 113
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 99
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 71
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER),}, // 14
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 29
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 59
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ONE),}, // 118
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 109
		{S256(slice_ONE), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 91
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 54
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 108
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 89
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE), S256(slice_ZER),}, // 50
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ONE),}, // 100
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 73
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 18
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 37
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 74
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 20
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 41
		{S256(slice_ZER), XOR256(S256(slice_ONE), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ONE),}, // 82
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 36
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE),}, // 72
		{S256(slice_ZER), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER), S256(slice_ZER),}, // 16
		{S256(slice_ONE), XOR256(S256(slice_ZER), TWO), S256(slice_ZER), S256(slice_ZER), S256(slice_ZER), S256(slice_ONE), S256(slice_ZER),}, // 33
};
//</editor-fold>
#endif



// <editor-fold desc="PT²"
#define PT64_2(input, output) {   \
output.cells[0x0] = input.cells[0x6];   \
output.cells[0x1] = input.cells[0x0];   \
output.cells[0x2] = input.cells[0x4];   \
output.cells[0x3] = input.cells[0x1];   \
output.cells[0x4] = input.cells[0x7];   \
output.cells[0x5] = input.cells[0x3];   \
output.cells[0x6] = input.cells[0x2];   \
output.cells[0x7] = input.cells[0x5];   \
output.cells[0x8] = input.cells[0xe];   \
output.cells[0x9] = input.cells[0x8];   \
output.cells[0xa] = input.cells[0xc];   \
output.cells[0xb] = input.cells[0x9];   \
output.cells[0xc] = input.cells[0xf];   \
output.cells[0xd] = input.cells[0xb];   \
output.cells[0xe] = input.cells[0xa];   \
output.cells[0xf] = input.cells[0xd];   }
// </editor-fold>

// <editor-fold desc="PT⁴"
#define PT64_4(input, output) {   \
output.cells[0x0] = input.cells[0x2];   \
output.cells[0x1] = input.cells[0x6];   \
output.cells[0x2] = input.cells[0x7];   \
output.cells[0x3] = input.cells[0x0];   \
output.cells[0x4] = input.cells[0x5];   \
output.cells[0x5] = input.cells[0x1];   \
output.cells[0x6] = input.cells[0x4];   \
output.cells[0x7] = input.cells[0x3];   \
output.cells[0x8] = input.cells[0xa];   \
output.cells[0x9] = input.cells[0xe];   \
output.cells[0xa] = input.cells[0xf];   \
output.cells[0xb] = input.cells[0x8];   \
output.cells[0xc] = input.cells[0xd];   \
output.cells[0xd] = input.cells[0x9];   \
output.cells[0xe] = input.cells[0xc];   \
output.cells[0xf] = input.cells[0xb];   }
// </editor-fold>

// <editor-fold desc="PT⁶"
#define PT64_6(input, output) {   \
output.cells[0x0] = input.cells[0x4];   \
output.cells[0x1] = input.cells[0x2];   \
output.cells[0x2] = input.cells[0x5];   \
output.cells[0x3] = input.cells[0x6];   \
output.cells[0x4] = input.cells[0x3];   \
output.cells[0x5] = input.cells[0x0];   \
output.cells[0x6] = input.cells[0x7];   \
output.cells[0x7] = input.cells[0x1];   \
output.cells[0x8] = input.cells[0xc];   \
output.cells[0x9] = input.cells[0xa];   \
output.cells[0xa] = input.cells[0xd];   \
output.cells[0xb] = input.cells[0xe];   \
output.cells[0xc] = input.cells[0xb];   \
output.cells[0xd] = input.cells[0x8];   \
output.cells[0xe] = input.cells[0xf];   \
output.cells[0xf] = input.cells[0x9];   }
// </editor-fold>

// <editor-fold desc="PT⁸"
#define PT64_8(input, output) {   \
output.cells[0x0] = input.cells[0x7];   \
output.cells[0x1] = input.cells[0x4];   \
output.cells[0x2] = input.cells[0x3];   \
output.cells[0x3] = input.cells[0x2];   \
output.cells[0x4] = input.cells[0x1];   \
output.cells[0x5] = input.cells[0x6];   \
output.cells[0x6] = input.cells[0x5];   \
output.cells[0x7] = input.cells[0x0];   \
output.cells[0x8] = input.cells[0xf];   \
output.cells[0x9] = input.cells[0xc];   \
output.cells[0xa] = input.cells[0xb];   \
output.cells[0xb] = input.cells[0xa];   \
output.cells[0xc] = input.cells[0x9];   \
output.cells[0xd] = input.cells[0xe];   \
output.cells[0xe] = input.cells[0xd];   \
output.cells[0xf] = input.cells[0x8];   }
// </editor-fold>

// <editor-fold desc="PT¹⁰"
#define PT64_10(input, output) {   \
output.cells[0x0] = input.cells[0x5];   \
output.cells[0x1] = input.cells[0x7];   \
output.cells[0x2] = input.cells[0x1];   \
output.cells[0x3] = input.cells[0x4];   \
output.cells[0x4] = input.cells[0x0];   \
output.cells[0x5] = input.cells[0x2];   \
output.cells[0x6] = input.cells[0x3];   \
output.cells[0x7] = input.cells[0x6];   \
output.cells[0x8] = input.cells[0xd];   \
output.cells[0x9] = input.cells[0xf];   \
output.cells[0xa] = input.cells[0x9];   \
output.cells[0xb] = input.cells[0xc];   \
output.cells[0xc] = input.cells[0x8];   \
output.cells[0xd] = input.cells[0xa];   \
output.cells[0xe] = input.cells[0xb];   \
output.cells[0xf] = input.cells[0xe];   }
// </editor-fold>

// <editor-fold desc="PT¹²"
#define PT64_12(input, output) {   \
output.cells[0x0] = input.cells[0x3];   \
output.cells[0x1] = input.cells[0x5];   \
output.cells[0x2] = input.cells[0x0];   \
output.cells[0x3] = input.cells[0x7];   \
output.cells[0x4] = input.cells[0x6];   \
output.cells[0x5] = input.cells[0x4];   \
output.cells[0x6] = input.cells[0x1];   \
output.cells[0x7] = input.cells[0x2];   \
output.cells[0x8] = input.cells[0xb];   \
output.cells[0x9] = input.cells[0xd];   \
output.cells[0xa] = input.cells[0x8];   \
output.cells[0xb] = input.cells[0xf];   \
output.cells[0xc] = input.cells[0xe];   \
output.cells[0xd] = input.cells[0xc];   \
output.cells[0xe] = input.cells[0x9];   \
output.cells[0xf] = input.cells[0xa];   }
// </editor-fold>

// <editor-fold desc="PT¹⁴"
#define PT64_14(input, output) {   \
output.cells[0x0] = input.cells[0x1];   \
output.cells[0x1] = input.cells[0x3];   \
output.cells[0x2] = input.cells[0x6];   \
output.cells[0x3] = input.cells[0x5];   \
output.cells[0x4] = input.cells[0x2];   \
output.cells[0x5] = input.cells[0x7];   \
output.cells[0x6] = input.cells[0x0];   \
output.cells[0x7] = input.cells[0x4];   \
output.cells[0x8] = input.cells[0x9];   \
output.cells[0x9] = input.cells[0xb];   \
output.cells[0xa] = input.cells[0xe];   \
output.cells[0xb] = input.cells[0xd];   \
output.cells[0xc] = input.cells[0xa];   \
output.cells[0xd] = input.cells[0xf];   \
output.cells[0xe] = input.cells[0x8];   \
output.cells[0xf] = input.cells[0xc];   }
// </editor-fold>

#define PT128_2(input, output){  \
output.cells[0x0] = input.cells[0x1];   \
output.cells[0x1] = input.cells[0x7];   \
output.cells[0x2] = input.cells[0x0];   \
output.cells[0x3] = input.cells[0x5];   \
output.cells[0x4] = input.cells[0x2];   \
output.cells[0x5] = input.cells[0x6];   \
output.cells[0x6] = input.cells[0x4];   \
output.cells[0x7] = input.cells[0x3];   \
output.cells[0x8] = input.cells[0x9];   \
output.cells[0x9] = input.cells[0xf];   \
output.cells[0xa] = input.cells[0x8];   \
output.cells[0xb] = input.cells[0xd];   \
output.cells[0xc] = input.cells[0xa];   \
output.cells[0xd] = input.cells[0xe];   \
output.cells[0xe] = input.cells[0xc];   \
output.cells[0xf] = input.cells[0xb];   }


#define PT128_4(input, output){  \
output.cells[0x0] = input.cells[0x7];   \
output.cells[0x1] = input.cells[0x3];   \
output.cells[0x2] = input.cells[0x1];   \
output.cells[0x3] = input.cells[0x6];   \
output.cells[0x4] = input.cells[0x0];   \
output.cells[0x5] = input.cells[0x4];   \
output.cells[0x6] = input.cells[0x2];   \
output.cells[0x7] = input.cells[0x5];   \
output.cells[0x8] = input.cells[0xf];   \
output.cells[0x9] = input.cells[0xb];   \
output.cells[0xa] = input.cells[0x9];   \
output.cells[0xb] = input.cells[0xe];   \
output.cells[0xc] = input.cells[0x8];   \
output.cells[0xd] = input.cells[0xc];   \
output.cells[0xe] = input.cells[0xa];   \
output.cells[0xf] = input.cells[0xd];   }


#define PT128_6(input, output){  \
output.cells[0x0] = input.cells[0x3];   \
output.cells[0x1] = input.cells[0x5];   \
output.cells[0x2] = input.cells[0x7];   \
output.cells[0x3] = input.cells[0x4];   \
output.cells[0x4] = input.cells[0x1];   \
output.cells[0x5] = input.cells[0x2];   \
output.cells[0x6] = input.cells[0x0];   \
output.cells[0x7] = input.cells[0x6];   \
output.cells[0x8] = input.cells[0xb];   \
output.cells[0x9] = input.cells[0xd];   \
output.cells[0xa] = input.cells[0xf];   \
output.cells[0xb] = input.cells[0xc];   \
output.cells[0xc] = input.cells[0x9];   \
output.cells[0xd] = input.cells[0xa];   \
output.cells[0xe] = input.cells[0x8];   \
output.cells[0xf] = input.cells[0xe];   }


#define PT128_8(input, output){  \
output.cells[0x0] = input.cells[0x5];   \
output.cells[0x1] = input.cells[0x6];   \
output.cells[0x2] = input.cells[0x3];   \
output.cells[0x3] = input.cells[0x2];   \
output.cells[0x4] = input.cells[0x7];   \
output.cells[0x5] = input.cells[0x0];   \
output.cells[0x6] = input.cells[0x1];   \
output.cells[0x7] = input.cells[0x4];   \
output.cells[0x8] = input.cells[0xd];   \
output.cells[0x9] = input.cells[0xe];   \
output.cells[0xa] = input.cells[0xb];   \
output.cells[0xb] = input.cells[0xa];   \
output.cells[0xc] = input.cells[0xf];   \
output.cells[0xd] = input.cells[0x8];   \
output.cells[0xe] = input.cells[0x9];   \
output.cells[0xf] = input.cells[0xc];   }


#define PT128_10(input, output){  \
output.cells[0x0] = input.cells[0x6];   \
output.cells[0x1] = input.cells[0x4];   \
output.cells[0x2] = input.cells[0x5];   \
output.cells[0x3] = input.cells[0x0];   \
output.cells[0x4] = input.cells[0x3];   \
output.cells[0x5] = input.cells[0x1];   \
output.cells[0x6] = input.cells[0x7];   \
output.cells[0x7] = input.cells[0x2];   \
output.cells[0x8] = input.cells[0xe];   \
output.cells[0x9] = input.cells[0xc];   \
output.cells[0xa] = input.cells[0xd];   \
output.cells[0xb] = input.cells[0x8];   \
output.cells[0xc] = input.cells[0xb];   \
output.cells[0xd] = input.cells[0x9];   \
output.cells[0xe] = input.cells[0xf];   \
output.cells[0xf] = input.cells[0xa];   }


#define PT128_12(input, output){  \
output.cells[0x0] = input.cells[0x4];   \
output.cells[0x1] = input.cells[0x2];   \
output.cells[0x2] = input.cells[0x6];   \
output.cells[0x3] = input.cells[0x1];   \
output.cells[0x4] = input.cells[0x5];   \
output.cells[0x5] = input.cells[0x7];   \
output.cells[0x6] = input.cells[0x3];   \
output.cells[0x7] = input.cells[0x0];   \
output.cells[0x8] = input.cells[0xc];   \
output.cells[0x9] = input.cells[0xa];   \
output.cells[0xa] = input.cells[0xe];   \
output.cells[0xb] = input.cells[0x9];   \
output.cells[0xc] = input.cells[0xd];   \
output.cells[0xd] = input.cells[0xf];   \
output.cells[0xe] = input.cells[0xb];   \
output.cells[0xf] = input.cells[0x8];   }


#define PT128_14(input, output){  \
output.cells[0x0] = input.cells[0x2];   \
output.cells[0x1] = input.cells[0x0];   \
output.cells[0x2] = input.cells[0x4];   \
output.cells[0x3] = input.cells[0x7];   \
output.cells[0x4] = input.cells[0x6];   \
output.cells[0x5] = input.cells[0x3];   \
output.cells[0x6] = input.cells[0x5];   \
output.cells[0x7] = input.cells[0x1];   \
output.cells[0x8] = input.cells[0xa];   \
output.cells[0x9] = input.cells[0x8];   \
output.cells[0xa] = input.cells[0xc];   \
output.cells[0xb] = input.cells[0xf];   \
output.cells[0xc] = input.cells[0xe];   \
output.cells[0xd] = input.cells[0xb];   \
output.cells[0xe] = input.cells[0xd];   \
output.cells[0xf] = input.cells[0x9];   }

uint64_t const bit_masks[64] = {
		0x1,
		0x2,
		0x4,
		0x8,
		0x10,
		0x20,
		0x40,
		0x80,
		0x100,
		0x200,
		0x400,
		0x800,
		0x1000,
		0x2000,
		0x4000,
		0x8000,
		0x10000,
		0x20000,
		0x40000,
		0x80000,
		0x100000,
		0x200000,
		0x400000,
		0x800000,
		0x1000000,
		0x2000000,
		0x4000000,
		0x8000000,
		0x10000000,
		0x20000000,
		0x40000000,
		0x80000000,
		0x100000000,
		0x200000000,
		0x400000000,
		0x800000000,
		0x1000000000,
		0x2000000000,
		0x4000000000,
		0x8000000000,
		0x10000000000,
		0x20000000000,
		0x40000000000,
		0x80000000000,
		0x100000000000,
		0x200000000000,
		0x400000000000,
		0x800000000000,
		0x1000000000000,
		0x2000000000000,
		0x4000000000000,
		0x8000000000000,
		0x10000000000000,
		0x20000000000000,
		0x40000000000000,
		0x80000000000000,
		0x100000000000000,
		0x200000000000000,
		0x400000000000000,
		0x800000000000000,
		0x1000000000000000,
		0x2000000000000000,
		0x4000000000000000,
		0x8000000000000000,
};
// leave empty arrays to make it easier to retrieve lanes by index
uint8_t const fixsliced_pt64[16][16] = {
		{},
		{},
		{6, 0, 4, 1, 7, 3, 2, 5, 14, 8,  12, 9,  15, 11, 10, 13},
		{},
		{2, 6, 7, 0, 5, 1, 4, 3, 10, 14, 15, 8,  13, 9,  12, 11},
		{},
		{4, 2, 5, 6, 3, 0, 7, 1, 12, 10, 13, 14, 11, 8,  15, 9},
		{},
		{7, 4, 3, 2, 1, 6, 5, 0, 15, 12, 11, 10, 9,  14, 13, 8},
		{},
		{5, 7, 1, 4, 0, 2, 3, 6, 13, 15, 9,  12, 8,  10, 11, 14},
		{},
		{3, 5, 0, 7, 6, 4, 1, 2, 11, 13, 8,  15, 14, 12, 9,  10},
		{},
		{1, 3, 6, 5, 2, 7, 0, 4, 9,  11, 14, 13, 10, 15, 8,  12}
};

// fs128 doesn't have nibble swapping, so the PT isn't adjusted like with fixsliced_pt64
uint8_t const fixsliced_pt128[16][16] =
		{
				{},
				{},
				{1,  7,  0,  5,  2,  6,  4,  3,  9,  15, 8,  13, 10, 14, 12, 11},
				{},
				{7,  3,  1,  6,  0,  4,  2,  5,  15, 11, 9,  14, 8,  12, 10, 13},
				{},
				{3,  5,  7,  4,  1,  2,  0,  6,  11, 13, 15, 12, 9,  10, 8,  14},
				{},
				{5,  6,  3,  2,  7,  0,  1,  4,  13, 14, 11, 10, 15, 8,  9,  12},
				{},
				{6,  4,  5,  0,  3,  1,  7,  2,  14, 12, 13, 8,  11, 9,  15, 10},
				{},
				{4,  2,  6,  1,  5,  7,  3,  0,  12, 10, 14, 9,  13, 15, 11, 8},
				{},
				{2,  0,  4,  7,  6,  3,  5,  1,  10, 8,  12, 15, 14, 11, 13, 9},
				{}
		};

#endif //CONSTANTS_H
