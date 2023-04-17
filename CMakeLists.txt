cmake_minimum_required(VERSION 3.20)
project(forkskinnyplus)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mavx512f -mavx512dq")

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

add_executable(forkskinny128-384-benchmarking
        src/benchmarking/forkskinny128-benchmarking.cpp)