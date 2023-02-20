## Weekly report 20 - 26 february

# Monday

Completed the fixsliced keyschedule, but don't yet know how to test it. The paper of SKINNY provides a test vector +
Erik has demo code to test against, but this isn't the case with the fixslicing keyschedule, can't find a test vector in
the paper. Thinking about cloning their repo and hoping that we both compute the same tks, but if this doesn't work, the
problem could lie anywhere because my implementation is not exactly the same of course.
I could mail the authors of the fixslicing paper and ask for a test vector if they had one?

# Thursday

# Friday

## What was on my mind

- I should read more code of existing bitsliced implementations, but they are very cumbersome to read. I usually
  skim some code I think I can learn from and I can see what they are trying to do, but it's a whole other story seeing
  exactly *how* they're trying to do it, especially because bit-slicing and SIMD produce very obscure code by nature. On
  top of that, a lot of those repositories from lwc candidates and rweather etc. implement bit-slicing and/or simd in a
  slightly different way than I do, so it's also difficult to compare correctness with the fixslicing tks for example.
  The authors of fixslicing use bitslicing with only 2 blocks in parallel, for example. Or they organize their data
  structure of how the key and cipher state is laid out differently, and so the code is much different from mine as
  well.