## Weekly report 28 nov - 4 dec

## Monday

The bitsliced LSFR performed its rotation using 4 move instructions, but I changed it so that it performs the
rotate-C1 in 1 instruction using SIMD. Then I tried to benchmark again, but sort of hit a wall on this benchmarking.
How I do it now is: I use intel's RDTSC instruction to give me the current 'cycle count'. This doesn't really work
because that includes cycles spent by other cores on different programs, and instruction pipelining also messes this up.
So the benchmark results are really noisy, undeterministic and unpredictable, especially on code that uses barely a few
cycles. I mailed Erik and Amit to ask how researchers at cosic time the execution of their protocols.

## Wednesday

Updated the bit sliced_fghi implementation to incorporate a x86 rotate-C1 instruction, instead of manually moving all
4
slices. This causes the vanilla bit-slice_t LSFR to be faster than the one that uses bit slicing + SIMD.
Shifted gears w.r.t. measuring the 'speed'. Measurements are now based and concluded on the amount and sort
of assembly instructions are generated. Started to build a small rapport which goes over the comparison of all 3 LSFR's
and its generated x86 instructions, comparing speed etc. The reason that this is a good idea, is that there
is [a paper](https://www.agner.org/optimize/instruction_tables.pdf)
which researched cycles spent per instruction on a lot of CPU's.
The results are promising, a bit-sliced_fghi LSFR calculating 16 cells in parallel needs 6 simple instructions, with
only 2
of them moving from stack to register. It also only needs 16 bits in space inside the register (i.e.: we could do a lot
more in parallel). Whereas the previous
sequential_abcde LSFR could only do 8 cells in parallel using 12 instructions while occupying all 64 bits in a register.
Afterwards, it appeared my unslice_internal operation wasn't fully correct, so need to fix this tomorrow (shouldn't affect the
aforementioned benchmarking results).

## Thursday

Started implementing the 4-bit sbox. Again it relies on a logical circuit which needs a rotate instruction, so it seems
promising that bit slicing will offer speedups.

### What was on my mind

- In theory, my bitsliced LSFR + SIMD should be much faster. You can even clearly see that it produces a lot less
  instructions in the executable dump. A quick inspection of how many cycles every instruction needs, would convince
  everybody that the bit sliced_fghi lsfr *should* be faster. But when I, for example, execute both the old
  sequential_abcde and
  bitsliced lsfr in a loop with 1000 iterations, and I then take the medium, the sequential_abcde one is a lot faster.
  But
  when I do it unrolled without a loop (e.g. 9 times each), the bit sliced_fghi lsfr is a lot faster in total. Even
  running
  those 9 LSFR runs multiple times, the bit sliced_fghi one appears to be faster (not always because of cycle noise by
  other
  cores etc.) I think
  this is because of overhead that the for loop gives, because the loop iterator needs to be checked, incremented and
  cached etc. on every iteration, but this is just a guess.
- Measuring execution time exactly w.r.t. amount of cpu cycles can be done if I switch to a 32 bit arduino or
  something, but setting up this device would take time, and development would progress slower (less convenient
  development environment). Since the functions we
  want to measure are ridiculously simple and small, I found it best to just inspect the amount and the sort of
  instructions generated. Later on, we can move to 32-bit or even 8-bit microcontrollers. SIMD also doesn't appear to
  offer and extra speedup at the moment, the problem is not that it's slow, it's that the vanilla bit sliced lsfr
  is ridiculously fast (6 instructions for up to 64 cells!).

