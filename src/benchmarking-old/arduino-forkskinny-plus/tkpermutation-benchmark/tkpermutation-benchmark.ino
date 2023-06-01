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
  uint16_t slices[4];  // [p³o³...b³a³, ... , p⁰o⁰...b⁰a⁰], where there are 16 4-bit cells named a through p
  u64 state;
  //__m64 m64state;  // SIMD 64-bit register
} State64Sliced_16_t;

/**
 * Union that describes a 64-bit 4x4 array of cells16.
 */
typedef union {
  uint16_t row[4];   /// Stored as 4x 16-bit uint partitions (cccc cccc cccc cccc) [c = 4-bit state slices]
  uint32_t lrow[2];  /// Stored as 2x 32-bit uint partitions  (cccccccc cccccccc)
  u64 llrow;    /// Stored as 1x 64-bit uint 			  (cccccccccccccccc)
} State64_t;

/**
 * For every nibble, take the ith significant bit and unpack128 those all together.
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
  u64 m = 1 << i;

  return ((x & (m << 28)) >> 21 >> i) | ((x & (m << 24)) >> 18 >> i) | ((x & (m << 20)) >> 15 >> i) | ((x & (m << 16)) >> 12 >> i) | ((x & (m << 12)) >> 9 >> i) | ((x & (m << 8)) >> 6 >> i) | ((x & (m << 4)) >> 3 >> i) | ((x & m) >> i);
}

/**
 * Place all the equally significant bits packed in *raw*, back in their nibble on their correct significant position.
 * I.e., every bit of *raw* gets projected to a nibble. More specifically it gets projected to the
 * ith significant bit of each nibble.
 *
 * e.g.:
 * 	raw: 0000 0000 0000 1111
 * 	i: 1 (0-based index)
 *
 * 	output: 0000 (0000)¹¹ 0010 0010 0010 0010
 * 		-> the 4 ones of *raw* each get projected to the second-LSD in a separate nibble
 *
 * @param value: the one of the 4 packed input rows of 16 bits in bit sliced_fghi representation
 * @param i: 0-based index to identify to which significant bit of every nibble needs to be projected.
 * @return
 */
inline uint32_t unslice_index(uint8_t value, uint8_t i) __attribute__((always_inline));
uint32_t unslice_index(uint8_t value, uint8_t i) {
  // because uint4_t doesn't exist
  assert(i <= 3);

  u64 x = value;

  return ((x & 0x1) << i)
         | ((x & 0x2) << i << 3)
         | ((x & 0x4) << i << 6)
         | ((x & 0x8) << i << 9)
         | ((x & 0x10) << i << 12)
         | ((x & 0x20) << i << 15)
         | ((x & 0x40) << i << 18)
         | ((x & 0x80) << i << 21);
}

inline uint32_t slice_internal(const anonymous union blocks) __attribute__((always_inline));
uint32_t slice_internal(const anonymous union blocks) {
  return slice_index(blocks, 0)
         | (slice_index(blocks, 1) << 8)
         | (slice_index(blocks, 2) << 16)
         | (slice_index(blocks, 3) << 24);
}

inline uint32_t unslice_accelerated(uint32_t state) __attribute__((always_inline));
uint32_t unslice_accelerated(uint32_t state) {
  return unslice_index((state & 0x000000FF), 0)
         | unslice_index(((state & 0x0000FF00) >> 8), 1)
         | unslice_index(((state & 0x00FF0000) >> 16), 2)
         | unslice_index(((state & 0xFF000000) >> 24), 3);
}

