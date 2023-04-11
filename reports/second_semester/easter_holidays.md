## Monday, april 3rd

Started to *really* implement the fixslicing key schedule. The fixslicing paper says that if you take the original
keyschedule and
apply the lfsr's to the whole state instead of the top 2 rows alone, then that should produce the same result as the
fixslicing schedule, so I will use this to verify correctness. The fixslicing keyschedule uses multiple PT's applied to
each other, which can get really confusing because I swap the nibbles in the state and so permutation lanes also have to
be swapped. I'm going to adjust the
implementation and undo this 'nibble swapping' because it just makes everything more complicated and less readable. If
in the end it appears that we indeed want to swap the order s.t. nibbles are specified from left to right, I'll just
encode like that it before encrypting. Otherwise, cells of the state are swapped and so all operations need to be
implemented differently, which gets really confusing. Also emailed Erik on a potential bug in his code.

## Tuesday, april 4th

continued writing the theses. I didn't work on code because I'm waiting on Erik's response on whether or not the
ShiftRows implementation is actually wrong. This would mean that Erik's test vectors aren't correct.
Concerning the theses: I migrated everything to overleaf. Started from my faculty's template and started adding texts
and the titles that generate the table of contents.

## Friday, april 7th

I changed my mind again and decided to stay with the swapped nibbles implementation. This makes things a tidy bit more
complicated, but I already had implemented almost everything, so it didn't really make sense to change the
implementation. I also FINALLY implemented and fixed the fixsliced keyschedule for fs64 and it produces the correct
round keys. I
didn't realize this, but the fixedslicing keyschedule actually *does* produce the same roundkeys, I initially thought
this wasn't the case. In the beginning I thought this fixed slicing key schedule was going to derive completely
different round keys than the original schedule, but this isn't the case. I drew it out on paper and now I fully
understand why and how it works.

## Monday, april 10th

Fixed/implemented the fixsliced keyschedule for fs128, which was a different implementation than with fs64. fixslicing
for fs128 needs different permutations because there's no nibble swapping. I then started reading on the PAEF mode and
study how it works so I can hopefully start implementing this first AEAD mode before the end of the easter holidays.

## Tuesday, april 11th

A long day today, for some reason I introduced a lot of bugs into the code the last couple of days, but I never noticed
this. TO start with, I bugged the demo code of Erik, it was not producing the correct test vectors. This was due to an
overflow where the extra 0x2 at the key injection step was cast from uint64 bit to uint32 bit and thus
overflown and not added. Next, I was adding forkskinny128 branch constant in forkskinny64 and it wasn't being added
correctly when not using 64-bit acceleration (only first 4 slices per cell were added, was left over when copying from
fs64 add_bc step). Then I created a function that produces sliced test vectors and returned them by reference, but this
introduced a segfault because the vectors were created by value on the stack, but the wrapping vector was passing their
addresses, essentially causing some kkind of use-after-free when accessing the cipher state in the round operations.

The good news is I added config for 8 bit and 128 bit slices, so can now run the implementation on a 8-bit
microcontroller (technically, although there still are some literal 'uint64_t' occurances in the code) and slices can
now also be 128 bit (which is what you want, because then 1 run of the code
encrypts 128 * 64 bits, which is 1 kilobyte, which is more or less the desired ct length for forkskinny according to Amit)