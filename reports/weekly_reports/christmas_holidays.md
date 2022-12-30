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

## What's on my mind

- Now that I finally have results that allow me to sleep at night, I'll be able to start writing the end-to-end
  bit-sliced implementation next week, although I'll probably not be able to finish it before the next semester starts.
- 64 blocks in parallel is really, *really* fast. I can't imagine how they do this, even if you could combine bit
  slicing with SIMD. Looking at their code raises more questions than answers, I really wish they documented their code. 