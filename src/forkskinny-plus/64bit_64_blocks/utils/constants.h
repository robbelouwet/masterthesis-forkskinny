#ifndef FORKSKINNYPLUS_CONSTANTS_H
#define FORKSKINNYPLUS_CONSTANTS_H

#include <cstdint>
#include "skinny64_datatypes.h"

#define ONE 0xffffffffffffffff
#define ZER 0x0

//<editor-fold desc="forkskinny64 sliced branch constant"
State64Sliced_t const branch_constant = {
		ONE, ZER, ZER, ZER,  // 1
		ZER, ONE, ZER, ZER,  // 2
		ZER, ZER, ONE, ZER,  // 4
		ONE, ZER, ZER, ONE,  // 9
		ONE, ONE, ZER, ZER,  // 3
		ZER, ONE, ONE, ZER,  // 6
		ONE, ZER, ONE, ONE,  // 13
		ZER, ONE, ZER, ONE,  // 10
		ONE, ZER, ONE, ZER,  // 5
		ONE, ONE, ZER, ONE,  // 11
		ONE, ONE, ONE, ZER,  // 7
		ONE, ONE, ONE, ONE,  // 15
		ZER, ONE, ONE, ONE,  // 14
		ZER, ZER, ONE, ONE,  // 12
		ZER, ZER, ZER, ONE,  // 8
		ONE, ZER, ZER, ZER, //  1
};
//</editor-fold>

//<editor-fold desc="pre-computed forkskinny64 sliced round constants"
uint64_t const forkskinny64_precomputed_round_constants[FORKSKINNY_ROUNDS_BEFORE + FORKSKINNY_ROUNDS_AFTER][7] = {
		{ONE, ZER, ZER, ZER, ZER, ZER, ZER}, //1
		{ONE, ONE, ZER, ZER, ZER, ZER, ZER}, //3
		{ONE, ONE, ONE, ZER, ZER, ZER, ZER}, //7
		{ONE, ONE, ONE, ONE, ZER, ZER, ZER}, //15
		{ONE, ONE, ONE, ONE, ONE, ZER, ZER}, //31
		{ONE, ONE, ONE, ONE, ONE, ONE, ZER}, //63
		{ZER, ONE, ONE, ONE, ONE, ONE, ONE}, //126
		{ONE, ZER, ONE, ONE, ONE, ONE, ONE}, //125
		{ONE, ONE, ZER, ONE, ONE, ONE, ONE}, //123
		{ONE, ONE, ONE, ZER, ONE, ONE, ONE}, //119
		{ONE, ONE, ONE, ONE, ZER, ONE, ONE}, //111
		{ONE, ONE, ONE, ONE, ONE, ZER, ONE}, //95
		{ZER, ONE, ONE, ONE, ONE, ONE, ZER}, //62
		{ZER, ZER, ONE, ONE, ONE, ONE, ONE}, //124
		{ONE, ZER, ZER, ONE, ONE, ONE, ONE}, //121
		{ONE, ONE, ZER, ZER, ONE, ONE, ONE}, //115
		{ONE, ONE, ONE, ZER, ZER, ONE, ONE}, //103
		{ONE, ONE, ONE, ONE, ZER, ZER, ONE}, //79
		{ZER, ONE, ONE, ONE, ONE, ZER, ZER}, //30
		{ONE, ZER, ONE, ONE, ONE, ONE, ZER}, //61
		{ZER, ONE, ZER, ONE, ONE, ONE, ONE}, //122
		{ONE, ZER, ONE, ZER, ONE, ONE, ONE}, //117
		{ONE, ONE, ZER, ONE, ZER, ONE, ONE}, //107
		{ONE, ONE, ONE, ZER, ONE, ZER, ONE}, //87
		{ZER, ONE, ONE, ONE, ZER, ONE, ZER}, //46
		{ZER, ZER, ONE, ONE, ONE, ZER, ONE}, //92
		{ZER, ZER, ZER, ONE, ONE, ONE, ZER}, //56
		{ZER, ZER, ZER, ZER, ONE, ONE, ONE}, //112
		{ONE, ZER, ZER, ZER, ZER, ONE, ONE}, //97
		{ONE, ONE, ZER, ZER, ZER, ZER, ONE}, //67
		{ZER, ONE, ONE, ZER, ZER, ZER, ZER}, //6
		{ONE, ZER, ONE, ONE, ZER, ZER, ZER}, //13
		{ONE, ONE, ZER, ONE, ONE, ZER, ZER}, //27
		{ONE, ONE, ONE, ZER, ONE, ONE, ZER}, //55
		{ZER, ONE, ONE, ONE, ZER, ONE, ONE}, //110
		{ONE, ZER, ONE, ONE, ONE, ZER, ONE}, //93
		{ZER, ONE, ZER, ONE, ONE, ONE, ZER}, //58
		{ZER, ZER, ONE, ZER, ONE, ONE, ONE}, //116
		{ONE, ZER, ZER, ONE, ZER, ONE, ONE}, //105
		{ONE, ONE, ZER, ZER, ONE, ZER, ONE} //83
};
//</editor-fold>

