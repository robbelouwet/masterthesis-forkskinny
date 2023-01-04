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
  uint64_t state;
  //__m64 m64state;  // SIMD 64-bit register
} State64Sliced_16_t;

/**
 * Union that describes a 64-bit 4x4 array of cells16.
 */
typedef union {
  uint16_t row[4];   /// Stored as 4x 16-bit uint partitions (cccc cccc cccc cccc) [c = 4-bit state cell]
  uint32_t lrow[2];  /// Stored as 2x 32-bit uint partitions  (cccccccc cccccccc)
  uint64_t llrow;    /// Stored as 1x 64-bit uint 			  (cccccccccccccccc)
} State64_t;

/**
 * For every nibble, take the ith significant bit and pack those all together.
 *
 * E.g.:
 *       v    v    v
 * x = 1000 1010 0000
 * i = 1 = slice the 2nd LSB of every nibble (0-based index)
 * output: 010
 *
 * @param x: the input to slice
 * @param m: 0 <= m <= 3; the 0-based index that masks the relevant significant bit of every cell.
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

/// f(x) = rotate_right(x, 1) << 8
inline uint16_t ror_row1_4bit_aligned(uint8_t input) __attribute__((always_inline));
uint16_t ror_row1_4bit_aligned(uint8_t input) {
	uint16_t values[] = {0x000, 0x800, 0x100, 0x900,
	                     0x200, 0xa00, 0x300, 0xb00,
	                     0x400, 0xc00, 0x500, 0xd00,
	                     0x600, 0xe00, 0x700, 0xf00,
	};
	return values[input];
}

/// f(x) = rotate_right(x, 2) << 4
inline uint16_t ror_row2_4bit_aligned(uint8_t input) __attribute__((always_inline));
uint16_t ror_row2_4bit_aligned(uint8_t input) {
	uint16_t values[] = {0x00, 0x40, 0x80, 0xc0,
	                     0x10, 0x50, 0x90, 0xd0,
	                     0x20, 0x60, 0xa0, 0xe0,
	                     0x30, 0x70, 0xb0, 0xf0,
	};
	return values[input];
}

/// f(x) = rotate_right(x, 3)
inline uint16_t ror_row3_4bit_aligned(uint8_t input) __attribute__((always_inline));
uint16_t ror_row3_4bit_aligned(uint8_t input) {
	uint16_t values[] = {0x0, 0x2, 0x4, 0x6,
	                     0x8, 0xa, 0xc, 0xe,
	                     0x1, 0x3, 0x5, 0x7,
	                     0x9, 0xb, 0xd, 0xf,
	};
	return values[input];
}
inline void shift_rows_sliced_lookup(State64Sliced_16_t *state) __attribute__((always_inline));
void shift_rows_sliced_lookup(State64Sliced_16_t *state) {
auto slice0 = state->slices[0];
	auto slice1 = state->slices[1];
	auto slice2 = state->slices[2];
	auto slice3 = state->slices[3];
	
	uint16_t values_row1[] = {0x000, 0x800, 0x100, 0x900,
	                          0x200, 0xa00, 0x300, 0xb00,
	                          0x400, 0xc00, 0x500, 0xd00,
	                          0x600, 0xe00, 0x700, 0xf00,
	};
	
	uint16_t values_row2[] = {0x00, 0x40, 0x80, 0xc0,
	                          0x10, 0x50, 0x90, 0xd0,
	                          0x20, 0x60, 0xa0, 0xe0,
	                          0x30, 0x70, 0xb0, 0xf0,
	};
	
	uint16_t values_row3[] = {0x0, 0x2, 0x4, 0x6,
	                          0x8, 0xa, 0xc, 0xe,
	                          0x1, 0x3, 0x5, 0x7,
	                          0x9, 0xb, 0xd, 0xf,
	};
	
	state->slices[0] = (slice0 & 0xF000)
	                   | values_row1[(slice0 & 0x0F00) >> 8]
	                   | values_row2[(slice0 & 0x00F0) >> 4]
	                   | values_row3[slice0 & 0x000F];
	
	state->slices[1] = (slice1 & 0xF000)
	                   | values_row1[(slice1 & 0x0F00) >> 8]
	                   | values_row2[(slice1 & 0x00F0) >> 4]
	                   | values_row3[slice1 & 0x000F];
	
	state->slices[2] = (slice2 & 0xF000)
	                   | values_row1[(slice2 & 0x0F00) >> 8]
	                   | values_row2[(slice2 & 0x00F0) >> 4]
	                   | values_row3[slice2 & 0x000F];
	
	state->slices[3] = (slice3 & 0xF000)
	                   | values_row1[(slice3 & 0x0F00) >> 8]
	                   | values_row2[(slice3 & 0x00F0) >> 4]
	                   | values_row3[slice3 & 0x000F];
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
	
	// Split up every slice into 4 rows each
	// So if slice0 contains 16 bits, then every bit of the first 4 bits of slice0 correspond to the most significant bit
	// of the first 4 cells (-> MSB because it's the first slice).
	// By this analogy, pack all the bits of the same row over every slice next to each other.
	uint16_t first_rows =   ((slice0 & 0x0F00) << 4)    | (slice1 & 0x0F00)         | ((slice2 & 0x0F00) >> 4)  | ((slice3 & 0x0F00) >> 8);
	uint16_t second_rows =  ((slice0 & 0x00F0) << 8)    | ((slice1 & 0x00F0) << 4)  | (slice2 & 0x00F0)         | ((slice3 & 0x00F0) >> 4);
	uint16_t third_rows =   ((slice0 & 0x000F) << 12)   | ((slice1 & 0x000F) << 8)  | ((slice2 & 0x000F) << 4)  | (slice3 & 0x000F);
	
	// now perform the shift row (ror = rotate-right)
	first_rows =    ((first_rows >> 1) & 0x7777)    | ((first_rows << 3) & 0x8888); // ror 1
	second_rows =   ((second_rows >> 2) & 0x3333)   | ((second_rows << 2) & 0xCCCC); // ror 2
	third_rows =    ((third_rows >> 3) & 0x1111)    | ((third_rows << 1) & 0xEEEE); // ror 3
	
	// reconstruct the slices
	state->slices[0] = (slice0 & 0xF000)  | ((first_rows & 0xF000) >> 4)  | ((second_rows & 0xF000) >> 8) | ((third_rows & 0xF000) >> 12);
	state->slices[1] = (slice1 & 0xF000)  | (first_rows  & 0x0F00)        | ((second_rows & 0x0F00) >> 4) | ((third_rows & 0x0F00) >> 8);
	state->slices[2] = (slice2 & 0xF000)  | ((first_rows & 0x00F0) << 4)  | (second_rows  & 0x00F0)       | ((third_rows & 0x00F0) >> 4);
	state->slices[3] = (slice3 & 0xF000)  | ((first_rows & 0x000F) << 8)  | ((second_rows & 0x000F) << 4) | (third_rows & 0x000F);
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
	
	state->row[2] = (row2 >> 4) | (row2 << 12);// skinny64_rotate_right(state->row[1], 4);
	state->row[1] = (row1 >> 8) | (row1 << 8);// skinny64_rotate_right(state->row[2], 8);
	state->row[0] = (row0 >> 12) | (row0 << 4);// skinny64_rotate_right(state->row[3], 12);
}

void run(){
  KIN1_InitCycleCounter(); /* enable DWT hardware */

  uint64_t state = (((uint64_t)rand()) << 32) | ((uint64_t)rand());  // 2⁶⁴
  Serial.print("State: ");
  Serial.println(state);
	
	auto vanilla = State64_t();
	vanilla.llrow = state;
	
	// slice the state
	auto sliced_packed_rows = State64Sliced_16_t();
	sliced_packed_rows.state = slice(state);
	auto sliced_packed_state = State64Sliced_16_t();
	sliced_packed_state.state = slice(state);
  auto sliced_lookup = State64Sliced_16_t();
	sliced_lookup.state = slice(state);
	
	// shift rows
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
	shift_rows_old(&vanilla);
  auto cycles_vanilla = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
	shift_rows_sliced_packed_rows(&sliced_packed_rows);
  auto cycles_packed_rows = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
	shift_rows_sliced_packed_state(&sliced_packed_state);
  auto cycles_packed_state = KIN1_GetCycleCounter();

  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
	shift_rows_sliced_lookup(&sliced_lookup);
  auto cycles_lookup = KIN1_GetCycleCounter();
	
	auto unsliced_packed_rows = unslice(sliced_packed_rows.state);
	auto unsliced_lookup = unslice(sliced_lookup.state);
  

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
}

void setup(){
  Serial.begin(9600);
  while (!Serial){}
}

void loop(){
  Serial.println("#####################################");
  run();
  delay(1000);
}