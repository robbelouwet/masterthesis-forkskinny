## Weekly report 27 february - 05 march

# Tuesday

Implemented AddConstant and MixCols. For the AddConstant I pre-computed all possible round constants (since they're
fixed and always the same with every block). Both all the round constants and the branch constant are pre-computed,
sliced and saved in the constants.h file. After finishing the operations, I was able to start sewing everything
together. I can now encrypt a single forkskinny-64-64 block according to the most basic mode (so going from M -> C0, C1
when s=b), or at least it spits out a ciphertext, but I haven't tested anything yet!

# Thursday

Continued on sewing together the rounds and started comparing to Erik's output. The AddConstant step wasn't implemented
correctly, which I fixed. Besides that, the sboxes, lfsrs and tk permutations seem to be correct when testing those
individually, ShiftRows and MixCols still have to be tested. There is still something going wrong though, Erik has
different ciphertext output than me with the exact same message and tweakeys. He outsources the round constants and
pre-computes them together with the key schedule, I have no idea what he's doing there so I emailed him.

## What was on my mind

- Because the first semester gave me a lot of experience, it feels like I'm able to do everything a lot faster. Still,
  I'm a bit anxious when I'll have to turn to benchmarking the code again, because I'll be forced to use the RDTSC
  instruction again, which I wanted to avoid (the whole reason I switched to that 32-bit cortex microcontroller).
- Everything seems to go smoothly, I just fear for the benchmarking.