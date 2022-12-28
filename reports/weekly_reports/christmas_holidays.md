## Report Christmas holidays

### Tuesday 27 dec

I got my hands on an Arduino Sense BLE 33 a few weeks ago, it has a Cortex M4 chip with a special register keeping score
of the current cycle count. Managed to set up PlatformIO IDE in VS Code, so I can write code in vanilla C++ instead of
the over simplified arduino IDE. Also managed to find a code snippet to actually poll the cycle count, seems to work.

### Wednesday 28 dec

I struggled a bit to find a suitable IDE/environment to write code for the arduino in vanilla c++, but decided it's not
worth my time and just stuck with the Arduino IDE. Managed to *finally* really benchmark the LSFR and sbox on the
arduino. A bit sliced sbox is faster: 16 vs 45 cycles, and the lsfr is faster as well: 14 vs 18 cycles. Measured on an
Arduino Sense BLE 33 (Cortex M4, 32 bit). Tomorrow I'll do the permutations and shiftrow.

## What's on my mind

- Now that I finally have results that allow me to sleep at night, I'll be able to start writing the end-to-end bit-sliced
  implementation next week, although I'll probably not be able to finish it before the next semester starts.