#ifndef FORKSKINNYPLUS_TEST_VECTORS_H
#define FORKSKINNYPLUS_TEST_VECTORS_H

#include <cstdint>
#include <vector>

#include "forkskinny64-plus/utils/forkskinny64-datatypes.h"
#include "forkskinny64-plus/utils/slicing64.h"

#include "forkskinny128-plus/utils/forkskinny128-datatypes.h"
#include "forkskinny128-plus/utils/slicing128.h"

static inline Blocks64_t M_rand_64(int seed) {
	auto M = Blocks64_t();
	srand(seed);
	for (auto &value: M.values)
		for (unsigned char & byte : value.bytes)
			byte = uint8_t(rand());
	return M;
}

static inline Blocks64_t M_64() {
	uint8_t pt[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	
	auto M = Blocks64_t();
	for (auto &value: M.values)
		for (int j = 0; j < 8; ++j)
			value.bytes[j] = pt[j];
	
	return M;
}

static inline Blocks64_t TK1_64() {
	uint8_t ct_tk1[8] = {0xf1, 0x11, 0x00, 0x22, 0x00, 0x33, 0x00, 0x44};
	// Set TK1
	auto TK1 = Blocks64_t();
	for (auto &value: TK1.values)
		for (int j = 0; j < 8; ++j)
			value.bytes[j] = ct_tk1[j];
	return TK1;
}

static inline Blocks64_t TK2_64() {
	uint8_t ct_tk2[8] = {0xf2, 0x55, 0x00, 0x66, 0x00, 0x77, 0x00, 0x88};
	
	// Set TK2
	auto TK2 = Blocks64_t();
	for (auto &value: TK2.values)
		for (int j = 0; j < 8; ++j)
			value.bytes[j] = ct_tk2[j];
	return TK2;
}

static inline Blocks64_t TK3_64() {
	uint8_t ct_tk3[8] = {0xf3, 0x99, 0x00, 0xaa, 0x00, 0xbb, 0x00, 0xcc};
	
	// Set TK3
	auto TK3 = Blocks64_t();
	for (auto &value: TK3.values)
		for (int j = 0; j < 8; ++j)
			value.bytes[j] = ct_tk3[j];
	return TK3;
}

// ----------------------------------------------

static inline Blocks128_t M_128() {
	uint8_t pt[16] = {
			0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	auto M = Blocks128_t();
	for (auto &value: M.values)
		for (int j = 0; j < 16; ++j)
			value.bytes[j] = pt[j];
	return M;
}

static inline Blocks128_t TK1_128() {
	uint8_t ct_tk1[16] =
			{0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d};
	// Set TK1
	auto TK1 = Blocks128_t();
	for (auto &value: TK1.values)
		for (int j = 0; j < 16; ++j)
			value.bytes[j] = ct_tk1[j];
	return TK1;
}

static inline Blocks128_t TK2_128() {
	uint8_t ct_tk2[16] = {
			0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a, 0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0x80, 0x01};
	
	// Set TK2
	auto TK2 = Blocks128_t();
	for (auto &value: TK2.values)
		for (int j = 0; j < 16; ++j)
			value.bytes[j] = ct_tk2[j];
	return TK2;
}

static inline Blocks128_t TK3_128() {
	uint8_t ct_tk3[16] = {
			0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46, 0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d};
	
	// Set TK3
	auto TK3 = Blocks128_t();
	for (auto &value: TK3.values)
		for (int j = 0; j < 16; ++j)
			value.bytes[j] = ct_tk3[j];
	return TK3;
}

#endif //FORKSKINNYPLUS_TEST_VECTORS_H
