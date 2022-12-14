cmake_minimum_required(VERSION 3.20)
project(forkskinnyplus)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


# Thesis project code:
add_executable(forkskinnyplus
        src/main.cpp
        src/headers/main.h
        src/utils.h
        src/forkskinny-plus/forkskinny-plus.cpp
        src/forkskinny-plus/headers/forkskinny-plus.h
        src/headers/full-state-slicing.h
        src/headers/half-state-slicing.h
        )

# standalone demo of forksinny-c
add_executable(forkskinny-demo
        libs/forkskinny-c/demo.c
        libs/forkskinny-c/forkskinny64-cipher.c
        libs/forkskinny-c/forkskinny128-cipher.c
        libs/forkskinny-c/headers/forkskinny-internal.h
        libs/forkskinny-c/headers/forkskinny64-cipher.h
        libs/forkskinny-c/headers/forkskinny128-cipher.h)

# benchmarking the LSFR for TK2
add_executable(benchmarking_lsfr
        src/benchmarking/lsfr_tk2_benchmarking.cpp)

# benchmarking the sbox
add_executable(benchmarking_sbox
        src/benchmarking/sbox_benchmarking.cpp)

