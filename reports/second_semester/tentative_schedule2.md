It is now April 13th

what the implementation supports:

tested: forkskinny64-192, forkskinny128-256, forkskinny128-384
probably works as well: fs64-128, fs128-192

what is left to do:
- Benchmarking
- PAEF
  - literature study still to be done!
- R-PAEF
  - literature study still to be done!

what I've already written about in the theses:
- Preface (half a page)
- Implementation aspects (3 pages): 
  - bit slicing (60%)
  - Memory layout (80%)
- Efficiency & speedup (1 page)
  - fixsliced keyschedule (50%)


What I'd like:
- Preface: thanks & context
- Abstract: 1 paragraph explanation of the theses
- Terminology & Abbreviations
- Introduction ENG (2 pages): Research question, thesis structure, goal of theses
- Introduction NL (2) (mandated by faculty)
- Previous work (2 pages): Arne Deprez, Antoon Purnal, Erik
- Implementation Aspects (20 pages)
  - Bit Slicing (5 pages) What? How? Why does it work? When should it be used? SIDE CHANNEL RESISTENCE?
    - intro etc.: what and how does it work
    - performance & when should it be used
    - side channel resistence
    - The slice and unslice operations
  - SIMD (8 pages)
    - What is it? How does it work? When should it be used? Why SIMD isn't always faster
    - compiler intrinsics & platform support -> SSE vs AVX2 vs AVX512
  - Efficient memory access & Optimizations (4 pages) What? How? Why?
    - Union Structs
    - Cipher & Tweakey state
- ForkSKINNY+ (18 pages) (The actual implementation, how it compares to others and what makes it different)
  - Existing implementations (re-used techniques -> union structs erik!)
  - Combining Bit Slicing with SIMD -> 128-, 256- or even 512 bit slices
    - Segmentation: Accelerating the slice & unslice operation -> How could it be optimized? accelerating it through SIMD itself again? Rotation for faster alignment?
    - Balancing slice size & operation speedup
  - Fixed Slicing
  - Design (8 pages)
    - Key schedule -> pre-computed!
    - Round function -> AddConstant inside key schedule!
- AEAD modes (3 pages)
  - PAEF
- Benchmarking (5 pages)
  - Slice Configuration (1 pages)
  - Results (4 pages + figures!!!)
- Conclusion (2 pages)
- Future Work (2 page)
  - Applications where 512 bit slices could really be used
  - More efficient balance between large slice & operation speedup
  - Accelerate the slice & unslice operations -> application-specific implementations leave ciphertext in sliced state?

----> total: +- 56 pages (of core theses material, no appendices)