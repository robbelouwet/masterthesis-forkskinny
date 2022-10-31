## Weekly report 31 oct - 06 nov

## Monday

Took a step back and went over
the [FixSlicing SKINNY](https://csrc.nist.gov/CSRC/media/Events/lightweight-cryptography-workshop-2020/documents/papers/fixslicing-lwc2020.pdf)
paper again. Researched on how to tell the compiler to use data parallelization/SIMD, and not do it ourselves. Made a CMakePresets.json file.
researched and tested out gcc compiler flags to suggest SIMD and vectorization optimalisations to the compiler

### What was on my mind

- For the moment, I'm setting my mind on [Erik's code](/contrib/forkskinny-c) and implementing basic fixed slicing with
  SIMD in the key schedule.
  I don't want to spend too much time on trying to analyze what the romulus implementation has done to get those
  speedups, that rabbit hole is just too deep, I don't have time for that because I still need to look into
  parallelizing the 2 branches after splitting. And I still need to do the modes.
- Thinking about how to force SIMD instructions. Using the intrinsic libraries directly might get cumbersome and most
  likely will cause problems on machines that work with slightly different SIMD instructions (e.g. not every cpu
  supports the AVX extensions). I'm thinking about *only*
  vectorizing the code into a bit-sliced representation, and not enforcing anything further, but let the compiler decide
  on the low-level SIMD data parallelization. We can suggest to the compiler to use SIMD with compiler flags, so this
  should work. This is a much better approach I think, because we decouple the code from the platform by letting the
  compiler decide.
