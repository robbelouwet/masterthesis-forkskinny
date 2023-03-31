## Weekly report 27 march - 02 april

# Monday

Fixed the fs128 s-box, ShiftRows and AddConstant step. Sew it together and can now encrypt fs128-384 in s=b mode,
decryption is up next.

# Thursday

Fixed decryption and wrote some text for paper. Can now also decrypt fs128-384 in any mode. refactored some code, made a
draft schedule of what is left to do for the thesis, started to implement the unit tests to test correctness of every
primitive version in every mode and mailed with Erik.

# Friday

refactored main() startpoints to neat unit tests so everything is tested automatically. Also refactored the
forkskinny128.h file to allow a variable amount rounds so it can be reused by fs128-256 and fs128-384.

## What was on my mind

