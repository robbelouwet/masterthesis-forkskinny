## Weekly report 24 - 30 october
### Monday
Read a paper on bit slicing and implementing fixed slicing in [AES](https://eprint.iacr.org/2020/1123.pdf) and
[SKINNY](https://www.esat.kuleuven.be/cosic/events/silc2020/wp-content/uploads/sites/4/2020/10/Submission6.pdf).
Started going in depth on Erik's code and did some refactoring to make it more readable in order to be able to transform it into using
fixed slicing later on (but first understanding his code). 


### What was on my mind
- How can I efficiently understand and play around with this bit slicing? Immediately implementing it into the cipher 
(that's not my code) might get cumbersome and I might not see the woods for the trees anymore. Maybe I can implement a
simple standalone s-box, time the cycles, implement bit slicing and compare?
- Too many approaches, I want to try too many things at once.
- Erik's code has a lot of conditional #if macro's for checking if we're running on a 32 or 64 bit platform. 
Thinking about splitting everything up to 2 seperate x32 and x64 files and remove all those checks to improve readability
In other words, check the macro once, if we're running on x32, inject the x32 source file, else the x64 source file. Other than that, the 2 files should be identical