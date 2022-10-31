cmake_minimum_required(VERSION 3.20)
project(contrib)

set(CMAKE_CXX_STANDARD 14)


add_library(contrib STATIC
        skinny-plus/internal-skinny-plus.c
        skinny-plus/internal-util.c
        skinny-plus/headers/internal-util.h
        skinny-plus/headers/internal-skinny-plus-config.h
        skinny-plus/headers/internal-skinny-plus.h

        forkskinny-c/forkskinny64-cipher.c
        forkskinny-c/forkskinny128-cipher.c
        forkskinny-c/headers/forkskinny-internal.h
        forkskinny-c/headers/forkskinny64-cipher.h
        forkskinny-c/headers/forkskinny128-cipher.h)