## Weekly report 21 - 27 nov

### Monday

Created a new union representing a bit sliced_fghi state (either TK or internal state, they're both the same data structure).
Also implemented the slice_t() operation which takes in a normal State64_t input and outputs it in sliced_fghi representation (
State64Sliced_16_t). The real state itself is now a struct, that also contains 4 pointers to the words containing all
the most significant bits, the 3rd most significant bits, the 2nd least significant bits and the least significant bits.
In this way, performing a rotation is only re-assigning addresses to those pointers and is supposed to be very, very
cheap.

## Wednesday

Implemented the reverse operation unslice128_accelerated_internal() and benchmarked a bit sliced_fghi LSFR with the sequential_abcde one. The bit sliced_fghi
one seems faster. when executing the bitsliced LSFR 5 times it takes about 25 cycles for all 5 runs. TIf I execute the
sequential_abcde LSFR 5 times, it takes about 50 cycles.

## Thursday

Ran the benchmark again and noticed the results were again about the same. Spent a lot of time, but in the end I found
out that my IDE had problems caching and every time I clicked 'run' it used an old executable. It wasn't recompiling
every time. Now I solved it, I literally looked at the assembly code, saw that indeed first 5 bit sliced_fghi LSFR's were
being run, and then 5 sequential_abcde LSFR's. the bit sliced_fghi runs in +- 50 instructions, the sequential_abcde ones need +-100
instructions (or whatever unit the RDTSC instruction represents, probably the amount of instructions, not cycles).

### What was on my mind

- How will my bitsliced State64Sliced_16_t data structure translate to the fixed slicing technique as shown in the
  paper?
- Before continuing with all this, compare a bit-sliced_fghi LSFR on State64Sliced_16_t with the original LSFR.
- Regarding the report on wednesday: when I try to execute both LSFRs in a loop of 1000 iterations, the results are
  really mixed, there is no clear speedup. But when I run both LSFRs 5 times after each other (so no for loop), the bit
  sliced_fghi one is clearly faster, I wonder why. Maybe it has to do with caching the loop iterator and incrementing it etc.
  Maybe that's why a loop iteration brings overhead in addition to the LSFR call and thus brings noise in the cycle
  count?
- There is a lot of variance in speed when comparing the sequential_abcde and bit-sliced_fghi LSFR. I'm going to have to make a
  habit of inspecting the assembly before concluding benchmarking results, because I can't trust my IDE to be
  deterministic in recompiling every time I click 'run'.
