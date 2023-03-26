#ifndef FORKSKINNYPLUS_CONSTANTS_H
#define FORKSKINNYPLUS_CONSTANTS_H

#include "forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "forkskinny128-plus/utils/forkskinny128-datatypes.h"

//<editor-fold desc="forkskinny64 sliced branch constant"
State64Sliced_t const branch_constant64 = {
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

//<editor-fold desc="forkskinny128 sliced branch constant"
State128Sliced_t const branch_constant128 = {
		ZER, ONE, ZER, ZER, ZER, ZER, ZER, ZER,  // 2
		ONE, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 1
		ZER, ZER, ZER, ONE, ZER, ZER, ZER, ZER,  // 8
		ZER, ZER, ONE, ZER, ZER, ZER, ZER, ONE,  // 4
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 20
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 10
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 82
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 41
		ZER, ONE, ZER, ONE, ZER, ZER, ZER, ZER,  // a
		ONE, ZER, ONE, ZER, ZER, ZER, ZER, ZER,  // 5
		ZER, ZER, ZER, ONE, ZER, ONE, ZER, ZER,  // 28
		ZER, ZER, ONE, ZER, ONE, ZER, ZER, ONE,  // 14
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // a2
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 51
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER,  // 88
		ZER, ZER, ZER, ZER, ZER, ZER, ZER, ZER  // 44
};
//</editor-fold>

//<editor-fold desc="88 pre-computed forkskinny sliced round constant lsfrs"
// the first 88 states of the addconstant lfsr containing rc⁰, rc¹, ..., rc⁶
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

#endif //FORKSKINNYPLUS_CONSTANTS_H
