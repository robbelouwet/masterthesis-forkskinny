## Weekly report 12 - 18 dec

## Monday

Worked on other deadlines.

## Wednesday

Started looking at how I could maybe benchmark the code on an arduino. I have one with ATMega 32u2 and ATMega 16u4, both
are 8-bit with 32 general purpose registers. I primarily looked at setting up the development environment and arduino
integration into Jetbrains CLion.

## Thursday

Worked on other deadlines, but I did manage to get my hands on a 32-bit Cortex M4 arduino!

### What was on my mind

- The more I think about it, the more I get convinced that I'm really going to get results, bit slicing seems promising.
  It seems promising for general speedups, but especially the memory advantage will allow us to compute both branches at
  once (with 32 bit registers) or even compute 2 full primitive calls at once (when using 64 bit registers)!
