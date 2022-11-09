## Weekly report 07 - 13 nov

## Monday

Started actually implementing the LSFR for TK2 in the tweakey schedule. More specifically, I was trying to figure out
how to get a State64_t (whch is the 4x4 tweakey state) into a __mm256 datatype. In other words, trying to bit slice the
4x4 TK2 tweakey state, such that I can apply the LSFR and PT in a SIMD manner later on, but first things first. When
this works, it's time to try
out [the SIMD instruction for a shift left](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=5264,4235,67,0,5523,5224,5264,5264,6531,6501&text=sl&techs=AVX,AVX2)
which will be used inside the LSFR.

## Wednesday

Thinking about fully changing the State64_t type to always be a simd register. Managed to get a shift-left operation
working in parallel on every 16-bit int in a 256 SIMD lane, it's 2-3x faster than looping over the 16 ints

## Thursday

### What was on my mind