//<editor-fold desc="pre-computed skinny64 sliced round constants"
uint64_t skinny64_precomputed_round_constants[62][6] = {
		{ONE, ZER, ZER, ZER, ZER, ZER, }, // 1
		{ONE, ONE, ZER, ZER, ZER, ZER, }, // 3
		{ONE, ONE, ONE, ZER, ZER, ZER, }, // 7
		{ONE, ONE, ONE, ONE, ZER, ZER, }, // 15
		{ONE, ONE, ONE, ONE, ONE, ZER, }, // 31
		{ZER, ONE, ONE, ONE, ONE, ONE, }, // 62
		{ONE, ZER, ONE, ONE, ONE, ONE, }, // 61
		{ONE, ONE, ZER, ONE, ONE, ONE, }, // 59
		{ONE, ONE, ONE, ZER, ONE, ONE, }, // 55
		{ONE, ONE, ONE, ONE, ZER, ONE, }, // 47
		{ZER, ONE, ONE, ONE, ONE, ZER, }, // 30
		{ZER, ZER, ONE, ONE, ONE, ONE, }, // 60
		{ONE, ZER, ZER, ONE, ONE, ONE, }, // 57
		{ONE, ONE, ZER, ZER, ONE, ONE, }, // 51
		{ONE, ONE, ONE, ZER, ZER, ONE, }, // 39
		{ZER, ONE, ONE, ONE, ZER, ZER, }, // 14
		{ONE, ZER, ONE, ONE, ONE, ZER, }, // 29
		{ZER, ONE, ZER, ONE, ONE, ONE, }, // 58
		{ONE, ZER, ONE, ZER, ONE, ONE, }, // 53
		{ONE, ONE, ZER, ONE, ZER, ONE, }, // 43
		{ZER, ONE, ONE, ZER, ONE, ZER, }, // 22
		{ZER, ZER, ONE, ONE, ZER, ONE, }, // 44
		{ZER, ZER, ZER, ONE, ONE, ZER, }, // 24
		{ZER, ZER, ZER, ZER, ONE, ONE, }, // 48
		{ONE, ZER, ZER, ZER, ZER, ONE, }, // 33
		{ZER, ONE, ZER, ZER, ZER, ZER, }, // 2
		{ONE, ZER, ONE, ZER, ZER, ZER, }, // 5
		{ONE, ONE, ZER, ONE, ZER, ZER, }, // 11
		{ONE, ONE, ONE, ZER, ONE, ZER, }, // 23
		{ZER, ONE, ONE, ONE, ZER, ONE, }, // 46
		{ZER, ZER, ONE, ONE, ONE, ZER, }, // 28
		{ZER, ZER, ZER, ONE, ONE, ONE, }, // 56
		{ONE, ZER, ZER, ZER, ONE, ONE, }, // 49
		{ONE, ONE, ZER, ZER, ZER, ONE, }, // 35
		{ZER, ONE, ONE, ZER, ZER, ZER, }, // 6
		{ONE, ZER, ONE, ONE, ZER, ZER, }, // 13
		{ONE, ONE, ZER, ONE, ONE, ZER, }, // 27
		{ZER, ONE, ONE, ZER, ONE, ONE, }, // 54
		{ONE, ZER, ONE, ONE, ZER, ONE, }, // 45
		{ZER, ONE, ZER, ONE, ONE, ZER, }, // 26
		{ZER, ZER, ONE, ZER, ONE, ONE, }, // 52
		{ONE, ZER, ZER, ONE, ZER, ONE, }, // 41
		{ZER, ONE, ZER, ZER, ONE, ZER, }, // 18
		{ZER, ZER, ONE, ZER, ZER, ONE, }, // 36
		{ZER, ZER, ZER, ONE, ZER, ZER, }, // 8
		{ONE, ZER, ZER, ZER, ONE, ZER, }, // 17
		{ZER, ONE, ZER, ZER, ZER, ONE, }, // 34
		{ZER, ZER, ONE, ZER, ZER, ZER, }, // 4
		{ONE, ZER, ZER, ONE, ZER, ZER, }, // 9
		{ONE, ONE, ZER, ZER, ONE, ZER, }, // 19
		{ZER, ONE, ONE, ZER, ZER, ONE, }, // 38
		{ZER, ZER, ONE, ONE, ZER, ZER, }, // 12
		{ONE, ZER, ZER, ONE, ONE, ZER, }, // 25
		{ZER, ONE, ZER, ZER, ONE, ONE, }, // 50
		{ONE, ZER, ONE, ZER, ZER, ONE, }, // 37
		{ZER, ONE, ZER, ONE, ZER, ZER, }, // 10
		{ONE, ZER, ONE, ZER, ONE, ZER, }, // 21
		{ZER, ONE, ZER, ONE, ZER, ONE, }, // 42
		{ZER, ZER, ONE, ZER, ONE, ZER, }, // 20
		{ZER, ZER, ZER, ONE, ZER, ONE, }, // 40
		{ZER, ZER, ZER, ZER, ONE, ZER, }, // 16
		{ZER, ZER, ZER, ZER, ZER, ONE, }, // 32
};
//</editor-fold>

#endif //FORKSKINNYPLUS_CONSTANTS_H
