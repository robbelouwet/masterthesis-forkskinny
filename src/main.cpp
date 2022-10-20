#include "benchmarking/headers/benchmark.h"

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

int main() {
    test_forkskinny();
}
