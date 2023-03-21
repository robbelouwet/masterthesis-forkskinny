## Weekly report 20 march - 26 march

# Monday

Worked a few hours. I'm leaving the accelerated slicing idea aside for now. I'm hitting some roadblocks and have doubts
on whether it
will be actually faster. Fixed also some code relating to the 512-bit slices, encryption and decryption now also work
with AVX512 slices, so 512 blocks in parallel.

# Tuesday

Refactored some code, implemented the fixsliced keyschedule for forkskinny64 and started implementing the forkskinny128
version. Although, that just comes down to copying the forkskinny64 folder and adjusting all the parameters and change
some operations.

## What was on my mind