inline void permute_sliced_packed_32(State64Sliced_16_t *tk) __attribute__((always_inline));
void permute_sliced_packed_32(State64Sliced_16_t *tk) {
  // avoid too much *tk address lookups by putting slices in local variables
  auto slice0 = tk->slices[0];
  auto slice1 = tk->slices[1];
  auto slice2 = tk->slices[2];
  auto slice3 = tk->slices[3];

  // unpack128 the bottom 2 rows of every slice_t in a single 32 bit word
  // i.e.: r3⁰r4⁰ | r3¹r4¹ | r3²r4² | r3³r4³
  uint32_t packed_slices =
    ((slice0 & 0x00FF) << 24)
    | ((slice1 & 0x00FF) << 16)
    | ((slice2 & 0x00FF) << 8)
    | (slice3 & 0x00FF);

  // perform permutation on the packed rows of the slices
  uint32_t packed_permuted_slices =
    ((packed_slices & 0xA8A8A8A8) >> 2)
    | ((packed_slices & 0x40404040) << 1)
    | ((packed_slices & 0x10101010) >> 4)
    | ((packed_slices & 0x04040404) << 2)
    | ((packed_slices & 0x02020202) << 1)
    | ((packed_slices & 0x01010101) << 6);

  // unpack128 results & reconstruct slices
  tk->slices[0] = ((packed_permuted_slices & 0xFF000000) >> 16) | ((slice0 & 0xFF00) >> 8);
  tk->slices[1] = ((packed_permuted_slices & 0x00FF0000) >> 8) | ((slice1 & 0xFF00) >> 8);
  tk->slices[2] = (packed_permuted_slices & 0x0000FF00) | ((slice2 & 0xFF00) >> 8);
  tk->slices[3] = ((packed_permuted_slices & 0x000000FF) << 8) | ((slice3 & 0xFF00) >> 8);
}

inline void permute_sliced_packed_16(State64Sliced_16_t *tk) __attribute__((always_inline));
void permute_sliced_packed_16(State64Sliced_16_t *tk) {
  // avoid too much *tk address lookups by putting slices in local variables
  auto slice0 = tk->slices[0];
  auto slice1 = tk->slices[1];
  auto slice2 = tk->slices[2];
  auto slice3 = tk->slices[3];

  // unpack128 the bottom 2 rows of the first 2 slices in 1 16-bit variable
  uint16_t slice_0_1 = ((slice0 & 0x00FF) << 8) | (slice1 & 0x00FF);

  // unpack128 the bottom 2 rows of the last 2 slices in 1 16-bit variable
  uint16_t slice_2_3 = ((slice2 & 0x00FF) << 8) | (slice3 & 0x00FF);

  // perform permutation on first 2 slices
  uint16_t permuted_slice_0_1 =     // source bits: 89ab cdef 89ab cdef
    ((slice_0_1 & 0xA8A8) >> 2)     //              1010 1000 1010 1000 | 0080 A0C0 0080 A0C0
    | ((slice_0_1 & 0x4040) << 1)   //              0100 0000 0100 0000 | 9080 A0C0 9080 A0C0
    | ((slice_0_1 & 0x1010) >> 4)   //              0001 0000 0001 0000 | 9080 A0CB 9080 A0CB
    | ((slice_0_1 & 0x0404) << 2)   //              0000 0100 0000 0100 | 9080 AECB 9080 AECB
    | ((slice_0_1 & 0x0202) << 1)   //              0000 0010 0000 0010 | 908D AECB 908D AECB
    | ((slice_0_1 & 0x0101) << 6);  //              0000 0001 0000 0001 | 9F8D AECB 9F8D AECB

  // perform permutation on last 2 slices
  uint16_t permuted_slice_2_3 =
    ((slice_2_3 & 0xA8A8) >> 2)     //              1010 1000 1010 1000 | 0080 A0C0 0080 A0C0
    | ((slice_2_3 & 0x4040) << 1)   //              0100 0000 0100 0000 | 9080 A0C0 9080 A0C0
    | ((slice_2_3 & 0x1010) >> 4)   //              0001 0000 0001 0000 | 9080 A0CB 9080 A0CB
    | ((slice_2_3 & 0x0404) << 2)   //              0000 0100 0000 0100 | 9080 AECB 9080 AECB
    | ((slice_2_3 & 0x0202) << 1)   //              0000 0010 0000 0010 | 908D AECB 908D AECB
    | ((slice_2_3 & 0x0101) << 6);  //              0000 0001 0000 0001 | 9F8D AECB 9F8D AECB

  // unpack128 results & reconstruct slices
  tk->slices[0] = (permuted_slice_0_1 & 0xFF00) | ((slice0 & 0xFF00) >> 8);
  tk->slices[1] = ((permuted_slice_0_1 & 0x00FF) << 8) | ((slice1 & 0xFF00) >> 8);
  tk->slices[2] = (permuted_slice_2_3 & 0xFF00) | ((slice2 & 0xFF00) >> 8);
  tk->slices[3] = ((permuted_slice_2_3 & 0x00FF) << 8) | ((slice3 & 0xFF00) >> 8);
}

