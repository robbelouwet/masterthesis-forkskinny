## Weekly report 24 april - april 30

# Monday

Looked into the memory alignment for SIMD. Turns out that unaligned loads from the stack are still faster than aligned
loads from the heap, so I'm leaving it as-is. SIMD loads are unaligned (but addresses are 32-byte aligned still).
I then looked into the segment slicing again. Although I think this could potentially give a big speedup, I decided (a
few hours later) not to pursue it, because it would require a lot of changes in the implementation and I don't have time
for this. Tomorrow I want to look at a last final idea to optimize the slice and unslice operation, but wednesday I'm
definitely starting on PAEF and s-PAEF modes, hopefully I can implement these in a few days, maybe a week tops, so that
I can have the last 4-3 weeks to fully focus on writing the theses.

# Tuesday

Started working on PAEF. Before noon, I read up on how it works, I find the pseudo-code protocol in the paper extremely
abstract. THe drawing on the 'design' section of the ForkAE website makes it much more clear. I also sent an email to
Antoon Prunal with a few questions on his PAEF implementation. After noon I started implementing it.

# Wednesday

Continued working on PAEF. I can almost encrypt, but it will most likely not be the same as Antoon's implementation
because there's some ambiguity
in how one should implement the protocol. There's no standardization, only a formal specification, which leaves room for
interpretation on some aspects when implementing it. For example with PAEF in fs64-192, suppose the nonce is 64 bits and
the counter as well, where exactly do I put the bit flags? Do I format like {N || f⁰f¹f² || 0⁶¹ || ctr} or do I format
like {N || 0⁶¹ || f⁰f¹f² || ctr}? Also, how much of a buffer do you specify for the counter, because when the counter
has value 1, it's only 1 bit long, but when we reach 128th block, the ctr value is now 8 bits long, so how much space do
you provide as buffer for the counter? Again, things like these are ambiguities that only arise when you start
implementing. For now I went with this approach: {N || f⁰f¹f² || 0⁶¹ || ctr} and designate TK3 for the counter value (so
counter has TK3 as 64-bit buffer to hold its value). This does mean that you can only encrypt 2048 petabyte (2^64).

# Saturday

Continued implementing PAEF. I've implemented the first part where the associated data blocks are encrypted, but I still
need to test.

# Sunday

PAEF-forkskinny-64-192 (encryption only) implemented. PAEF-AD encryption takes less cycles than vanilla primitive
encryption, because we slice less key material. Apparently I messed up my benchmarking calculations, it appears the
implementation is a lot slower than I thought. Gonna have to look into this, still have my idea to accelerate slicing.