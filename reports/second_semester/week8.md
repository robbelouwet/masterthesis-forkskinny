## Weekly report 24 april - april 30

# Monday

Looked into the memory alignment for SIMD. Turns out that unaligned loads from the stack are still faster than aligned
loads from the heap, so I'm leaving it as-is. SIMD loads are unaligned (but addresses are 32-byte aligned still).
I then looked into the segment slicing again. Although I think this could potentially give a big speedup, I decided (a
few hours later) not to pursue it, because it would require a lot of changes in the implementation and I don't have time
for this. Tomorrow I want to look at a last final idea to optimize the slice and unslice operation, but wednesday I'm
definitely starting on PAEF and s-PAEF modes, hopefully I can implement these in a few days, maybe a week tops, so that
I can have the last 4-3 weeks to fully focus on writing the theses.