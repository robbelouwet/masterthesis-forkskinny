#ifndef FORKSKINNYPLUS_LSFR_H
#define FORKSKINNYPLUS_LSFR_H

#include "../utils.h"
#include "forkskinny64-cipher.h"

std::tuple<ulong, ulong> lsfr_64_tk2(State64Sliced_t state);

vec lsfr_64_tk2_inverse(State64_t state);

vec lsfr_64_tk3(State64_t state);

vec lsfr_64_tk3_inverse(State64_t state);

#endif //FORKSKINNYPLUS_LSFR_H
