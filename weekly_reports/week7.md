## Weekly report 21 - 27 nov

### Monday

Created a new union representing a bit sliced state (either TK or internal state, they're both the same data
structure). Also implemented the slice() operation which takes in a normal State64_t input and outputs it in sliced
representation (State64Sliced_t). The real state itself is now a struct, that also contains 4 pointers to the words
containing all the most significant bits, the 3rd most significant bits, the 2nd least significant bits and the least
significant bits. In this way, performing a rotation is only re-assigning addresses to those pointers and is supposed
to be very, very cheap.

## Wednesday

## Thursday

### What was on my mind

- How will my bitsliced State64Sliced_t data structure translate to the fixed slicing technique as shown in the paper?
- Before continuing with all this, compare a bit-sliced LSFR on State64Sliced_t with the original LSFR.
