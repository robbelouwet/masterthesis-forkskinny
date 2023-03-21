#ifndef FORKSKINNYPLUS_FIXSLICING_PERMUTATIONS_H
#define FORKSKINNYPLUS_FIXSLICING_PERMUTATIONS_H

// <editor-fold desc="PT 2"
#define PT_2(input, output) {   \
output.cells[0x0] = input.cells[0x6];   \
output.cells[0x1] = input.cells[0x0];   \
output.cells[0x2] = input.cells[0x4];   \
output.cells[0x3] = input.cells[0x1];   \
output.cells[0x4] = input.cells[0x7];   \
output.cells[0x5] = input.cells[0x3];   \
output.cells[0x6] = input.cells[0x2];   \
output.cells[0x7] = input.cells[0x5];   \
output.cells[0x8] = input.cells[0xe];   \
output.cells[0x9] = input.cells[0x8];   \
output.cells[0xa] = input.cells[0xc];   \
output.cells[0xb] = input.cells[0x9];   \
output.cells[0xc] = input.cells[0xf];   \
output.cells[0xd] = input.cells[0xb];   \
output.cells[0xe] = input.cells[0xa];   \
output.cells[0xf] = input.cells[0xd];   }
// </editor-fold>

// <editor-fold desc="PT 4"
#define PT_4(input, output) {   \
output.cells[0x0] = input.cells[0x2];   \
output.cells[0x1] = input.cells[0x6];   \
output.cells[0x2] = input.cells[0x7];   \
output.cells[0x3] = input.cells[0x0];   \
output.cells[0x4] = input.cells[0x5];   \
output.cells[0x5] = input.cells[0x1];   \
output.cells[0x6] = input.cells[0x4];   \
output.cells[0x7] = input.cells[0x3];   \
output.cells[0x8] = input.cells[0xa];   \
output.cells[0x9] = input.cells[0xe];   \
output.cells[0xa] = input.cells[0xf];   \
output.cells[0xb] = input.cells[0x8];   \
output.cells[0xc] = input.cells[0xd];   \
output.cells[0xd] = input.cells[0x9];   \
output.cells[0xe] = input.cells[0xc];   \
output.cells[0xf] = input.cells[0xb];   }
// </editor-fold>

// <editor-fold desc="PT 6"
#define PT_6(input, output) {   \
output.cells[0x0] = input.cells[0x4];   \
output.cells[0x1] = input.cells[0x2];   \
output.cells[0x2] = input.cells[0x5];   \
output.cells[0x3] = input.cells[0x6];   \
output.cells[0x4] = input.cells[0x3];   \
output.cells[0x5] = input.cells[0x0];   \
output.cells[0x6] = input.cells[0x7];   \
output.cells[0x7] = input.cells[0x1];   \
output.cells[0x8] = input.cells[0xc];   \
output.cells[0x9] = input.cells[0xa];   \
output.cells[0xa] = input.cells[0xd];   \
output.cells[0xb] = input.cells[0xe];   \
output.cells[0xc] = input.cells[0xb];   \
output.cells[0xd] = input.cells[0x8];   \
output.cells[0xe] = input.cells[0xf];   \
output.cells[0xf] = input.cells[0x9];   }
// </editor-fold>

// <editor-fold desc="PT 8"
#define PT_8(input, output) {   \
output.cells[0x0] = input.cells[0x7];   \
output.cells[0x1] = input.cells[0x4];   \
output.cells[0x2] = input.cells[0x3];   \
output.cells[0x3] = input.cells[0x2];   \
output.cells[0x4] = input.cells[0x1];   \
output.cells[0x5] = input.cells[0x6];   \
output.cells[0x6] = input.cells[0x5];   \
output.cells[0x7] = input.cells[0x0];   \
output.cells[0x8] = input.cells[0xf];   \
output.cells[0x9] = input.cells[0xc];   \
output.cells[0xa] = input.cells[0xb];   \
output.cells[0xb] = input.cells[0xa];   \
output.cells[0xc] = input.cells[0x9];   \
output.cells[0xd] = input.cells[0xe];   \
output.cells[0xe] = input.cells[0xd];   \
output.cells[0xf] = input.cells[0x8];   }
// </editor-fold>

// <editor-fold desc="PT 10"
#define PT_10(input, output) {   \
output.cells[0x0] = input.cells[0x5];   \
output.cells[0x1] = input.cells[0x7];   \
output.cells[0x2] = input.cells[0x1];   \
output.cells[0x3] = input.cells[0x4];   \
output.cells[0x4] = input.cells[0x0];   \
output.cells[0x5] = input.cells[0x2];   \
output.cells[0x6] = input.cells[0x3];   \
output.cells[0x7] = input.cells[0x6];   \
output.cells[0x8] = input.cells[0xd];   \
output.cells[0x9] = input.cells[0xf];   \
output.cells[0xa] = input.cells[0x9];   \
output.cells[0xb] = input.cells[0xc];   \
output.cells[0xc] = input.cells[0x8];   \
output.cells[0xd] = input.cells[0xa];   \
output.cells[0xe] = input.cells[0xb];   \
output.cells[0xf] = input.cells[0xe];   }
// </editor-fold>

// <editor-fold desc="PT 12"
#define PT_12(input, output) {   \
output.cells[0x0] = input.cells[0x3];   \
output.cells[0x1] = input.cells[0x5];   \
output.cells[0x2] = input.cells[0x0];   \
output.cells[0x3] = input.cells[0x7];   \
output.cells[0x4] = input.cells[0x6];   \
output.cells[0x5] = input.cells[0x4];   \
output.cells[0x6] = input.cells[0x1];   \
output.cells[0x7] = input.cells[0x2];   \
output.cells[0x8] = input.cells[0xb];   \
output.cells[0x9] = input.cells[0xd];   \
output.cells[0xa] = input.cells[0x8];   \
output.cells[0xb] = input.cells[0xf];   \
output.cells[0xc] = input.cells[0xe];   \
output.cells[0xd] = input.cells[0xc];   \
output.cells[0xe] = input.cells[0x9];   \
output.cells[0xf] = input.cells[0xa];   }
// </editor-fold>

// <editor-fold desc="PT 14"
#define PT_14(input, output) {   \
output.cells[0x0] = input.cells[0x1];   \
output.cells[0x1] = input.cells[0x3];   \
output.cells[0x2] = input.cells[0x6];   \
output.cells[0x3] = input.cells[0x5];   \
output.cells[0x4] = input.cells[0x2];   \
output.cells[0x5] = input.cells[0x7];   \
output.cells[0x6] = input.cells[0x0];   \
output.cells[0x7] = input.cells[0x4];   \
output.cells[0x8] = input.cells[0x9];   \
output.cells[0x9] = input.cells[0xb];   \
output.cells[0xa] = input.cells[0xe];   \
output.cells[0xb] = input.cells[0xd];   \
output.cells[0xc] = input.cells[0xa];   \
output.cells[0xd] = input.cells[0xf];   \
output.cells[0xe] = input.cells[0x8];   \
output.cells[0xf] = input.cells[0xc];   }
// </editor-fold>

#endif //FORKSKINNYPLUS_FIXSLICING_PERMUTATIONS_H
