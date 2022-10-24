## Weekly report 17 - 23 october

### Monday

Migrated the github repo into the KUL gitlab repo I was provided from my faculty and invited Elena and Amit. Fixed up
the structure of this repository, was struggling with getting the git submodules right. Cloning
with `--recurse-submodules` now clones the external repos as well.

# Wednesday

Looked at using Intel's Timestamp Counter Register to view current cpu cycles and maybe use it to benchmark the
primitive. Refactored the project to hard-include the external repositories (instead of git modules) so we can apply
changes to that code. This works now, skinny-plus is succesfully included as library and dynamically linked to our main
src code, so we can plug and play both libraries (etc. use SKINNY+ primitive from Romulus in Erik's ForkSKINNY
implementation?)

# Thursday

How can we make sure that CMake targets a 64-bit architecture?
https://cmake.org/pipermail/cmake/2007-February/012789.html.
Timed Erik's implementation using RDTSC innstruction

### What was on my mind

- Can we migrate the SKINNY+ implementation from rweather's Romelus cipher into Erik's ForkSkinny implementation to get
  practically free speedups?