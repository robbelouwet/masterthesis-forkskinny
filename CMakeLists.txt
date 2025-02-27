cmake_minimum_required(VERSION 3.20)
project(forkskinnyplus)

# Find Google Benchmark
find_package(benchmark REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Standard optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-all-loops -fopt-info-vec-optimized=build.log -floop-parallelize-all")

# Enable the AVX2 & SSE vector extensions (needed for segmentation + will be used for auto-vectorization)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mavx -msse -msse2 -msse3 -mfpmath=sse")

# Enable the AVX512 vector extensions (for auto-vectorization)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512f -mavx512dq")

add_executable(demo
        libs/forkskinny-c/demo.c
        libs/forkskinny-c/forkskinny64-cipher.c
        libs/forkskinny-c/forkskinny128-cipher.c)


add_executable(constants_generator constants_generator.cpp)

add_executable(forkskinny64-plus-test
        src/forkskinny64-plus/forkskinny64_test.cpp
        )

add_executable(forkskinny128-plus-test
        src/forkskinny128-plus/forkskinny128_test.cpp
        )

add_executable(forkskinny-benchmark src/benchmark.cpp)
target_link_libraries(forkskinny-benchmark benchmark pthread)

#add_executable(forkskinny128-384-benchmarking
#        src/benchmarking/forkskinny128-benchmarking.h)
#
## Link with Google Benchmark
#target_link_libraries(forkskinny128-384-benchmarking benchmark pthread)

#add_executable(improved_ks-192-benchmarking
#        src/benchmarking/improved_ks-benchmarking.h)
#target_link_libraries(improved_ks-192-benchmarking benchmark pthread)