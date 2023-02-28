## Weekly report 27 february - 05 march

# Tuesday

Implemented AddConstant and MixCols. For the AddConstant I pre-computed all possible round constants (since they're
fixed and always the same with every block). Both all the round constants and the branch constant are pre-computed,
sliced and saved in the constants.h file. After finishing the operations, I was able to start sewing everything
together. I can now encrypt a single forkskinny-64-64 block according to the most basic mode (so going from M -> C0, C1
when s=b), or at least it spits out a ciphertext, but I haven't tested anything yet!

# Thursday

## What was on my mind

- Because the first semester gave me a lot of experience, it feels like I'm able to do everything a lot faster. Still,
  I'm a bit anxious when I'll have to turn to benchmarking the code again, because I'll be forced to use the RDTSC
  instruction again, which I wanted to avoid (the whole reason I switched to that 32-bit cortex microcontroller).