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
		ZER, ONE, ZER, ZER,  // 2
		ONE, ZER, ZER, ZER,  // 1
		ONE, ZER, ZER, ONE,  // 9
		ZER, ZER, ONE, ZER,  // 4
		ZER, ONE, ONE, ZER,  // 6
		ONE, ONE, ZER, ZER,  // 3
		ZER, ONE, ZER, ONE,  // A
		ONE, ZER, ONE, ONE,  // D
		ONE, ONE, ZER, ONE,  // B
		ONE, ZER, ONE, ZER,  // 5
		ONE, ONE, ONE, ONE,  // F
		ONE, ONE, ONE, ZER,  // 7
		ZER, ZER, ONE, ONE,  // C
		ZER, ONE, ONE, ONE,  // E
		ONE, ZER, ZER, ZER,  //  1
		ZER, ZER, ZER, ONE,  // 8
};
//</editor-fold>

//<editor-fold desc="forkskinny64 segmented branch constant">
slice_t const segmented_branch_constant64[64] = {
		ZER, ONE, ONE, ZER,
		ONE, ZER, ZER, ZER,
		ZER, ZER, ZER, ONE,
		ZER, ZER, ONE, ZER,
		ZER, ONE, ZER, ONE,
		ONE, ONE, ONE, ZER,
		ONE, ZER, ZER, ONE,
		ZER, ZER, ONE, ONE,
		ONE, ONE, ONE, ONE,
		ONE, ZER, ONE, ONE,
		ZER, ONE, ONE, ONE,
		ONE, ZER, ONE, ZER,
		ZER, ZER, ONE, ZER,
		ZER, ONE, ZER, ZER,
		ONE, ONE, ZER, ZER,
		ONE, ONE, ZER, ONE,
};
//</editor-fold>

//<editor-fold desc="forkskinny128 sliced branch constant"
// the branch constant in forkskinny128 doesn't have 'swapped nibbles' within a byte like forkskinny64 has,
// because cells are already 8-bit and are definable datatypes with a specified order of significance.
// So we don't need to account for this and so, the order of branch constant cells is the same as in the paper
slice_t const branch_constant128[128] = {
		ONE, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 0x1
		ZER, ONE, ZER, ZER, ZER, ZER, ZER, ZER,  // 0x2
		ZER, ZER, ONE, ZER, ZER, ZER, ZER, ZER,  // 0x4
		ZER, ZER, ZER, ONE, ZER, ZER, ZER, ZER,  // 0x8
		ZER, ZER, ZER, ZER, ONE, ZER, ZER, ZER,  // 0x10
		ZER, ZER, ZER, ZER, ZER, ONE, ZER, ZER,  // 0x20
		ONE, ZER, ZER, ZER, ZER, ZER, ONE, ZER,  // 0x41
		ZER, ONE, ZER, ZER, ZER, ZER, ZER, ONE,  // 0x82
		ONE, ZER, ONE, ZER, ZER, ZER, ZER, ZER,  // 0x5
		ZER, ONE, ZER, ONE, ZER, ZER, ZER, ZER,  // 0xa
		ZER, ZER, ONE, ZER, ONE, ZER, ZER, ZER,  // 0x14
		ZER, ZER, ZER, ONE, ZER, ONE, ZER, ZER,  // 0x28
		ONE, ZER, ZER, ZER, ONE, ZER, ONE, ZER,  // 0x51
		ZER, ONE, ZER, ZER, ZER, ONE, ZER, ONE,  // 0xa2
		ZER, ZER, ONE, ZER, ZER, ZER, ONE, ZER,  // 0x44
		ZER, ZER, ZER, ONE, ZER, ZER, ZER, ONE,  // 0x88
};
//</editor-fold>

