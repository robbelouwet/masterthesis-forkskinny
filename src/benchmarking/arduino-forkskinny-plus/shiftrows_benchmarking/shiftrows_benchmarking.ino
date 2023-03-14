/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */

/*!< DWT Control register */
#define KIN1_DWT_CONTROL (*((volatile uint32_t *)0xE0001000))

/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNTENA_BIT (1UL << 0)

/*!< DWT Cycle Counter register */
#define KIN1_DWT_CYCCNT (*((volatile uint32_t *)0xE0001004))

/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_DEMCR (*((volatile uint32_t *)0xE000EDFC))

/*!< Trace enable bit in DEMCR register */
#define KIN1_TRCENA_BIT (1UL << 24)

/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */
#define KIN1_InitCycleCounter() \
  KIN1_DEMCR |= KIN1_TRCENA_BIT

/*!< Reset cycle counter */
#define KIN1_ResetCycleCounter() \
  KIN1_DWT_CYCCNT = 0

/*!< Enable cycle counter */
#define KIN1_EnableCycleCounter() \
  KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT

/*!< Disable cycle counter */
#define KIN1_DisableCycleCounter() \
  KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT

/*!< Read cycle counter register */
#define KIN1_GetCycleCounter() \
  KIN1_DWT_CYCCNT

typedef union {
  uint8_t halves[2];
  uint16_t value;
} Slice_t;

typedef union {
  Slice_t slices[4];  // [p³o³...b³a³, ... , p⁰o⁰...b⁰a⁰], where there are 16 4-bit cells named a through p
  uint64_t state;
  //__m64 m64state;  // SIMD 64-bit register
} State64Sliced2_16_t;

typedef union {
  uint16_t slices[4];  // [p³o³...b³a³, ... , p⁰o⁰...b⁰a⁰], where there are 16 4-bit cells named a through p
  uint64_t state;
  //__m64 m64state;  // SIMD 64-bit register
} State64Sliced_16_t;

/**
 * Union that describes a 64-bit 4x4 array of cells16.
 */
typedef union {
  uint16_t row[4];   /// Stored as 4x 16-bit uint partitions (cccc cccc cccc cccc) [c = 4-bit state slices]
  uint32_t lrow[2];  /// Stored as 2x 32-bit uint partitions  (cccccccc cccccccc)
  uint64_t llrow;    /// Stored as 1x 64-bit uint 			  (cccccccccccccccc)
} State64_t;

/**
 * For every nibble, take the ith significant bit and pack those all together.
 *
 * E.g.:
 *       v    v    v
 * x = 1000 1010 0000
 * i = 1 = slice_t the 2nd LSB of every nibble (0-based index)
 * output: 010
 *
 * @param x: the input to slice_t
 * @param m: 0 <= m <= 3; the 0-based index that masks the relevant significant bit of every slices.
 * @return the input in a bit sliced_fghi manner
 */
inline uint32_t slice_index(uint32_t x, uint8_t i) __attribute__((always_inline));
uint32_t slice_index(uint32_t x, uint8_t i) {
  // because uint4_t doesn't exist
  assert(i <= 3);

  // the mask for the bit, on nibble level (so a 4-bit mask that will be shifted)
  uint64_t m = 1 << i;

  return ((x & (m << 28)) >> 21 >> i) | ((x & (m << 24)) >> 18 >> i) | ((x & (m << 20)) >> 15 >> i) | ((x & (m << 16)) >> 12 >> i) | ((x & (m << 12)) >> 9 >> i) | ((x & (m << 8)) >> 6 >> i) | ((x & (m << 4)) >> 3 >> i) | ((x & m) >> i);
}

/**
 * Place all the equally significant bits packed in *value*, back in their nibble on their correct significant position.
 * I.e., every bit of *value* gets projected to a nibble. More specifically it gets projected to the
 * ith significant bit of each nibble.
 *
 * e.g.:
 * 	value: 0000 0000 0000 1111
 * 	i: 1 (0-based index)
 *
 * 	output: 0000 (0000)¹¹ 0010 0010 0010 0010
 * 		-> the 4 ones of *value* each get projected to the second-LSD in a separate nibble
 *
 * @param value: the one of the 4 packed input rows of 16 bits in bit sliced_fghi representation
 * @param i: 0-based index to identify to which significant bit of every nibble needs to be projected.
 * @return
 */
