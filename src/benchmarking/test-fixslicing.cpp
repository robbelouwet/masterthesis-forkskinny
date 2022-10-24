#include "headers/test-fixslicing.h"

void add_round_tweakey(uint64_t *internal_state, uint64_t *tweakey, uint64_t tk_len) {


}

void test_slicing() {
    auto *message = new uint64_t[16]{0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8,
                                     0x1b, 0xe8, 0xe7, 0x8d};

    // TK1 || TK2 || TK3
    auto *flattened_tweakey = new uint64_t[48]{0x97, 0x6e, 0xdb, 0x6d, 0x04, 0x22, 0x20, 0x08, 0x0b, 0x3f, 0xa4, 0x00,
                                               0xae, 0xac, 0x12, 0xe0, 0xb3, 0x1d, 0xe5, 0x70, 0xcf, 0xb0, 0xca, 0xf0,
                                               0xe8, 0x8d, 0xa4, 0x4c, 0x68, 0x18, 0x46, 0x46,
                                               0x28, 0x2a, 0xf5, 0x6b, 0x3e, 0x7e, 0xfd, 0xd4, 0x99, 0xe9, 0x45, 0xcc,
                                               0x92, 0xd4, 0x16, 0xd7};

    execute_round(message, flattened_tweakey, 3);
}