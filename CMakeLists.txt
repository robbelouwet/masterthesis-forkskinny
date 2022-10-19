cmake_minimum_required(VERSION 3.20)
project(ForkSkinnyPlus)

set(CMAKE_CXX_STANDARD 14)


# Thesis project code:
add_executable(forkskinnyplus
        src/main.cpp
        src/headers/main.h

        #Erik's forkskinny-c
        #contrib/forkskinny-c/demo.c
        contrib/forkskinny-c/forkskinny128-cipher.c
        contrib/forkskinny-c/forkskinny64-cipher.c
        contrib/forkskinny-c/headers/forkskinny128-cipher.h
        contrib/forkskinny-c/headers/forkskinny64-cipher.h
        contrib/forkskinny-c/headers/forkskinny-internal.h)

add_subdirectory(contrib/skinny-plus)
target_link_libraries(forkskinnyplus PUBLIC skinny-plus)
target_include_directories(forkskinnyplus PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/contrib/skinny-plus"
        )