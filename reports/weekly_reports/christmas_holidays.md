## Report Christmas holidays

### Tuesday 27 dec

I got my hands on an Arduino Sense BLE 33 a few weeks ago, it has a Cortex M4 chip with a special register keeping score
of the current cycle count. Managed to set up PlatformIO IDE in VS Code, so I can write code in vanilla C++ instead of
the over simplified arduino IDE. Also managed to find a code snippet to actually poll the cycle count, seems to work.

### Wednesday 28 dec

I struggled a bit to find a suitable IDE/environment to write code for the arduino in vanilla c++, but decided it's not
worth my time and just stuck with the Arduino IDE. Managed to *finally* really benchmark the LSFR and sbox on the
arduino. A bit sliced sbox is faster: 16 vs 45 cycles, and the lsfr is faster as well: 14 vs 18 cycles. Measured on an
Arduino Sense BLE 33 (Cortex M4, 32 bit). Tomorrow I'll do the permutations and shiftrow.

### Friday 30 dec

Started working on the TK permutation, i.e. performing the permutation on a bit sliced state. I think that we're going
to run into the first disadvantage/slowdown as a result of using bit slicing, because we have to perform 4 permutations
on every slice instead of the original single permutation on the whole state. I think in the end, this slowdown will be
negligible, but still. I also looked at the [SIMD skinny implementation](https://github.com/kste/skinny_avx), they claim
they can do 64 blocks in parallel, which is ridiculously fast. I was excited to plan on doing 2 blocks in parallel, let
alone 64... Although I haven't really looked at combining SIMD with bit slicing, this is for the next semester (when I
get to the modes).

### Saturday 31 dec

I implemented the TK permutation in a bit sliced manner. Erik's old permutation takes about 42 cycles, new bit sliced
permutation takes 82 cycles when permuting every slice separately. This was expected since we actually do 4x the work
that Erik's previous implementation
needed, so this is no surprise. Although I think I can speed this up with a factor of 2 if I pack 2 slices in 1 32-bit
variable. Then we only do 2 of these 'slice-permutations' instead of 4.

### Sunday 01 jan

I implemented my idea of packing 2 or all 4 slices in 1 value, and performing the permutation on this packed value.

- old non-bitsliced permutation: 56 cycles (using 2x 32 bit registers)
- 4 times 1 permutation per slice: 131 cycles (using 4x 8 bit registers)
- 2 times 1 permutation per 2 slices: 77 cycles (using 2x 16 bit registers)
- 1 permutation across all 4 slices: 35 cycles (using 1 32 bit register)

This is good news, we can follow the last option with the least amount of cycles, but then we need a 32 bit register
which we don't want if we want to parallelize multiple primitive calls in the same register (will form a bottleneck). So
we're going to follow the 3rd bullet which needs 77 cycles. Even though this is a slowdown, this option only needs
16-bit registers and will allow us to parallelize multiple primitive calls in the same register, which will compensate
for this slowdown.

## What's on my mind

- Now that I finally have results that allow me to sleep at night, I'll be able to start writing the end-to-end
  bit-sliced implementation next week, although I'll probably not be able to finish it before the next semester starts.
- 64 blocks in parallel is really, *really* fast. I can't imagine how they do this, even if you could combine bit
  slicing with SIMD. Looking at their code raises more questions than answers, I really wish they documented their code.
- It turns out that Cortex M4 chips also
  have [SIMD intrinsics](https://www.keil.com/pack/doc/CMSIS/Core/html/group__intrinsic__SIMD__gr.html) built on 32-bit
  SIMD registers, so definitely taking a look at that. Maybe we can use this to speed up the permutation or speed up the
  slice() and unslice() operations. If we can make the slicing operations much faster, then it might become more
  lucrative to first unslice, permute in 1 cycle through SIMD, and slice again. Because the permutation is ridiculously
  fast with 64 bit SIMD (e.g. this
  intel [SIMD intrinsic](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=shuffle&techs=MMX,SSE_ALL&ig_expand=6562,5660)
  that performs a constant permutation in 1 instruction).