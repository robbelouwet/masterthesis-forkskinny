## Weekly report 14 march - 19 march

# Tuesday

Migrated the whole implementation to a parameterized version using dynamic macro definitions of what a slice_internal is and how
2 slices should be xor'ed, and'ed, or'ed etc. In other words, can now run the implementation but define a slice_internal to be a
uint32_t, and then the whole implementation uses 32-bit slices, so we could run on arduino theoretically. A slice_internal can
also be a 256-bit or 512-bit simd register, but support for this isn't complete yet. 64-bit (with avx2 support) and
32-bit sliced implementation works.

# Wednesday

Implemented support for 256-bit SIMD slices. Also refactored the sbox implementation to use the macro logical
operations, and thus the s box circuit works with __m256i slices. THen i started implementing an accelerated version of
the slice_internal() and unslice_accelerated_internal() methods.

# Thursday

continued implementing the accelerated slice_internal operations, will only work for 64-bit and 256-bit slices though. __m512
datatypes do not have a permuyte or shift operation across lanes.

# Friday

## What was on my mind

