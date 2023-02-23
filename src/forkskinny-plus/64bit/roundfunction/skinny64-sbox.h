#ifndef FORKSKINNYPLUS_SKINNY64_SBOX_H
#define FORKSKINNYPLUS_SKINNY64_SBOX_H

#include "../forkskinny64-plus.h"

static inline Cell64 skinny64_sbox(Cell64 cell){
	auto result = Cell64();
	
	result.slices[3] = cell.slices[0] ^ ~(cell.slices[3] | cell.slices[2]);
	result.slices[2] = cell.slices[3] ^ ~(cell.slices[2] | cell.slices[1]);
	result.slices[1] = cell.slices[2] ^ ~(cell.slices[1] | result.slices[3]);
	result.slices[0] = cell.slices[1] ^ ~(result.slices[3] | result.slices[2]);
	
	return result;
}

static inline Cell64 skinny64_sbox_inverse(Cell64 cell){

}

#endif //FORKSKINNYPLUS_SKINNY64_SBOX_H
