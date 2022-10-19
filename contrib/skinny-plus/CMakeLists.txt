cmake_minimum_required(VERSION 3.20)
project(skinny-plus)

set(CMAKE_CXX_STANDARD 14)

add_library(skinny-plus SHARED
        internal-skinny-plus.c
        internal-util.c
        headers/internal-util.h
        headers/internal-skinny-plus-config.h
        headers/internal-skinny-plus.h)