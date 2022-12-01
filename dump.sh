#!/bin/bash
objdump -M intel --dwarf=decodedline -d "./cmake-build-debug/benchmarking_lsfr" > ./extern/lsfr.asm
objdump -M intel --dwarf=decodedline -d "./cmake-build-debug/benchmarking_sbox" > ./extern/sbox.asm