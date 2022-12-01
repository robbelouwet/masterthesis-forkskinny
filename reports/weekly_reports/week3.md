## Weekly report 24 - 30 october

### Monday

Read a paper on bit slicing and implementing fixed slicing in [AES](https://eprint.iacr.org/2020/1123.pdf) and
[SKINNY](https://www.esat.kuleuven.be/cosic/events/silc2020/wp-content/uploads/sites/4/2020/10/Submission6.pdf).
Started going in depth on Erik's code and did some refactoring to make it more readable in order to be able to transform
it into using
fixed slicing later on (but first understanding his code).

### Wednesday

Bit slicing (fixed slicing) and SIMD instructions appear to go hand-in-hand, so I looked at SIMD instructions first.
there appear to be multiple intrinsic libraries (?). A limited but broadly adopted gcc vector extensions intrinsic
library of GCC,
and an intel-specific but highly performant intrinsic 'xmmintrin.h'. Both these libraries facilitate SIMD instructions.
I'm starting to conceptually understand how bit slicing works and how this goes hand-in-hand with SIMD, but it's far
from evident.
Looked at how Fixed Slicing was first done with the [GIFT cipher paper](https://eprint.iacr.org/2020/412.pdf).

### Thursday

In order to perform bit slicing (later on fixed slicing), the inputs have to be rearranged to be put in their bitslice
representation. This is made possible with the `SWAPMOVE` technique, described
in [this paper, section 3](https://eprint.iacr.org/2020/412.pdf). Trying to figure out how to apply this to forkskinny.
Section 2.3
of [this paper](https://csrc.nist.gov/CSRC/media/Events/lightweight-cryptography-workshop-2020/documents/papers/fixslicing-lwc2020.pdf)
describes how the tweakey schedule of SKINNY could be greatly optimized with fixed slicing, but they switch up the order
of the LFSR and PT steps when updating the key schedule in the AddRoundKey step, so not sure if this impacts security.
Although they don't mention it. This issue basically comes down to whether or not `LSFR(PT(x))`
and `PT(LSFR(x))` offer equivalent security, whether it's ok to switch up their order.
Created an access token for elena to clone the repo as a workaround for not having an @kuleuven.be email address (this
is needed to log in to gitlab.kuleuven.be)

### What was on my mind

- How can I efficiently understand and play around with this bit slicing? Immediately implementing it into the cipher
  (that's not my code) might get cumbersome and I might not see the woods for the trees anymore. Maybe I can implement a
  simple standalone s-box, time the cycles, implement bit slicing and compare?
- It often happens that I don't know which trail to follow first or which path is a dead-end rabbit hole.
- Erik's code has a lot of conditional #if macro's for checking if we're running on a 32 or 64 bit platform.
  Thinking about splitting everything up to 2 seperate x32 and x64 files and remove all those checks to improve
  readability
  In other words, check the macro once, if we're running on x32, inject the x32 source file, else the x64 source file.
  Other than that, the 2 files should be identical