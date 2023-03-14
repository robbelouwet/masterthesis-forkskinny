## Weekly report 14 - 20 nov

### Monday

Fixed the LSFR SIMD implementation to now only apply on the top 2 rows. Still, this implementation needs 500 - 3000
cycles, while a SIMD-less sequential_abcde implementation only needs around 50. I thought maybe the problem was with the CPU
having to load the state after evey single SIMD instruction, but after debugging the register, I concluded the SIMD
values never leave the SIMD register after an instruction.

## Wednesday

Look into a simple example of what bit slicing is, and looked at comparing a single round in forkskinny and skinny+.

## Thursday 
Came up with an idea to bit-slice_t the tweakey in order to perform the same LSFR on all cells in parallel,
trying this out (on paper first).

### What was on my mind

- Starting to think that SIMD on its own is not going to give any speedups. The fact that state cells are only 4-bit
  and there are only 16 cells gives us too little data to fully use the parallelization that 256-bit registers offer.
  For example, if the tweakey state consisted of 16 cells of 16 bits each, then this would nicely fit in a simd
  register. But the fact that the state is only 16x 4-bit values, and because the LSFR and PT can be described in simple
  logical gates, already makes the tweakey keys super cheap without SIMD.
- Thinking about leaving SIMD for what it is, and looking at bit slicing and then the fixed-slicing technique.
- Looking at bit-slicing the key-keys