// for every slice_t: PT[9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7] (source indices)
/* Permutation generated by http://programming.sirrida.de/calcperm.php */
inline void permute_sliced_circuit(State64Sliced_16_t *tk) __attribute__((always_inline));
void permute_sliced_circuit(State64Sliced_16_t *tk) {
  auto slice0 = tk->slices[0];
  auto slice1 = tk->slices[1];
  auto slice2 = tk->slices[2];
  auto slice3 = tk->slices[3];
  // --------------------------------------------------- source bits: Z123 4567 89ab cdef	| Cumulative raw
  tk->slices[0] = ((slice0 & 0xFF00) >> 8)      // masks:       1111 1111 0000 0000 | 0000 0000 Z123 4567
                  | ((slice0 & 0x00A8) << 6)    //              0000 0000 1010 1000 | 0080 A0C0 Z123 4567
                  | ((slice0 & 0x0040) << 9)    //              0000 0000 0100 0000 | 9080 A0C0 Z123 4567
                  | ((slice0 & 0x0010) << 4)    //              0000 0000 0001 0000 | 9080 A0CB Z123 4567
                  | ((slice0 & 0x0004) << 10)   //              0000 0000 0000 0100 | 9080 AECB Z123 4567
                  | ((slice0 & 0x0002) << 9)    //              0000 0000 0000 0010 | 908D AECB Z123 4567
                  | ((slice0 & 0x0001) << 14);  //              0000 0000 0000 0001 | 9F8D AECB Z123 4567

  // --------------------------------------------------- source bits: Z123 4567 89ab cdef	| Cumulative raw
  tk->slices[1] = ((slice1 & 0xFF00) >> 8)      // masks:       1111 1111 0000 0000 | 0000 0000 Z123 4567
                  | ((slice1 & 0x00A8) << 6)    //              0000 0000 1010 1000 | 0080 A0C0 Z123 4567
                  | ((slice1 & 0x0040) << 9)    //              0000 0000 0100 0000 | 9080 A0C0 Z123 4567
                  | ((slice1 & 0x0010) << 4)    //              0000 0000 0001 0000 | 9080 A0CB Z123 4567
                  | ((slice1 & 0x0004) << 10)   //              0000 0000 0000 0100 | 9080 AECB Z123 4567
                  | ((slice1 & 0x0002) << 9)    //              0000 0000 0000 0010 | 908D AECB Z123 4567
                  | ((slice1 & 0x0001) << 14);  //              0000 0000 0000 0001 | 9F8D AECB Z123 4567

  // --------------------------------------------------- source bits: Z123 4567 89ab cdef	| Cumulative raw
  tk->slices[2] = ((slice2 & 0xFF00) >> 8)      // masks:       1111 1111 0000 0000 | 0000 0000 Z123 4567
                  | ((slice2 & 0x00A8) << 6)    //              0000 0000 1010 1000 | 0080 A0C0 Z123 4567
                  | ((slice2 & 0x0040) << 9)    //              0000 0000 0100 0000 | 9080 A0C0 Z123 4567
                  | ((slice2 & 0x0010) << 4)    //              0000 0000 0001 0000 | 9080 A0CB Z123 4567
                  | ((slice2 & 0x0004) << 10)   //              0000 0000 0000 0100 | 9080 AECB Z123 4567
                  | ((slice2 & 0x0002) << 9)    //              0000 0000 0000 0010 | 908D AECB Z123 4567
                  | ((slice2 & 0x0001) << 14);  //              0000 0000 0000 0001 | 9F8D AECB Z123 4567

  // --------------------------------------------------- source bits: Z123 4567 89ab cdef	| Cumulative raw
  tk->slices[3] = ((slice3 & 0xFF00) >> 8)      // masks:       1111 1111 0000 0000 | 0000 0000 Z123 4567
                  | ((slice3 & 0x00A8) << 6)    //              0000 0000 1010 1000 | 0080 A0C0 Z123 4567
                  | ((slice3 & 0x0040) << 9)    //              0000 0000 0100 0000 | 9080 A0C0 Z123 4567
                  | ((slice3 & 0x0010) << 4)    //              0000 0000 0001 0000 | 9080 A0CB Z123 4567
                  | ((slice3 & 0x0004) << 10)   //              0000 0000 0000 0100 | 9080 AECB Z123 4567
                  | ((slice3 & 0x0002) << 9)    //              0000 0000 0000 0010 | 908D AECB Z123 4567
                  | ((slice3 & 0x0001) << 14);  //              0000 0000 0000 0001 | 9F8D AECB Z123 4567
}