inline uint32_t unslice_index(uint8_t value, uint8_t i) __attribute__((always_inline));
uint32_t unslice_index(uint8_t value, uint8_t i) {
  // because uint4_t doesn't exist
  assert(i <= 3);

  uint64_t x = value;

  return ((x & 0x1) << i)
         | ((x & 0x2) << i << 3)
         | ((x & 0x4) << i << 6)
         | ((x & 0x8) << i << 9)
         | ((x & 0x10) << i << 12)
         | ((x & 0x20) << i << 15)
         | ((x & 0x40) << i << 18)
         | ((x & 0x80) << i << 21);
}

inline uint32_t slice(uint32_t state) __attribute__((always_inline));
uint32_t slice(uint32_t state) {
  return slice_index(state, 0)
         | (slice_index(state, 1) << 8)
         | (slice_index(state, 2) << 16)
         | (slice_index(state, 3) << 24);
}

inline uint32_t unslice(uint32_t state) __attribute__((always_inline));
uint32_t unslice(uint32_t state) {
  return unslice_index((state & 0x000000FF), 0)
         | unslice_index(((state & 0x0000FF00) >> 8), 1)
         | unslice_index(((state & 0x00FF0000) >> 16), 2)
         | unslice_index(((state & 0xFF000000) >> 24), 3);
}

uint16_t const row1_4bit_aligned[] = {
  0x000,
  0x800,
  0x100,
  0x900,
  0x200,
  0xa00,
  0x300,
  0xb00,
  0x400,
  0xc00,
  0x500,
  0xd00,
  0x600,
  0xe00,
  0x700,
  0xf00,
};

uint16_t const row2_4bit_aligned[] = {
  0x00,
  0x40,
  0x80,
  0xc0,
  0x10,
  0x50,
  0x90,
  0xd0,
  0x20,
  0x60,
  0xa0,
  0xe0,
  0x30,
  0x70,
  0xb0,
  0xf0,
};

uint16_t const row3_4bit_aligned[] = {
  0x0,
  0x2,
  0x4,
  0x6,
  0x8,
  0xa,
  0xc,
  0xe,
  0x1,
  0x3,
  0x5,
  0x7,
  0x9,
  0xb,
  0xd,
  0xf,
};

