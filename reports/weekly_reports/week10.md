## Weekly report 28 nov - 4 dec

## Monday

Worked on other deadlines.

## Wednesday

Started looking at how I could maybe benchmark the code on an arduino. I have one with ATMega 32u2 and ATMega 16u4, both
are 8-bit with 32 general purpose registers. I primarily looked at setting up the development environment and arduino
integration into Jetbrains CLion.

## Thursday

Started implementing the 4-bit sbox. Again it relies on a logical circuit which needs a rotate instruction, so it seems
promising that bit slicing will offer speedups.

### What was on my mind

- The more I think about it, the more I get convinced that I'm really going to get results, bit slicing seems promising.
  It seems promising for general speedups, but especially the memory advantage will allow us to compute both branches at
  once (with 32 bit registers) or even compute 2 full primitive calls at once (when using 64 bit registers)!
