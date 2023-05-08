#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdio>
#include "forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "forkskinny128-plus/utils/forkskinny128-datatypes.h"

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
State64Sliced_t const segmented_branch_constant64 = {
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
		slice_ONE, slice_ONE, slice_ZER, slice_ONE,
};
//</editor-fold>

//<editor-fold desc="forkskinny128 sliced branch constant"
// the branch constant in forkskinny128 doesn't have 'swapped nibbles' within a byte like forkskinny64 has,
// because cells are already 8-bit and are definable datatypes with a specified order of significance.
// So we don't need to account for this and so, the order of branch constant cells is the same as in the paper
slice_t const branch_constant128[128] = {
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
};
//</editor-fold>

//<editor-fold desc="88 pre-computed forkskinny sliced round constants"
// the first 88 states of the addconstant lfsr containing {rc⁰, rc¹, ..., rc⁶} each
slice_t forkskinny_precomputed_round_constants[88][7] = {
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, }, // 1
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, }, // 3
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, }, // 7
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, }, // 15
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, }, // 31
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 63
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 126
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 125
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 123
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 119
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, }, // 111
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 95
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 62
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 124
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 121
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 115
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, }, // 103
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, }, // 79
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, }, // 30
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 61
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 122
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 117
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, }, // 107
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, }, // 87
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, }, // 46
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 92
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 56
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 112
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, }, // 97
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, }, // 67
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, }, // 6
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, }, // 13
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, }, // 27
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, }, // 55
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, }, // 110
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 93
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 58
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 116
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ONE, }, // 105
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, }, // 83
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, }, // 38
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ONE, }, // 76
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ZER, }, // 24
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, }, // 49
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, }, // 98
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, }, // 69
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, }, // 10
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, }, // 21
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, }, // 43
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ONE, }, // 86
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, }, // 44
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 88
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, }, // 48
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, }, // 96
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, }, // 65
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ZER, }, // 2
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, }, // 5
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ZER, }, // 11
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, }, // 23
		{slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ZER, }, // 47
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 94
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 60
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ONE, }, // 120
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 113
		{slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ONE, }, // 99
		{slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ONE, }, // 71
		{slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, slice_ZER, }, // 14
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, slice_ZER, }, // 29
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, slice_ZER, }, // 59
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ONE, }, // 118
		{slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, }, // 109
		{slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 91
		{slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, slice_ZER, }, // 54
		{slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, slice_ONE, }, // 108
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, slice_ONE, }, // 89
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, slice_ZER, }, // 50
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ONE, }, // 100
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, }, // 73
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, }, // 18
		{slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, }, // 37
		{slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, }, // 74
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, slice_ZER, }, // 20
		{slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, slice_ZER, }, // 41
		{slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ONE, }, // 82
		{slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, slice_ZER, }, // 36
		{slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, slice_ONE, }, // 72
		{slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, slice_ZER, }, // 16
		{slice_ONE, slice_ZER, slice_ZER, slice_ZER, slice_ZER, slice_ONE, slice_ZER, }, // 33
};
//</editor-fold>

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

#endif //CONSTANTS_H
