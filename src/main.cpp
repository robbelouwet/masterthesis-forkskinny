#include <x86intrin.h>
#include <iostream>

#define VECLEN 16


typedef float vec;

void test_simd(vec *a, vec *b, vec *__restrict c) {
	for (int i = 0; i < VECLEN; i++) {
		c[i] = a[i] * b[i];
	}
}


int main() {
	vec a[16] = {6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f};
	vec b[16] = {6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f};
	vec c[16] = {6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f,6.0f, 6.0f};
	
	unsigned long before = _rdtsc();
	test_simd(a, b, c);
	unsigned long after = _rdtsc();
	
	std::cout << after - before << " cycles, c: " << *c;
	//test_benchmark();
}