uint8_t const row0_row1[] = { 0x0, 0x8, 0x1, 0x9, 0x2, 0xa, 0x3, 0xb, 0x4, 0xc, 0x5, 0xd, 0x6,
                              0xe, 0x7, 0xf, 0x10, 0x18, 0x11, 0x19, 0x12, 0x1a, 0x13, 0x1b, 0x14,
                              0x1c, 0x15, 0x1d, 0x16, 0x1e, 0x17, 0x1f, 0x20, 0x28, 0x21, 0x29, 0x22,
                              0x2a, 0x23, 0x2b, 0x24, 0x2c, 0x25, 0x2d, 0x26, 0x2e, 0x27, 0x2f, 0x30,
                              0x38, 0x31, 0x39, 0x32, 0x3a, 0x33, 0x3b, 0x34, 0x3c, 0x35, 0x3d, 0x36,
                              0x3e, 0x37, 0x3f, 0x40, 0x48, 0x41, 0x49, 0x42, 0x4a, 0x43, 0x4b, 0x44,
                              0x4c, 0x45, 0x4d, 0x46, 0x4e, 0x47, 0x4f, 0x50, 0x58, 0x51, 0x59, 0x52,
                              0x5a, 0x53, 0x5b, 0x54, 0x5c, 0x55, 0x5d, 0x56, 0x5e, 0x57, 0x5f, 0x60,
                              0x68, 0x61, 0x69, 0x62, 0x6a, 0x63, 0x6b, 0x64, 0x6c, 0x65, 0x6d, 0x66,
                              0x6e, 0x67, 0x6f, 0x70, 0x78, 0x71, 0x79, 0x72, 0x7a, 0x73, 0x7b, 0x74,
                              0x7c, 0x75, 0x7d, 0x76, 0x7e, 0x77, 0x7f, 0x80, 0x88, 0x81, 0x89, 0x82,
                              0x8a, 0x83, 0x8b, 0x84, 0x8c, 0x85, 0x8d, 0x86, 0x8e, 0x87, 0x8f, 0x90,
                              0x98, 0x91, 0x99, 0x92, 0x9a, 0x93, 0x9b, 0x94, 0x9c, 0x95, 0x9d, 0x96,
                              0x9e, 0x97, 0x9f, 0xa0, 0xa8, 0xa1, 0xa9, 0xa2, 0xaa, 0xa3, 0xab, 0xa4,
                              0xac, 0xa5, 0xad, 0xa6, 0xae, 0xa7, 0xaf, 0xb0, 0xb8, 0xb1, 0xb9, 0xb2,
                              0xba, 0xb3, 0xbb, 0xb4, 0xbc, 0xb5, 0xbd, 0xb6, 0xbe, 0xb7, 0xbf, 0xc0,
                              0xc8, 0xc1, 0xc9, 0xc2, 0xca, 0xc3, 0xcb, 0xc4, 0xcc, 0xc5, 0xcd, 0xc6,
                              0xce, 0xc7, 0xcf, 0xd0, 0xd8, 0xd1, 0xd9, 0xd2, 0xda, 0xd3, 0xdb, 0xd4,
                              0xdc, 0xd5, 0xdd, 0xd6, 0xde, 0xd7, 0xdf, 0xe0, 0xe8, 0xe1, 0xe9, 0xe2,
                              0xea, 0xe3, 0xeb, 0xe4, 0xec, 0xe5, 0xed, 0xe6, 0xee, 0xe7, 0xef, 0xf0,
                              0xf8, 0xf1, 0xf9, 0xf2, 0xfa, 0xf3, 0xfb, 0xf4, 0xfc, 0xf5, 0xfd, 0xf6,
                              0xfe, 0xf7, 0xff };

uint8_t const row2_row3[] = { 0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x1, 0x3, 0x5, 0x7, 0x9,
                              0xb, 0xd, 0xf, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x41,
                              0x43, 0x45, 0x47, 0x49, 0x4b, 0x4d, 0x4f, 0x80, 0x82, 0x84, 0x86, 0x88,
                              0x8a, 0x8c, 0x8e, 0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d, 0x8f, 0xc0,
                              0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xc1, 0xc3, 0xc5, 0xc7, 0xc9,
                              0xcb, 0xcd, 0xcf, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x11,
                              0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f, 0x50, 0x52, 0x54, 0x56, 0x58,
                              0x5a, 0x5c, 0x5e, 0x51, 0x53, 0x55, 0x57, 0x59, 0x5b, 0x5d, 0x5f, 0x90,
                              0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0x91, 0x93, 0x95, 0x97, 0x99,
                              0x9b, 0x9d, 0x9f, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde, 0xd1,
                              0xd3, 0xd5, 0xd7, 0xd9, 0xdb, 0xdd, 0xdf, 0x20, 0x22, 0x24, 0x26, 0x28,
                              0x2a, 0x2c, 0x2e, 0x21, 0x23, 0x25, 0x27, 0x29, 0x2b, 0x2d, 0x2f, 0x60,
                              0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x61, 0x63, 0x65, 0x67, 0x69,
                              0x6b, 0x6d, 0x6f, 0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xa1,
                              0xa3, 0xa5, 0xa7, 0xa9, 0xab, 0xad, 0xaf, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8,
                              0xea, 0xec, 0xee, 0xe1, 0xe3, 0xe5, 0xe7, 0xe9, 0xeb, 0xed, 0xef, 0x30,
                              0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x31, 0x33, 0x35, 0x37, 0x39,
                              0x3b, 0x3d, 0x3f, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e, 0x71,
                              0x73, 0x75, 0x77, 0x79, 0x7b, 0x7d, 0x7f, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8,
                              0xba, 0xbc, 0xbe, 0xb1, 0xb3, 0xb5, 0xb7, 0xb9, 0xbb, 0xbd, 0xbf, 0xf0,
                              0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0xf1, 0xf3, 0xf5, 0xf7, 0xf9,
                              0xfb, 0xfd, 0xff };

