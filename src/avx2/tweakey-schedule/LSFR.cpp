#include "../../headers/LSFR.h"
#include "../../headers/forkskinny64-cipher.h"
#include "x86intrin.h"
#include "cmath"


vec lsfr_64_tk2(State64_t state) {
	State64_expanded_t expanded_state = State64_expanded_t ();
	
	// split and expand bytes into an array of padded nibbles
	for (int i = 0; i < 7; i++) {
		// yyyy xxxx & 0000 1111
		expanded_state.cells[2*i] = state.pairs[i] & 15; // = 0000 xxxx
		
		// yyyy xxxx & 1111 0000 = yyyy 0000;
		expanded_state.cells[(2*i)+1] = (state.pairs[i] & 240) >> 4; // yyyy 0000 >> 4 = 0000 yyyy
	}
	
	int appel = 1;
	
	// __m256i param, vector van 16x16bit integers, shift left by count
	//_mm256_sll_epi16()
}
