#include <iostream>
#include "x86intrin.h"
#include "headers/LSFR.h"
#include "cmath"

#define VECLEN 16

int main() {
	State64_t state = State64_t();
	
	state.llrow = 18446744073709551615; // 2^64 - 1
	
	lsfr_64_tk2(state);
}
