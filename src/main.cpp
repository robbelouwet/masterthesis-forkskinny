#include "main.h"
#include <x86intrin.h>
#include <iostream>
#include "utils.h"
#include "xmmintrin.h"
#include "benchmarking/headers/benchmark.h"

/**
 * \brief Swaps bits within two words.
 *
 * \param a The first word.
 * \param b The second word.
 * \param mask Mask for the bits to shift.
 * \param shift Shift amount in bits.
 */
#define skinny_swap_move(a, b, mask, shift) \
    do { \
        uint32_t tmp = ((b) ^ ((a) >> (shift))) & (mask); \
        (b) ^= tmp; \
        (a) ^= tmp << (shift); \
    } while (0)

//<editor-fold desc="platform check">
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
     #define ENV64BIT 1
  #else
    #define ENV32BIT 1
  #endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT 1
#endif
#endif

// Perform the check
#ifndef ENV64BIT
#error "Platform not 64-bit!"
#endif

static_assert(sizeof(void*) ==8, "Not running on 64-bit!");
//</editor-fold>

void test_simd(){
	// vanilla addition
	ulong before_v = _rdtsc();
	float a_1 = 3.0f + 2.0f;
	float a_2 = 3.0f + 2.0f;
	float a_3 = 3.0f + 2.0f;
	float a_4 = 3.0f + 2.0f;
	ulong after_v = _rdtsc();
	std::cout << "VANILLA cycles: " << after_v - before_v;
	
	uint *a = new uint(255);
	uint *b = new uint(0);
	
	skinny_swap_move(*a, *b, 0x128, 0x6);
	
	uint appel = 1;
	
}

int main() {
	test_simd();
    //test_benchmark();
}