/// f(x) = rotate_right(x, 1) << 8
inline uint16_t ror_row1_4bit_aligned(uint8_t input) __attribute__((always_inline));
uint16_t ror_row1_4bit_aligned(uint8_t input) {
  uint16_t row1_4bit_aligned[] = {
    0x000,
    0x800,
    0x100,
    0x900,
    0x200,
    0xa00,
    0x300,
    0xb00,
    0x400,
    0xc00,
    0x500,
    0xd00,
    0x600,
    0xe00,
    0x700,
    0xf00,
  };
  return row1_4bit_aligned[input];
}

/// f(x) = rotate_right(x, 2) << 4
inline uint16_t ror_row2_4bit_aligned(uint8_t input) __attribute__((always_inline));
uint16_t ror_row2_4bit_aligned(uint8_t input) {
  uint16_t row2_4bit_aligned[] = {
    0x00,
    0x40,
    0x80,
    0xc0,
    0x10,
    0x50,
    0x90,
    0xd0,
    0x20,
    0x60,
    0xa0,
    0xe0,
    0x30,
    0x70,
    0xb0,
    0xf0,
  };
  return row2_4bit_aligned[input];
}

/// f(x) = rotate_right(x, 3)
inline uint16_t ror_row3_4bit_aligned(uint8_t input) __attribute__((always_inline));
uint16_t ror_row3_4bit_aligned(uint8_t input) {
  uint16_t row3_4bit_aligned[] = {
    0x0,
    0x2,
    0x4,
    0x6,
    0x8,
    0xa,
    0xc,
    0xe,
    0x1,
    0x3,
    0x5,
    0x7,
    0x9,
    0xb,
    0xd,
    0xf,
  };
  return row3_4bit_aligned[input];
}

inline void shift_rows_sliced_lookup_half_slices(State64Sliced2_16_t *state) __attribute__((always_inline));
void shift_rows_sliced_lookup_half_slices(State64Sliced2_16_t *state) {


  auto slice0 = state->slices[0];
  auto slice1 = state->slices[1];
  auto slice2 = state->slices[2];
  auto slice3 = state->slices[3];

  slice0.halves[0] = row2_row3[slice0.halves[0]];
  slice0.halves[1] = row0_row1[slice0.halves[1]];

  slice1.halves[0] = row2_row3[slice1.halves[0]];
  slice1.halves[1] = row0_row1[slice1.halves[1]];

  slice2.halves[0] = row2_row3[slice2.halves[0]];
  slice2.halves[1] = row0_row1[slice2.halves[1]];

  slice3.halves[0] = row2_row3[slice3.halves[0]];
  slice3.halves[1] = row0_row1[slice3.halves[1]];

  state->slices[0] = slice0;
  state->slices[1] = slice1;
  state->slices[2] = slice2;
  state->slices[3] = slice3;
}

inline void shift_rows_sliced_lookup(State64Sliced_16_t *state) __attribute__((always_inline));
void shift_rows_sliced_lookup(State64Sliced_16_t *state) {
  auto slice0 = state->slices[0];
  auto slice1 = state->slices[1];
  auto slice2 = state->slices[2];
  auto slice3 = state->slices[3];

  state->slices[0] = (slice0 & 0xF000)
                     | row1_4bit_aligned[(slice0 & 0x0F00) >> 8]
                     | row2_4bit_aligned[(slice0 & 0x00F0) >> 4]
                     | row3_4bit_aligned[slice0 & 0x000F];

  state->slices[1] = (slice1 & 0xF000)
                     | row1_4bit_aligned[(slice1 & 0x0F00) >> 8]
                     | row2_4bit_aligned[(slice1 & 0x00F0) >> 4]
                     | row3_4bit_aligned[slice1 & 0x000F];

  state->slices[2] = (slice2 & 0xF000)
                     | row1_4bit_aligned[(slice2 & 0x0F00) >> 8]
                     | row2_4bit_aligned[(slice2 & 0x00F0) >> 4]
                     | row3_4bit_aligned[slice2 & 0x000F];

  state->slices[3] = (slice3 & 0xF000)
                     | row1_4bit_aligned[(slice3 & 0x0F00) >> 8]
                     | row2_4bit_aligned[(slice3 & 0x00F0) >> 4]
                     | row3_4bit_aligned[slice3 & 0x000F];
}

