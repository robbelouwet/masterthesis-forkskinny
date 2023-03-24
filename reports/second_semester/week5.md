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

# Friday

generated more pre-computed round constants for fs128-384, which has the most amount of rounds. Also generated the
pre-computed fs128 branch constant (so implemented the script that generates this). Also implemented lfsr's for tk2 and
tk3. Now I can start implementing the key schedule of fs128 which I couldn't start with because the add-constant step is
intertwined with the key schedule.

## What was on my mind