void permute_sliced_step(State64Sliced_16_t *tk) __attribute__((always_inline));
void permute_sliced_step(State64Sliced_16_t *tk) {
  auto slice0 = tk->slices[0];
  auto slice1 = tk->slices[1];
  auto slice2 = tk->slices[2];
  auto slice3 = tk->slices[3];

  // save top 2 rows
  auto slice0_row_1_2 = ((slice0 & 0xFF00));
  auto slice1_row_1_2 = ((slice1 & 0xFF00));
  auto slice2_row_1_2 = ((slice2 & 0xFF00));
  auto slice3_row_1_2 = ((slice3 & 0xFF00));

  // save last 2 rows
  auto slice0_row_3_4 = (slice0 & 0x00FF);
  auto slice1_row_3_4 = (slice1 & 0x00FF);
  auto slice2_row_3_4 = (slice2 & 0x00FF);
  auto slice3_row_3_4 = (slice3 & 0x00FF);

  // PT[8, A, C]
  uint16_t res_slice0_row_3_4 = ((slice0_row_3_4 & 0b10101000) >> 2);
  uint16_t res_slice1_row_3_4 = ((slice1_row_3_4 & 0b10101000) >> 2);
  uint16_t res_slice2_row_3_4 = ((slice2_row_3_4 & 0b10101000) >> 2);
  uint16_t res_slice3_row_3_4 = ((slice3_row_3_4 & 0b10101000) >> 2);


  // PT[9]
  res_slice0_row_3_4 |= ((slice0_row_3_4 & 0b01000000) << 1);
  res_slice1_row_3_4 |= ((slice1_row_3_4 & 0b01000000) << 1);
  res_slice2_row_3_4 |= ((slice2_row_3_4 & 0b01000000) << 1);
  res_slice3_row_3_4 |= ((slice3_row_3_4 & 0b01000000) << 1);

  // PT[B]
  res_slice0_row_3_4 |= (slice0_row_3_4 & 0b00010000) >> 4;
  res_slice1_row_3_4 |= (slice1_row_3_4 & 0b00010000) >> 4;
  res_slice2_row_3_4 |= (slice2_row_3_4 & 0b00010000) >> 4;
  res_slice3_row_3_4 |= (slice3_row_3_4 & 0b00010000) >> 4;

  // PT[D]
  res_slice0_row_3_4 |= (slice0_row_3_4 & 0b00000100) << 2;
  res_slice1_row_3_4 |= (slice1_row_3_4 & 0b00000100) << 2;
  res_slice2_row_3_4 |= (slice2_row_3_4 & 0b00000100) << 2;
  res_slice3_row_3_4 |= (slice3_row_3_4 & 0b00000100) << 2;

  // PT[E]
  res_slice0_row_3_4 |= (slice0_row_3_4 & 0b00000010) << 1;
  res_slice1_row_3_4 |= (slice1_row_3_4 & 0b00000010) << 1;
  res_slice2_row_3_4 |= (slice2_row_3_4 & 0b00000010) << 1;
  res_slice3_row_3_4 |= (slice3_row_3_4 & 0b00000010) << 1;

  // PT[F]
  res_slice0_row_3_4 |= (slice0_row_3_4 & 0b00000001) << 6;
  res_slice1_row_3_4 |= (slice1_row_3_4 & 0b00000001) << 6;
  res_slice2_row_3_4 |= (slice2_row_3_4 & 0b00000001) << 6;
  res_slice3_row_3_4 |= (slice3_row_3_4 & 0b00000001) << 6;

  tk->slices[0] = (res_slice0_row_3_4 << 8) | (slice0_row_1_2 >> 8);
  tk->slices[1] = (res_slice1_row_3_4 << 8) | (slice1_row_1_2 >> 8);
  tk->slices[2] = (res_slice2_row_3_4 << 8) | (slice2_row_1_2 >> 8);
  tk->slices[3] = (res_slice3_row_3_4 << 8) | (slice3_row_1_2 >> 8);
}

