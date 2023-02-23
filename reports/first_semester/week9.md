## Weekly report 05 - 11 dec

## Monday

Implemented the S box implementation, it seems a bit faster but not that much. The primary gain lies in the space
advantage again, this is good because we could later on maybe calculate both branches in parallel. Also started to look
at the shiftrows and TK permutation. Starting to think that bit slicing's disadvantage is going to show in the
permutations.

## Wednesday
Worked on other deadlines.

## Thursday
Worked on other deadlines.

### What was on my mind

- Need to find a way to optimize permutations in bit sliced manner, because this is going to be more expensive than the
  previous sequential approach, at first sight.