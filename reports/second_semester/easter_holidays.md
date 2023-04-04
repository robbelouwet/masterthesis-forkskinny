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
Concerning the theses: I migrated everything to overleaf. Statrted from my faculty's template and started adding texts
and the titles that generate the table of contents.