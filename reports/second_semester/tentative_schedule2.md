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
- Preface: thanks & context (1 page)
- Abstract: 1 paragraph explanation of the theses (1 page)
- Terminology & Abbreviations (2 pages?)
- Introduction (8 pages): Research question, previous work, thesis structure, goal of theses
- Implementation Aspects (20 pages)
  - Bit Slicing (8 pages) What? How? Why does it work? When should it be used? SIDE CHANNEL RESISTENCE?
    - intro etc.: what and how does it work
    - performance & when should it be used
    - side channel resistence
    - The slice and unslice operations
  - SIMD (8 pages)
    - What ist it? How does it work? When should it be used?
    - compiler intrinsics & platform support -> SSE vs AVX2 vs AVX512
  - Efficient memory access (4 pages) What? How? Why?
    - Union Structs
    - Cipher & Tweakey state
- ForkSKINNY++ (10 pages)
  - SKINNY+ & existing implementations (re-used techniques -> union structs erik!) 
  - Combining Bit Slicing with SIMD -> 128-, 256- or even 512 bit slices
    - Accelerating the slice & unslice operation -> How could it be optimized? accelerating it through SIMD itself again? Rotation for faster alignment?
    - Balancing slice size & operation speedup
  - Fixed Slicing
- AEAD modes (10 pages)
  - PAEF
  - R-PAEF
- Benchmarking (10 pages)
  - Slice Configuration (2 pages)
  - Results (8 pages + figures!!!) (wij vs. results van SKINNY+)
- Conclusion (2 pages)
- Future Work (2 page)
  - Applications where 512 bit slices could really be used
  - More efficient balance between large slice & operation speedup
  - Accelerate the slice & unslice operations -> application-specific implementations leave ciphertext in sliced state?

----> total: +- 60 pages (of core theses material, no appendices)