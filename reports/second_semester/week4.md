## Weekly report 14 march - 19 march

# Tuesday

Migrated the whole implementation to a parameterized version using dynamic macro definitions of what a slice is and how
2 slices should be xor'ed, and'ed, or'ed etc. In other words, can now run the implementation but define a slice to be a
uint32_t, and then the whole implementation uses 32-bit slices, so we could run on arduino theoretically. A slice can
also be a 256-bit or 512-bit simd register, but support for this isn't complete yet. 64-bit (with avx2 support) and
32-bit sliced implementation works.

# Wednesday

# Thursday

# Friday

## What was on my mind

