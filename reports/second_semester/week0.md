## Weekly report 13 - 19 february

# Monday

Made tentative keys for this semester and compared with Arne Deprez' work.

# Thursday

Implemented the new slice_t and unslice_internal operation that slices and unslices 64x 64-bit states, and unslice_internal correctly. This
works.

# Friday

Implemented the LFSR and TK permutation in the new bit sliced implementation, and precomputed the permutation tables for
fixed slicing approach. Since I started doing the key schedule anyway, I started implementing the full fixed-sliced
tweakey schedule according to the paper. Since this is a 64 bit implementation, I used SIMD instructions.

## What was on my mind

- Starting to think that this extended bitslicing approach (so 64 blocks in parallel) is not only going to be much
  faster, but it's also going to be easier to implement than what I did last semester. Because now every slice_t can be
  viewed as 1 bit (conceptually) whereas with the previous approach, 1 slice_t represented 16 different bits, which caused
  trouble when implementing the round operations.
- Now that I have some experience with benchmarking on the cortex M4, I know bit slicing is actually faster. I'll
  implement everything on 64 bit with SIMD, and when I can encrypt a full block, then I'll do 64bit benchmarking using _
  rdtsc(), and see average execution time. My only worry is still that these simd instructions advertise to be faster,
  but I remember a single SIMD intrinsic needing hunderds of cycles back in november, but I didn't really know what I
  was doing yet, so we'll see.