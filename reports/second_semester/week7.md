## Weekly report 17 april - 23 april

# Monday

wrote a bit in theses, edited text on bit slicing and memory layout. Then started benchmarking the code, no results yet.

# Tuesday

Started on benchmarking the code. Very simple: run 1000 iterations and subtract timestamp before and after, divide by
1000 then divide by slice_size (amount of blocks in parallel), this gives cycles per primitive call. Results weren't
super good, so got an idea on how to increase performance of the sbox.

# Wednesday

Performance increase on sbox didn't turn out to be good enough. I have some trouble with interpreting benchmarking
results of other SKINNY implementations. [This implementation](https://github.com/kste/skinny_avx) claims to require
only a couplu of cpu cycles per byte! Wth?? So 1 byte going through 36 rounds, needs a total of around 4 cycles? So a
single plaintext byte needs 4/36=0.11 cycles per round opertion? So a round operation costs 0.11 * 8 bytes = 0.88
cycles? This seems really fshy, I think I'm misinterpreting this? Maybe they mean 4 cycles per byte PER ROUND? This
would make much more sense. Anyway, Arne Deprez mentions in [his paper](https://eprint.iacr.org/2020/1295.pdf) a single
skinny64 round function needing about 97 cycles, which is about the same as Erik's implementation if I remember
correctly. I don't want to jump to conclusions, but at the moment a single round only needs 7-15 cycles in my
implementation :)))

# Sunday

Before noon, I installed a [benchmarking tool](https://github.com/google/benchmark) to analyze cpu usage and inspect
call stacks and see what symbols/functions require the most execution time. You can find the results under
src/benchmarking/results. It's quite astonishing to see that a tremendous amount of execution time is spent on loading
unaligned memory into or from AVX2 vectors. When inspecting the perf output, you really see a lot of execution time and
percentage goes to the '__memmove_avx_unaligned_erms' symbol, which is called by libc's 'memcpy' and 'memmove' to
optimize loading unaligned SIMD vectors. In other words, I'm going to have to refactor where and how I load the cipher state during
the rounds. I already knew that SIMD really requires 32-byte alignment for fast optimizations, but I need to think on
how I'm going to apply this.

## What was on my mind