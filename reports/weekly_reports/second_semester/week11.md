## Weekly report 17 - 23 october

# Monday

Made tentative schedule for this semester and compared with Arne Deprez' work.

# Thursday

Implemented the new slice and unslice operation that slices and unslices 64x 64-bit states, and unslice correctly. This
works.

# Thursday

## What was on my mind

- Starting to think that this extended bitslicing approach (so 64 blocks in parallel) is not only going to be much
  faster, but it's also going to be easier to implement than what I did last semester. Because now every slice can be
  viewed as 1 bit (conceptually) whereas with the previous approach, 1 slice represented 16 different bits, which caused
  trouble when implementing the round operations.