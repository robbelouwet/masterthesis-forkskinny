## Weekly report 14 - 20 nov

### Monday

Fixed the LSFR SIMD implementation to now only apply on the top 2 rows. Still, this implementation needs 500 - 3000
cycles, while a SIMD-less sequential implementation only needs around 50. I thought maybe the problem was with the CPU
having to load the state after evey single SIMD instruction, but after debugging the register, I concluded the SIMD
values never leave the SIMD register after an instruction.


## Wednesday
1) look into a simple example of what bit slicing is
2) try to use romulus skinny-128-384 and construct forkskinny-128-384
   1) How to extend the LSFR to 7 bits at the AddConstants step?
   2) after this works, maybe look at how to extend implementation to forkskinny-64-128?

### What was on my mind

- Starting to think that SIMD on its own is not going to give any speedups. The fact that state cells are only 4-bit
  and there are only 16 cells gives us too little data to fully use the parallelization that 256-bit registers offer.
  For example, if the tweakey state consisted of 16 cells of 16 bits each, then this would nicely fit in a simd
  register. But the fact that the state is only 16x 4-bit values, and because the LSFR and PT can be described in simple
  logical gates, already makes the tweakey schedule super cheap without SIMD.
- Thinking about leaving SIMD for what it is, and looking at bit slicing and then the fixed-slicing technique.