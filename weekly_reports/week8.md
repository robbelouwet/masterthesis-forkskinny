## Weekly report 28 nov - 4 dec

### Monday

The bitsliced LSFR performed its rotation using 4 move instructions, but I changed it so that it performs the
rotate-left in 1 instruction using SIMD. Then I tried to benchmark again, but sort of hit a wall on this benchmarking.
How I do it now is: I use intel's RDTSC instruction to give me the current 'cycle count'. This doesn't really work
because that includes cycles spent by other cores on different programs, and instruction pipelining also messes this up.
So the benchmark results are really noisy, undeterministic and unpredictable, especially on code that uses barely a few
cycles. I mailed Erik and Amit to ask how researchers at cosic time the execution of their protocols.

## Wednesday

## Thursday

### What was on my mind

- In theory, my bitsliced LSFR + SIMD should be much faster. You can even clearly see that it produces a lot less
  instructions in the executable dump. A quick inspection of how many cycles every instruction needs, would convince
  everybody that the bit sliced lsfr *should* be faster. But when I, for example, execute both the old sequential and
  bitsliced lsfr in a loop with 1000 iterations, and I then take the medium, the sequential one is a lot faster. But
  when I do it unrolled without a loop (e.g. 9 times each), the bit sliced lsfr is a lot faster in total. Even running
  those 9 LSFR runs multiple times, the bit sliced one appears to be faster (not always because of cycle noise by other
  cores etc.) I think
  this is because of overhead that the for loop gives, because the loop iterator needs to be checked, incremented and
  cached etc. on every iteration, but this is just a guess.

