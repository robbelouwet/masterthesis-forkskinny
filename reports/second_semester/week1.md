## Weekly report 20 - 26 february

# Monday

Completed the fixsliced keyschedule, but don't yet know how to test it. The paper of SKINNY provides a test vector +
Erik has demo code to test against, but this isn't the case with the fixslicing keyschedule, can't find a test vector in
the paper. Thinking about cloning their repo and hoping that we both compute the same tks, but if this doesn't work, the
problem could lie anywhere because my implementation is not exactly the same of course.
I could mail the authors of the fixslicing paper and ask for a test vector if they had one?

# Tuesday

Implemented slice_t, unslice128_internal and fixedslicing key schedule operation for skinny128. I had to do this because in order to
test the correctness of the fixedslicing keyschedule, I have to compare it against the implementation of the authors of
[the paper](https://www.esat.kuleuven.be/cosic/events/silc2020/wp-content/uploads/sites/4/2020/10/Submission6.pdf) on
the fixed slicing technique, and their implementation of the fixsliced keyschedule computes 2 schedules of skinny128 in
parallel.

# Thursday

Refactored some code because my desktop apparently doesn't support the AVX512 hardware, only up to AVX2 with 256bit
registers. Next, implemented the skinny64 keyschedule with no fixslicing, then re-implemented the s-box and shift rows
round operations because the slicing works in a different way than from what I had last semester, although most of the
code of those operations really is the same.

## What was on my mind

- I should read more code of existing bitsliced implementations, but they are very cumbersome to read. I usually
  skim some code I think I can learn from and I can see what they are trying to do, but it's a whole other story seeing
  exactly *how* they're trying to do it, especially because bit-slicing and SIMD produce very obscure code by nature. On
  top of that, a lot of those repositories from lwc candidates and rweather etc. implement bit-slicing and/or simd in a
  slightly different way than I do, so it's also difficult to compare correctness with the fixslicing tks for example.
  The authors of fixslicing use bitslicing with only 2 blocks in parallel, for example. Or they organize their data
  structure of how the key and cipher state is laid out differently, and so the code is much different from mine as
  well.
- I have an idea: why stick with doing 64 blocks in parallel? The thought was that we can only do 64 blocks in parallel,
  because registers can only be 64 bits, but what about simd registers? Simd registers can go up to 512 bits, why not
  calculate 512 blocks in parallel? What if a single slice_t now equals an `__m512i` data type, which is a special 512 bit
  register? The ony thing we have to be able to do is the basic logical operations (xor, or, and) and we need to be able
  to move a slice_t, but we can do this. Moving is just re-assignment within an array, and the logical operations also
  exist:
    - _mm512_xor_si512(__m512i a, __m512i
      b) ([link](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=6790,694,6097,5383,7788,363,7788&text=_mm512_xor_si512&techs=AVX_512))
    - _mm512_or_si512(__m512i a, __m512i
      b) ([link](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=6790,694,6097,5383,7788,363,7788,5295&techs=AVX_512&text=_mm512_or_si512))
    - _mm512_and_si512(__m512i a, __m512i
      b) ([link](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=6790,694,6097,5383,7788,363,7788,5295,363&techs=AVX_512&text=_mm512_and_si512))

  SIMD was already used in skinny-like implementations, but it was used to speed up operations themselves, and not
  increase the size of a slice_t (hence you will only find skinny implementations with at most 64 blocks in parallel,
  never more). The only catch will be that I won't be able to use simd speedups in the operations
  themselves. So either we make the slices 512 bits and do all the operations manually as if we're working on individual
  bits of an internal state; or we do 64 bits per slice_t, and might get some speedups in the operations themselves. The
  problem is that getting speedups in the operations themselves would have to offer exactly 4x speedups, otherwise
  256bit simd slices with manual operations would become faster, which isn't always the case, let alone with 512 bit
  slices.