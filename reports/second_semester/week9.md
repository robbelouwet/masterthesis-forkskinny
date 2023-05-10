## Weekly report 1 may - 7 may

# Monday

Wrote on thesis. Wrote some pages on SIMD an english version of introduction.

# Tuesday

Wrote on thesis. Wrote the dutch version of the introduction and started implementing the accelerated slicing as well,
seems to work.

# Wednesday, Thursday

Worked on accelerated slicing to use bit rotation on the blocks in order to align all equally significant bits.

# Thursday

Accelerated slicing works on 64-bit slices. Speedup from 900 cycles per block to just barely 150!!!

# Saturday

Fixed the accelerated slicing to work on all slice sizes. The speedup is more or less equivalent for other slice sizes.
I also started on the segmenting approach where I basically group multiple slices of the same significance relative to
their cell (so slice 0, 4, 8 and 12) and group them in a 'sqegment' which is basically a 256-bit SIMD register. By doing
this, I can significantly speed up the round operations. I refactored the code to adhere to the segmentation approach
and already implemented the s box and round key injection step in the segmented approach.