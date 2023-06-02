#include <iostream>
#include "benchmarking/forkskinny64-benchmarking.h"
#include "benchmarking/forkskinny128-benchmarking.h"

int main(){
	benchmark_forkskinny64_128();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_forkskinny64_192();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_PAEF_forkskinny64_192();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_forkskinny128_256();
	std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++\n";
	benchmark_forkskinny128_384();
}