inline void shift_rows_sliced_packed_state(State64Sliced_16_t *state) __attribute__((always_inline));
void shift_rows_sliced_packed_state(State64Sliced_16_t *state) {
  auto slice0 = state->slices[0];
  auto slice1 = state->slices[1];
  auto slice2 = state->slices[2];
  auto slice3 = state->slices[3];

  state->slices[0] = (slice0 & 0xF000)
                     | (((slice0 & 0x0E00) >> 1) | ((slice0 & 0x0100) << 3))
                     | (((slice0 & 0x00C0) >> 2) | ((slice0 & 0x0030) << 2))
                     | (((slice0 & 0x0008) >> 3) | ((slice0 & 0x0007) << 1));

  state->slices[1] = (slice1 & 0xF000)
                     | (((slice1 & 0x0E00) >> 1) | ((slice1 & 0x0100) << 3))
                     | (((slice1 & 0x00C0) >> 2) | ((slice1 & 0x0030) << 2))
                     | (((slice1 & 0x0008) >> 3) | ((slice1 & 0x0007) << 1));

  state->slices[2] = (slice2 & 0xF000)
                     | (((slice2 & 0x0E00) >> 1) | ((slice2 & 0x0100) << 3))
                     | (((slice2 & 0x00C0) >> 2) | ((slice2 & 0x0030) << 2))
                     | (((slice2 & 0x0008) >> 3) | ((slice2 & 0x0007) << 1));

  state->slices[3] = (slice3 & 0xF000)
                     | (((slice3 & 0x0E00) >> 1) | ((slice3 & 0x0100) << 3))
                     | (((slice3 & 0x00C0) >> 2) | ((slice3 & 0x0030) << 2))
                     | (((slice3 & 0x0008) >> 3) | ((slice3 & 0x0007) << 1));
}

inline void shift_rows_sliced_packed_rows(State64Sliced_16_t *state) __attribute__((always_inline));
void shift_rows_sliced_packed_rows(State64Sliced_16_t *state) {
  // @formatter:off
  auto slice0 = state->slices[0];
  auto slice1 = state->slices[1];
  auto slice2 = state->slices[2];
  auto slice3 = state->slices[3];

  // Split up every slice_t into 4 rows each
  // So if slice0 contains 16 bits, then the first 4 bits of slice0 correspond to the most significant bit
  // of the first 4 cells (-> MSB because it's the first slice_t).
  // By this analogy, pack all the bits of the same row over every slice_t next to each other.
  uint16_t first_rows = ((slice0 & 0x0F00) << 4) | (slice1 & 0x0F00) | ((slice2 & 0x0F00) >> 4) | ((slice3 & 0x0F00) >> 8);
  uint16_t second_rows = ((slice0 & 0x00F0) << 8) | ((slice1 & 0x00F0) << 4) | (slice2 & 0x00F0) | ((slice3 & 0x00F0) >> 4);
  uint16_t third_rows = ((slice0 & 0x000F) << 12) | ((slice1 & 0x000F) << 8) | ((slice2 & 0x000F) << 4) | (slice3 & 0x000F);

  // now perform the shift row (ror = rotate-C0)
  first_rows = ((first_rows >> 1) & 0x7777) | ((first_rows << 3) & 0x8888);     // ror 1
  second_rows = ((second_rows >> 2) & 0x3333) | ((second_rows << 2) & 0xCCCC);  // ror 2
  third_rows = ((third_rows >> 3) & 0x1111) | ((third_rows << 1) & 0xEEEE);     // ror 3

  // reconstruct the slices
  state->slices[0] = (slice0 & 0xF000) | ((first_rows & 0xF000) >> 4) | ((second_rows & 0xF000) >> 8) | ((third_rows & 0xF000) >> 12);
  state->slices[1] = (slice1 & 0xF000) | (first_rows & 0x0F00) | ((second_rows & 0x0F00) >> 4) | ((third_rows & 0x0F00) >> 8);
  state->slices[2] = (slice2 & 0xF000) | ((first_rows & 0x00F0) << 4) | (second_rows & 0x00F0) | ((third_rows & 0x00F0) >> 4);
  state->slices[3] = (slice3 & 0xF000) | ((first_rows & 0x000F) << 8) | ((second_rows & 0x000F) << 4) | (third_rows & 0x000F);
  //@formatter:on
}

