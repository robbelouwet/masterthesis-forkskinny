#include <iostream>
#include "benchmarking/forkskinny64-benchmarking.h"
#include "benchmarking/forkskinny128-benchmarking.h"

#ifdef __AVX2__
auto avx2 = "true";
#else
auto avx2 = "false";
#endif

#ifdef __AVX512F__
auto avx512 = "true";
#else
auto avx512 = "false";
#endif

int main(){
	std::cout << "Compiled with AVX2 support: " << avx2 << std::endl;
	std::cout << "Compiled with AVX512 support: " << avx512 << std::endl;
	std::cout << std::endl;
	benchmark_forkskinny64_128();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_forkskinny64_192();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_PAEF_forkskinny64_192();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_forkskinny128_256();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_forkskinny128_384();
	std::cout << std::endl;
}