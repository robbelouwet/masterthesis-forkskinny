cmake_minimum_required(VERSION 3.20)
project(forkskinnyplus)

set(CMAKE_CXX_STANDARD 14)


# Thesis project code:
add_executable(forkskinnyplus
        src/main.cpp
        src/main.h src/benchmarking/benchmark.cpp
        src/benchmarking/headers/benchmark.h
        src/benchmarking/test-fixslicing.cpp
        src/benchmarking/headers/test-fixslicing.h src/utils.h)


add_subdirectory(contrib)

target_link_libraries(forkskinnyplus PUBLIC contrib)
target_include_directories(forkskinnyplus PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/contrib")

