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
on every slice_t instead of the original single permutation on the whole state. I think in the end, this slowdown will be
negligible, but still. I also looked at the [SIMD skinny implementation](https://github.com/kste/skinny_avx), they claim
they can do 64 blocks in parallel, which is ridiculously fast. I was excited to plan on doing 2 blocks in parallel, let
alone 64... Although I haven't really looked at combining SIMD with bit slicing, this is for the next semester (when I
get to the modes).

### Saturday 31 dec

I implemented the TK permutation in a bit sliced manner. Erik's old permutation takes about 42 cycles, new bit sliced
permutation takes 82 cycles when permuting every slice_t separately. This was expected since we actually do 4x the work
that Erik's previous implementation
needed, so this is no surprise. Although I think I can speed this up with a factor of 2 if I unpack 2 slices in 1 32-bit
variable. Then we only do 2 of these 'slice_t-permutations' instead of 4.

### Sunday 01 jan

I implemented my idea of packing 2 or all 4 slices in 1 raw, and performing the permutation on this packed raw.

- old non-bitsliced permutation: 56 cycles (using 2x 32 bit registers)
- 4 times 1 permutation per slice_t: 131 cycles (using 4x 8 bit registers)
- 2 times 1 permutation per 2 slices: 77 cycles (using 2x 16 bit registers)
- 1 permutation across all 4 slices: 35 cycles (using 1 32 bit register)

This is good news, we can follow the last option with the least amount of cycles, but then we need a 32 bit register
which we don't want if we want to parallelize multiple primitive calls in the same register (will form a bottleneck). So
we're going to follow the 3rd bullet which needs 77 cycles. Even though this is a slowdown, this option only needs
16-bit registers and will allow us to parallelize multiple primitive calls in the same register, which will compensate
for this slowdown.

### Tuesday 03 jan

Implemented the shift rows operation, benchmarked it on arduino. We hit a real roadblock, a bitsliced shift row
operation is 4-5 times as slow as before. The problem is again we have to perform the shiftrow on every slice_t, which is
again, evidently, 4 times the amount of work that was done before.
I'm thinking of a way to make this faster: what if we use lookup tables? If every row is just a bit rotation, what if we
just wake a 4-bit lookup table that just maps a nibble to its bit rotated raw? Then we can swap a rotate instruction
with a single lookup. Since the lookup table is only 4-bit, we only need 2^4 values, which well fits in the arduino's
cache. If we could simplify bit rotations like this, I believe we could achieve a tremendous speedup, as a bit rotation
is multiple instructions, replacing it with a single lookup could speed this up drastically. I'll try this tomorrow.

## Wednesday 04 jan

Implemented the substitution tables. I made 4 lookup tables, 3 of them map to a nibble's rotated raw, rotating 1,2 or
3
bits (for the 2nd, 3rd and 4th row in shiftrows). I then also made a 8-bit lookup table that maps row3||row4 to its
corresponding raw where row3 is rotated 2 bits and row4 3 bits (as 1 8-bit raw).
When benchmarking individually, the 4-bit lookup tables seem faster, even 2 4-bit lookups are faster than 1 8-bit lookup
for a combined row2||row3, for some reason.
a 4-bit lookup takes 6-8 cycles, this is a speedup compared to manually calculating a rotation (13 cycles). This isn't
good enough though, as we still need to do everything 4 times, and 4*6 is still way more than 13.
When actually using these lookups in the full shift row operation, it's even slower than everything else I've tried,
even though the lookups individually are 2x faster than a bit rotation calculation, for some reason (?)

## Saturday 07 jan

Continued working on the shift rows. I fear we're going to have to let go of our goal to have every operation use online
a 16 bit register. We can make the shift rows (and probably mixcols as well) equally fast as before, if we unpack the
whole state in a 64 bit register (or repeat 2 times in 32 bit register). This way we can still parallelize the other
operations that use only 16 bit registers, we can still use bit slicing and thus also use fixed slicing key keys.
Only the shift rows and mix cols will be performed using larger registers. Either we use larger registers or we have a
slowdown with shift rows and mix cols, it's one or the other.

## Sunday 08 jan

Some very good news: I tried some tricks with lookup tables and union structs to speed up the shift rows operation, and
it looks like my latest idea only needs 30 cycles to perform the whole shift row operation in a bitsliced manner, where
the previous non-bitsliced implementation needed 24. This is a slowdown of a few cycles, but becomes negligible once we factor in the
speedups from the other round function operations and when we consider parallelizing multiple round functions in the same
register.

## What's on my mind

- Now that I finally have results that allow me to sleep at night, I'll be able to start writing the end-to-end
  bit-sliced implementation next week, although I'll probably not be able to finish it before the next semester starts.
- 64 blocks in parallel is really, *really* fast. I can't imagine how they do this, even if you could combine bit
  slicing with SIMD. Looking at their code raises more questions than answers, I really wish they documented their code.
- It turns out that Cortex M4 chips also
  have [SIMD intrinsics](https://www.keil.com/unpack/doc/CMSIS/Core/html/group__intrinsic__SIMD__gr.html) built on 32-bit
  SIMD registers, so definitely taking a look at that. Maybe we can use this to speed up the permutation or speed up the
  slice_t() and unslice_accelerated_internal() operations. If we can make the slicing operations much faster, then it might become more
  lucrative to first unslice_internal, permute in 1 cycle through SIMD, and slice_t again. Because the permutation is ridiculously
  fast with 64 bit SIMD (e.g. this
  intel [SIMD intrinsic](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=shuffle&techs=MMX,SSE_ALL&ig_expand=6562,5660)
  that performs a constant permutation in 1 instruction).
- The shift row operation is really difficult to optimize, as it's a function that doesn't change the raw of every
  individual slices by a logical operation, rather it moves cells around within their state. And bit slicing is not really
  optimized for that. I fear the same reasoning will hold for mix cols, but I haven't looked at mix cols yet. 