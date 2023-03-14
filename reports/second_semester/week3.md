## Weekly report 06 march - 12 march

# Tuesday

It works! Can correctly compute a forkskinny64-192 block encryption with correct output (verified with Erik's
implementation). Normally, fs64-64, fs64-128 should also work in s=b mode, but haven't tested yet, this is for tomorrow.
Tomorrow:

- code cleanup: get rif of debug-related code and statements, unnecessary comments
- optimize: get rid of loops and see what else can be optimized
- decryption: implement all reverse operations

# Wednesday

Implemented all inverse operations of the round function and implemented decryption in all forwarding modes. Can now
encrypt and decrypt a fs64-192 block according to every 'forward' mode.

tomorrow:

- cleanup debug related code
- `#define` dynamic `slice_t` datatype together with its dynamic `xor_slice`, `and_slice` and `not_slice` operations so
  that you can dynamically tweak how many blocks you want to do in parallel and define how big your slices should be.
  This should allow for 1 single straightforward codebase instead of multiple implementations of the cipher for multiple
  register widths and slice_t sizes. This would also allow easily running the code with 32-bit slices and run in on the
  arduino.

# Thursday

# Friday

## What was on my mind