inline void shift_rows_old(State64_t *state) __attribute__((always_inline));
void shift_rows_old(State64_t *state) {
  // reversed order because of endianness
  // MSB        -- llrow --        LSB
  // row[0] | row[1] | row[2] | row[3]
  auto row0 = state->row[0];
  auto row1 = state->row[1];
  auto row2 = state->row[2];

  state->row[2] = (row2 >> 4) | (row2 << 12);  // skinny64_rotate_right(state->row[1], 4);
  state->row[1] = (row1 >> 8) | (row1 << 8);   // skinny64_rotate_right(state->row[2], 8);
  state->row[0] = (row0 >> 12) | (row0 << 4);  // skinny64_rotate_right(state->row[3], 12);
}

void run() {
  KIN1_InitCycleCounter(); /* enable DWT hardware */

  uint64_t state = (((uint64_t)rand()) << 32) | ((uint64_t)rand());  // 2⁶⁴
  Serial.print("State: ");
  Serial.println(state);

  auto vanilla = State64_t();
  vanilla.llrow = state;

  // slice_t the state
  auto sliced_packed_rows = State64Sliced_16_t();
  sliced_packed_rows.state = slice(state);
  auto sliced_packed_state = State64Sliced_16_t();
  sliced_packed_state.state = slice(state);
  auto sliced_lookup = State64Sliced_16_t();
  sliced_lookup.state = slice(state);
  auto sliced_lookup_8bit = State64Sliced2_16_t();
  sliced_lookup_8bit.state = slice(state);


  // shift rows vanilla
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  shift_rows_old(&vanilla);
  auto cycles_vanilla = KIN1_GetCycleCounter();

  // shift rows, aligning & packing rows
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  shift_rows_sliced_packed_rows(&sliced_packed_rows);
  auto cycles_packed_rows = KIN1_GetCycleCounter();

  // shift rows on full slices
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  shift_rows_sliced_packed_state(&sliced_packed_state);
  auto cycles_packed_state = KIN1_GetCycleCounter();

  // shift rows using 4-bit lookups
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  shift_rows_sliced_lookup(&sliced_lookup);
  auto cycles_lookup = KIN1_GetCycleCounter();

  // shift rows using 8-bit lookups and avoiding alignment through half slices
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  shift_rows_sliced_lookup_half_slices(&sliced_lookup_8bit);
  auto cycles_lookup_8bit = KIN1_GetCycleCounter();

  auto unsliced_packed_rows = unslice(sliced_packed_rows.state);
  auto unsliced_lookup = unslice(sliced_lookup.state);
  auto unsliced_lookup_8bit = unslice(sliced_lookup_8bit.state);


  Serial.print("Old shift rows: ");
  Serial.print(vanilla.llrow);
  Serial.print(", cycles: ");
  Serial.println(cycles_vanilla);

  Serial.print("bit sliced, packed rows: ");
  Serial.print(unsliced_packed_rows);
  Serial.print(", cycles: ");
  Serial.println(cycles_packed_rows);

  Serial.print("bit sliced, 4-bit lookups: ");
  Serial.print(unsliced_lookup);
  Serial.print(", cycles: ");
  Serial.println(cycles_lookup);

  Serial.print("bit sliced, 8-bit data section lookup without slice_t alignment: ");
  Serial.print(unsliced_lookup_8bit);
  Serial.print(", cycles: ");
  Serial.println(cycles_lookup_8bit);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  Serial.println("#####################################");
  run();
  delay(1000);
}