void permute_old(State64_t *tk) __attribute__((always_inline));
void permute_old(State64_t *tk) {
  /* PT = [9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7] */
  /* Permutation generated by http://programming.sirrida.de/calcperm.php */
  //	uint32_t x = tk->lrow[1];
  //	tk->lrow[1] = tk->lrow[0];
  //	tk->lrow[0] = ((x & 0x0000000FU) << 4) |
  //	              ((x & 0x00F0F0F0U) << 8) |
  //	              ((x & 0x0F000000U) >> 24) |
  //	              ((x & 0x00000F00U) << 16) |
  //	              ((x & 0xF0000000U) >> 12) |
  //	              ((x & 0x000F0000U) >> 8);

  uint16_t row2 = tk->row[2];
  uint16_t row3 = tk->row[3];
  tk->row[2] = tk->row[0];
  tk->row[3] = tk->row[1];
  row3 = (row3 << 8) | (row3 >> 8);
  tk->row[0] = ((row2 << 4) & 0x00F0U) | ((row2 << 8) & 0xF000U) | (row3 & 0x0F0FU);
  tk->row[1] = ((row2 >> 8) & 0x00F0U) | (row2 & 0x0F00U) | ((row3 >> 4) & 0x000FU) | (row3 & 0xF000U);
}

void run(){
  KIN1_InitCycleCounter(); /* enable DWT hardware */

  u64 state = (((u64)rand()) << 32) | ((u64)rand());  // 2⁶⁴
  Serial.print("State: ");
  Serial.println(state);

  // ---- INT ----
  auto sliced_step = State64Sliced_16_t();
  sliced_step.state = slice_internal(state);

  auto sliced_circuit = State64Sliced_16_t();
  sliced_circuit.state = slice_internal(state);

  auto sliced_packed_16 = State64Sliced_16_t();
  sliced_packed_16.state = slice_internal(state);

  auto sliced_packed_32 = State64Sliced_16_t();
  sliced_packed_32.state = slice_internal(state);

  auto vanilla = State64_t();
  vanilla.llrow = state;
  // -------------

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  permute_old(&vanilla);
  auto cycles_old = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  permute_sliced_step(&sliced_step);
  auto cycles_step = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  permute_sliced_circuit(&sliced_circuit);
  auto cycles_circuit = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  permute_sliced_packed_16(&sliced_packed_16);
  auto cycles_packed_16 = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  permute_sliced_packed_32(&sliced_packed_32);
  auto cycles_packed_32 = KIN1_GetCycleCounter();

  Serial.print("Old permutation: ");
  Serial.print(vanilla.llrow);
  Serial.print(", cycles: ");
  Serial.println(cycles_old);

  Serial.print("Step permutation: ");
  Serial.print(sliced_step.state);
  Serial.print(", cycles: ");
  Serial.println(cycles_step);

  Serial.print("Circuit permutation: ");
  Serial.print(sliced_circuit.state);
  Serial.print(", cycles: ");
  Serial.println(cycles_circuit);

  Serial.print("packed 16 permutation: ");
  Serial.print(sliced_packed_16.state);
  Serial.print(", cycles: ");
  Serial.println(cycles_packed_16);

  Serial.print("packed 32 permutation: ");
  Serial.print(sliced_packed_32.state);
  Serial.print(", cycles: ");
  Serial.println(cycles_packed_32);
}


void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  run();
  Serial.println("#########################################");
  delay(1000);
}