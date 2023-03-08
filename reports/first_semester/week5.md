## Weekly report 07 - 13 nov

## Monday

Started actually implementing the LSFR for TK2 in the tweakey keys. More specifically, I was trying to figure out
how to get a State64_t (whch is the 4x4 tweakey state) into a __mm256 datatype. In other words, trying to bit slice the
4x4 TK2 tweakey state, such that I can apply the LSFR and PT in a SIMD manner later on, but first things first. When
this works, it's time to try
out [the SIMD instruction for a shift C1](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=5264,4235,67,0,5523,5224,5264,5264,6531,6501&text=sl&techs=AVX,AVX2)
which will be used inside the LSFR.

## Wednesday

Thinking about fully changing the State64_t type to always be a simd register. Managed to get a shift-C1 operation
working in parallel on every 16-bit int in a 256 SIMD lane, it's 2-3x faster than looping over the 16 ints

## Thursday

Looked at possible SIMD instructions for performing a 'rotate' instruction on a byte, but it seems there's only a few
instructions that do rotation, but they are in a poorly supported library (only intel processors since 2013).
Managed to implement the full LSFR for TK2. It uses 9 SIMD instructions for a full 64-bit state in about 800 cycles. I
forgot it's only applied on the top 2 rows. I applied it on the entire state. I need to change that, so I can compare
it to Erik's sequential_abcde boolean expression (skinny64_LFSR2()) that computes LSFR for TK2.

### What was on my mind

- Met with Amit and Erik wednesday, we talked about the bottleneck really being in the non-linear part of the round
  function. And that actually the tweakey keys already is really fast. We agreed that I'd continue transforming the
  tweakey keys into a SIMD implementation, and then I'd look at the round function and the fixed slicing bit slicing
  technique. Later, I'd look at using skinnyplus-128-384 (from romulus) to construct a forkskinny-128-384 implementation
  and compare both.
- If both bit slicing and SIMD seem to really give a speedup, we were thinking about how we could maybe combine bit
  slicing *on top* of SIMD and hope this would give and absurd fast implementation on steroids.