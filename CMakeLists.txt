cmake_minimum_required(VERSION 3.20)
project(forkskinnyplus)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -msse4.2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

# Thesis project code:
add_executable(forkskinnyplus
        src/main.cpp
        src/headers/main.h
        src/utils.h
        src/avx2/tweakey-schedule/LSFR.cpp
        src/headers/LSFR.h "src/forkskinny-plus/forkskinny-plus.cpp" "src/forkskinny-plus/forkskinny-plus.h")

# standalone demo of forksinny-c
add_executable(forkskinny-demo
        libs/forkskinny-c/demo.c
        libs/forkskinny-c/forkskinny64-cipher.c
        libs/forkskinny-c/forkskinny128-cipher.c
        libs/forkskinny-c/headers/forkskinny-internal.h
        libs/forkskinny-c/headers/forkskinny64-cipher.h
        libs/forkskinny-c/headers/forkskinny128-cipher.h)


add_subdirectory(libs)

target_link_libraries(forkskinnyplus PUBLIC libs)
target_include_directories(forkskinnyplus PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/libs")

