## Weekly report 8 may - 14 may

# Monday

So I implemented both Ideas and there is a speed up but it's still way too slow. I think the problem is that my
implementation oftwen switches from SIMD to general-purpose registers. Like this whole concept of ambiguous data (with
this union structs, that you can access it as simd lane or as array of 4 ints) actually bears a disadvantage because
when you do such a swap, it needs to leave the SIMD register and get stored in mem, THEN loaded into GP registers and
takes up a lot of cpu cycles. I'm adjusting the
implementation such that when running 64-bit with AVX acceleration, the key and cipher state are accessed ONLY as SIMD
registers. By doing this, the states will remain in simd registers throughout the whole primitive call, and I think this
might get rid of the massive slowdown that I experienced up until now. If this doesn't work then so be it, it's may 8th
and I have only written 10 pages of my theses, so still need to do that.
Together with this segmentation idea, the sbox is already in simd-only format, am now converting the vanilla key
schedule to simd-only.