#!/bin/bash
objdump -M intel --dwarf=decodedline -d "./cmake-build-debug/forkskinnyplus" > ./extern/dump.asm