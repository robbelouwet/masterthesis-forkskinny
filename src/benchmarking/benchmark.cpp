#include <cstdint>
#include <iostream>
#include "headers/benchmark.h"
#include "forkskinny-c/headers/forkskinny64-cipher.h"

void print_block(uint8_t *block, unsigned int n) {
    for (unsigned int i = 0; i < n; i++)
        printf("%02x", block[i]);
}

void test_forkskinny(uint8_t key[3 * FORKSKINNY64_BLOCK_SIZE], uint8_t message[FORKSKINNY64_BLOCK_SIZE]) {

    // Pre-compute key schedule
    KeySchedule64_t tk1, tk23;
    forkskinny_c_64_192_init_tk1(&tk1, key, FORKSKINNY64_MAX_ROUNDS);
    forkskinny_c_64_192_init_tk2_tk3(&tk23, key + FORKSKINNY64_BLOCK_SIZE, FORKSKINNY64_MAX_ROUNDS);

    // Encrypt / forward one leg
    uint8_t output_right_oneleg[FORKSKINNY64_BLOCK_SIZE];

    unsigned long cycles_spent;
    forkskinny_c_64_192_encrypt(&tk1, &tk23, nullptr, output_right_oneleg,
                                message, &cycles_spent);

    std::cout << "Forkskinny-64-192 Forward s=0\n";
    std::cout << "Tweakey: ";
    print_block(key, 3 * FORKSKINNY64_BLOCK_SIZE);
    std::cout << "\nMessage: ";
    print_block(message, FORKSKINNY64_BLOCK_SIZE);
    std::cout << "\nC0: ";
    print_block(output_right_oneleg, FORKSKINNY64_BLOCK_SIZE);
}

void test_skinnyplus(uint8_t key[24], uint8_t message[8]) {


}

void test_benchmark() {
    uint8_t key[3 * FORKSKINNY64_BLOCK_SIZE] = {0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8,
                                                0x1b, 0xe8, 0xe7, 0x8d, 0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a};
    uint8_t message[FORKSKINNY64_BLOCK_SIZE] = {0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec};


    test_forkskinny(key, message);

    test_skinnyplus(key, message);
}
