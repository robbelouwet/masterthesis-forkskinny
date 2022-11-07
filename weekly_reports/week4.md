## Weekly report 31 oct - 06 nov

## Monday

Took a step back and went over
the [FixSlicing SKINNY](https://csrc.nist.gov/CSRC/media/Events/lightweight-cryptography-workshop-2020/documents/papers/fixslicing-lwc2020.pdf)
paper again. Researched on how to tell the compiler to use data parallelization/SIMD, and not do it ourselves. Made a
CMakePresets.json file.
researched and tested out gcc compiler flags to suggest SIMD and vectorization optimisations to the compiler.

## Wednesday
Sick

## Thursday

Continued working on making the compiler use vectorization. Trying to find tutorial examples that should make it use
vectorization and tell me when compiling, no success so far.
I found the correct compiler flag, I already found the flag `-fopt-info` to show this info, but apparantly this changed
and the vectorization info is now shown through `-fopt-info-internals`. Also found where this info is displayed in my
IDE.
I'm starting to think that auto-vectorization isn't a good idea, because you're basically hoping the compiler is smart
enough to recognize the fixed-slice representation and vectorizes it. Intel and AMD both support the AVX and AVX2
intrinsics, so thinking about implementing it directly. This shouldn't be too hard because the LSFR's and permutation
both can be described with logical gates, which goes very well with the provided intrinsic library's instructions.

### What was on my mind

- For the moment, I'm setting my mind on [Erik's code](/libs/forkskinny-c) and thinking about implementing
  fixed slicing with SIMD in the key schedule.
  I don't want to spend too much time on trying to analyze what the romulus skinny+ implementation has done to get those
  speedups, that rabbit hole is just too deep, I don't have time for that because I still need to look into
  parallelizing the 2 branches after splitting. And I still need to do the modes.
- Thinking about how to force SIMD instructions. Using the intrinsic libraries directly might get cumbersome and most
  likely will cause problems on machines that work with slightly different SIMD instructions (e.g. not every cpu
  supports the AVX extensions). I'm thinking about *only*
  vectorizing the code into a bit-sliced representation, and not enforcing anything further, but let the compiler decide
  on the low-level SIMD data parallelization. We can suggest to the compiler to use SIMD with compiler flags, so this
  should work. This is a much better approach I think, because we decouple the code from the platform by letting the
  compiler decide.