//<editor-fold desc="88 pre-computed forkskinny sliced round constants"
// the first 88 states of the addconstant lfsr containing {rc⁰, rc¹, ..., rc⁶} each
slice_t forkskinny_precomputed_round_constants[88][7] = {
		{ONE, ZER, ZER, ZER, ZER, ZER, ZER, }, // 1
		{ONE, ONE, ZER, ZER, ZER, ZER, ZER, }, // 3
		{ONE, ONE, ONE, ZER, ZER, ZER, ZER, }, // 7
		{ONE, ONE, ONE, ONE, ZER, ZER, ZER, }, // 15
		{ONE, ONE, ONE, ONE, ONE, ZER, ZER, }, // 31
		{ONE, ONE, ONE, ONE, ONE, ONE, ZER, }, // 63
		{ZER, ONE, ONE, ONE, ONE, ONE, ONE, }, // 126
		{ONE, ZER, ONE, ONE, ONE, ONE, ONE, }, // 125
		{ONE, ONE, ZER, ONE, ONE, ONE, ONE, }, // 123
		{ONE, ONE, ONE, ZER, ONE, ONE, ONE, }, // 119
		{ONE, ONE, ONE, ONE, ZER, ONE, ONE, }, // 111
		{ONE, ONE, ONE, ONE, ONE, ZER, ONE, }, // 95
		{ZER, ONE, ONE, ONE, ONE, ONE, ZER, }, // 62
		{ZER, ZER, ONE, ONE, ONE, ONE, ONE, }, // 124
		{ONE, ZER, ZER, ONE, ONE, ONE, ONE, }, // 121
		{ONE, ONE, ZER, ZER, ONE, ONE, ONE, }, // 115
		{ONE, ONE, ONE, ZER, ZER, ONE, ONE, }, // 103
		{ONE, ONE, ONE, ONE, ZER, ZER, ONE, }, // 79
		{ZER, ONE, ONE, ONE, ONE, ZER, ZER, }, // 30
		{ONE, ZER, ONE, ONE, ONE, ONE, ZER, }, // 61
		{ZER, ONE, ZER, ONE, ONE, ONE, ONE, }, // 122
		{ONE, ZER, ONE, ZER, ONE, ONE, ONE, }, // 117
		{ONE, ONE, ZER, ONE, ZER, ONE, ONE, }, // 107
		{ONE, ONE, ONE, ZER, ONE, ZER, ONE, }, // 87
		{ZER, ONE, ONE, ONE, ZER, ONE, ZER, }, // 46
		{ZER, ZER, ONE, ONE, ONE, ZER, ONE, }, // 92
		{ZER, ZER, ZER, ONE, ONE, ONE, ZER, }, // 56
		{ZER, ZER, ZER, ZER, ONE, ONE, ONE, }, // 112
		{ONE, ZER, ZER, ZER, ZER, ONE, ONE, }, // 97
		{ONE, ONE, ZER, ZER, ZER, ZER, ONE, }, // 67
		{ZER, ONE, ONE, ZER, ZER, ZER, ZER, }, // 6
		{ONE, ZER, ONE, ONE, ZER, ZER, ZER, }, // 13
		{ONE, ONE, ZER, ONE, ONE, ZER, ZER, }, // 27
		{ONE, ONE, ONE, ZER, ONE, ONE, ZER, }, // 55
		{ZER, ONE, ONE, ONE, ZER, ONE, ONE, }, // 110
		{ONE, ZER, ONE, ONE, ONE, ZER, ONE, }, // 93
		{ZER, ONE, ZER, ONE, ONE, ONE, ZER, }, // 58
		{ZER, ZER, ONE, ZER, ONE, ONE, ONE, }, // 116
		{ONE, ZER, ZER, ONE, ZER, ONE, ONE, }, // 105
		{ONE, ONE, ZER, ZER, ONE, ZER, ONE, }, // 83
		{ZER, ONE, ONE, ZER, ZER, ONE, ZER, }, // 38
		{ZER, ZER, ONE, ONE, ZER, ZER, ONE, }, // 76
		{ZER, ZER, ZER, ONE, ONE, ZER, ZER, }, // 24
		{ONE, ZER, ZER, ZER, ONE, ONE, ZER, }, // 49
		{ZER, ONE, ZER, ZER, ZER, ONE, ONE, }, // 98
		{ONE, ZER, ONE, ZER, ZER, ZER, ONE, }, // 69
		{ZER, ONE, ZER, ONE, ZER, ZER, ZER, }, // 10
		{ONE, ZER, ONE, ZER, ONE, ZER, ZER, }, // 21
		{ONE, ONE, ZER, ONE, ZER, ONE, ZER, }, // 43
		{ZER, ONE, ONE, ZER, ONE, ZER, ONE, }, // 86
		{ZER, ZER, ONE, ONE, ZER, ONE, ZER, }, // 44
		{ZER, ZER, ZER, ONE, ONE, ZER, ONE, }, // 88
		{ZER, ZER, ZER, ZER, ONE, ONE, ZER, }, // 48
		{ZER, ZER, ZER, ZER, ZER, ONE, ONE, }, // 96
		{ONE, ZER, ZER, ZER, ZER, ZER, ONE, }, // 65
		{ZER, ONE, ZER, ZER, ZER, ZER, ZER, }, // 2
		{ONE, ZER, ONE, ZER, ZER, ZER, ZER, }, // 5
		{ONE, ONE, ZER, ONE, ZER, ZER, ZER, }, // 11
		{ONE, ONE, ONE, ZER, ONE, ZER, ZER, }, // 23
		{ONE, ONE, ONE, ONE, ZER, ONE, ZER, }, // 47
		{ZER, ONE, ONE, ONE, ONE, ZER, ONE, }, // 94
		{ZER, ZER, ONE, ONE, ONE, ONE, ZER, }, // 60
		{ZER, ZER, ZER, ONE, ONE, ONE, ONE, }, // 120
		{ONE, ZER, ZER, ZER, ONE, ONE, ONE, }, // 113
		{ONE, ONE, ZER, ZER, ZER, ONE, ONE, }, // 99
		{ONE, ONE, ONE, ZER, ZER, ZER, ONE, }, // 71
		{ZER, ONE, ONE, ONE, ZER, ZER, ZER, }, // 14
		{ONE, ZER, ONE, ONE, ONE, ZER, ZER, }, // 29
		{ONE, ONE, ZER, ONE, ONE, ONE, ZER, }, // 59
		{ZER, ONE, ONE, ZER, ONE, ONE, ONE, }, // 118
		{ONE, ZER, ONE, ONE, ZER, ONE, ONE, }, // 109
		{ONE, ONE, ZER, ONE, ONE, ZER, ONE, }, // 91
		{ZER, ONE, ONE, ZER, ONE, ONE, ZER, }, // 54
		{ZER, ZER, ONE, ONE, ZER, ONE, ONE, }, // 108
		{ONE, ZER, ZER, ONE, ONE, ZER, ONE, }, // 89
		{ZER, ONE, ZER, ZER, ONE, ONE, ZER, }, // 50
		{ZER, ZER, ONE, ZER, ZER, ONE, ONE, }, // 100
		{ONE, ZER, ZER, ONE, ZER, ZER, ONE, }, // 73
		{ZER, ONE, ZER, ZER, ONE, ZER, ZER, }, // 18
		{ONE, ZER, ONE, ZER, ZER, ONE, ZER, }, // 37
		{ZER, ONE, ZER, ONE, ZER, ZER, ONE, }, // 74
		{ZER, ZER, ONE, ZER, ONE, ZER, ZER, }, // 20
		{ONE, ZER, ZER, ONE, ZER, ONE, ZER, }, // 41
		{ZER, ONE, ZER, ZER, ONE, ZER, ONE, }, // 82
		{ZER, ZER, ONE, ZER, ZER, ONE, ZER, }, // 36
		{ZER, ZER, ZER, ONE, ZER, ZER, ONE, }, // 72
		{ZER, ZER, ZER, ZER, ONE, ZER, ZER, }, // 16
		{ONE, ZER, ZER, ZER, ZER, ONE, ZER, }, // 33
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
