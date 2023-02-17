/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */

/*!< DWT Control register */
#define KIN1_DWT_CONTROL (*((volatile uint32_t*)0xE0001000))

/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNTENA_BIT (1UL << 0)

/*!< DWT Cycle Counter register */
#define KIN1_DWT_CYCCNT (*((volatile uint32_t*)0xE0001004))

/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_DEMCR (*((volatile uint32_t*)0xE000EDFC))

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
 * @param m: 0 <= m <= 3; the 0-based index that masks the relevant significant bit of every slices.
 * @return the input in a bit sliced_fghi manner
 */
inline uint32_t slice_index(uint32_t x, uint8_t i) __attribute__((always_inline));
uint32_t slice_index(uint32_t x, uint8_t i) {
  // because uint4_t doesn't exist
  assert(i <= 3);

  // the mask for the bit, on nibble level (so a 4-bit mask that will be shifted)
  uint32_t m = 1 << i;

	return ((x & (m << 28)) >> 21 >> i) |
	       ((x & (m << 24)) >> 18 >> i) |
	       ((x & (m << 20)) >> 15 >> i) |
	       ((x & (m << 16)) >> 12 >> i) |
	       ((x & (m << 12)) >> 9 >> i) |
	       ((x & (m << 8)) >> 6 >> i) |
	       ((x & (m << 4)) >> 3 >> i) |
	       ((x & m) >> i);
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

  uint32_t x = value;

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

void run() {
  KIN1_InitCycleCounter(); /* enable DWT hardware */

  uint32_t state = random();
  Serial.print("State: ");
  Serial.println(state);

  uint32_t sliced = slice(state);

  // --- Sliced LSFR ---
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  sliced = ((sliced & 0xFFFF000000000000) >> 0x30) ^ (sliced << 0x10);
  sliced ^= (sliced & 0xFFFF000000000000) >> 0x30;
  auto cycles_sliced = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("Sliced LSFR: ");
  Serial.print(sliced);
  Serial.print(", cycles: ");
  Serial.println(cycles_sliced);
  // -------------------



  // --- Sequential LSFR ---
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  state = ((state << 1) & 0xEEEEEEEEEEEEEEEEU) ^ (((state >> 3) ^ (state >> 2)) & 0x1111111111111111U);
  auto cycles_sequential = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("Sequential LSFR: ");
  Serial.print(state);
  Serial.print(", cycles: ");
  Serial.println(cycles_sequential);
  // -----------------------
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  run();
  delay(1000);
}
