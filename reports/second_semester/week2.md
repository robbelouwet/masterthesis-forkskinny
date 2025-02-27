## Weekly report 27 february - 05 march

# Tuesday

Implemented AddConstant and MixCols. For the AddConstant I pre-computed all possible round constants (since they're
fixed and always the same with every block). Both all the round constants and the branch constant are pre-computed,
sliced and saved in the constants.h file. After finishing the operations, I was able to start sewing everything
together. I can now encrypt a single forkskinny-64-64 block according to the most basic mode (so going from M -> C0, C1
when s=b), or at least it spits out a ciphertext, but I haven't tested anything yet!

# Wednesday

Continued on sewing together the rounds and started comparing to Erik's output. The AddConstant step wasn't implemented
correctly, which I fixed. Besides that, the sboxes, lfsrs and tk permutations seem to be correct when testing those
individually, ShiftRows and MixCols still have to be tested. There is still something going wrong though, Erik has
different ciphertext output than me with the exact same message and tweakeys. He outsources the round constants and
pre-computes them together with the key schedule, I have no idea what he's doing there so I emailed him.

# Thursday

Ran into a little problem. The reason why I keep producing wrong ciphertexts is because in my implementation,
the first, upper C1 cell of the cipherstate represent my least significant bits. So my operations are all correct (i
think) but I apply them in a mirror-like manner. This means that when a plaintext block, for example, equals
0xFEDCBA987654321, 0x1 is my first upper-C1 cell, but that should be 0xF, and 0xE should be the second etc. When
looking at Erik's implementation, he also does it like this. Although I think I can relatively quickly solve this by
adjusting the slicing operation where slices are just turned around. So the first slice_t will now become the last one
etc. When unslicing, I mirror them back in place.

# Friday

Fixed the problem by reversing the order of the sliced cells, so 0x0123456789ABCDE as a plaintext block is first
reversed to 0xFEDCBA9876543210 and then sliced. If I do this, the results of the ShiftRows and tk permutation are the
same as Erik's and they make sense. Although still no correct ciphertext is computed, still stuck on the weird
AddConstant step. I benchmarked against skinny-64-192 and Erik's forkskinny-64-192 but both produce wrong ciphertexts,
even though the individual operations all seem to work fine and correctly (except for AddConstant step which I have
trouble comparing because Erik's implemented it in a weird way inside the keyschedule). Also the AddBranchConstant I cant
really test except for calculating it by hand and verifying maybe.

## What was on my mind

- Because the first semester gave me a lot of experience, it feels like I'm able to do everything a lot faster. Still,
  I'm a bit anxious when I'll have to turn to benchmarking the code again, because I'll be forced to use the RDTSC
  instruction again, which I wanted to avoid (the whole reason I switched to that 32-bit cortex microcontroller).
- Everything seems to go smoothly, I just fear for the benchmarking.