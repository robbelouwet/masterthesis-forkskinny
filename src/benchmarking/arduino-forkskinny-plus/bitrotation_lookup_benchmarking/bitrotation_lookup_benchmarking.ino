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


uint16_t values_row1[] = {
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

uint16_t values_row2[] = {
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

uint16_t values_row3[] = {
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

inline uint8_t shiftrows_row2_row3(uint8_t row2row3) __attribute__((always_inline));
uint8_t shiftrows_row2_row3(uint8_t row2row3) {

  uint8_t values[] = { 0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x1, 0x3, 0x5, 0x7, 0x9,
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
  return values[row2row3];
}

/// f(x) = rotate_right(x, 1) << 8
inline uint16_t ror_row1_4bit_aligned(uint16_t input) __attribute__((always_inline));
uint16_t ror_row1_4bit_aligned(uint16_t input) {
  uint16_t values[] = {
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
  return values[input];
}

/// f(x) = rotate_right(x, 2) << 4
inline uint16_t ror_row2_4bit_aligned(uint16_t input) __attribute__((always_inline));
uint16_t ror_row2_4bit_aligned(uint16_t input) {
  uint16_t values[] = {
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
  return values[input];
}

/// f(x) = rotate_right(x, 3)
inline uint16_t ror_row3_4bit_aligned(uint16_t input) __attribute__((always_inline));
uint16_t ror_row3_4bit_aligned(uint16_t input) {
  uint16_t values[] = {
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
  return values[input];
}

void shift_1_row() {
  // how fast is local-variable access?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  uint16_t v1 = ror_row2_4bit_aligned(6);
  auto cycles_inline = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("inline function, 4-bit lookup: ");
  Serial.print(v1);
  Serial.print(", cycles: ");
  Serial.println(cycles_inline);

  // how fast is data-section access?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  uint16_t v2 = values_row2[6];
  auto cycles_data_section = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("data section, 4-bit lookup: ");
  Serial.print(v2);
  Serial.print(", cycles: ");
  Serial.println(cycles_data_section);

  // how fast is actually calculating a bit rotation?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  uint16_t v3 = ((6 >> 1) | (6 << 3)) << 4;
  auto cycles_data_calculation = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("actually calculating: ");
  Serial.print(v3);
  Serial.print(", cycles: ");
  Serial.println(cycles_data_calculation);
}

void shift_2_rows() {
  // how fast is local-variable access?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  uint16_t v1 = ror_row2_4bit_aligned(6);
  uint16_t v1a = ror_row3_4bit_aligned(6);
  auto cycles_inline = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("2x inline function 4-bit lookups: ");
  Serial.print(v1);
  Serial.print(v1a);
  Serial.print(", cycles: ");
  Serial.println(cycles_inline);

  // how fast is data-section access?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  uint16_t v2 = values_row2[6];
  uint16_t v2a = values_row3[6];
  auto cycles_data_section = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("2x data section 4-bit lookups: ");
  Serial.print(v2);
  Serial.print(v2a);
  Serial.print(", cycles: ");
  Serial.println(cycles_data_section);

  // how fast is actually calculating a bit rotation?
  uint16_t r1 = rand();
  uint16_t r2 = rand();
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter(); /* start counting */
  uint16_t v3 = ((r1 >> 1) | (r1 << 3)) << 4;
  uint16_t v3a = (r2 >> 1) | (r2 << 3);
  auto cycles_data_calculation = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("2x actually calculating: ");
  Serial.print(v3);Serial.print(v3a);
  Serial.print(", cycles: ");
  Serial.println(cycles_data_calculation);

  // how fast is the combined row2row3 lookup table?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter();/* start counting */
  auto v4 = shiftrows_row2_row3(0xCE);
  auto cycles_inline_row2row3 = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("1x inline function 8-bit lookup: ");
  Serial.print(v4);
  Serial.print(", cycles: ");
  Serial.println(cycles_inline_row2row3);

    // how fast is the combined row2row3 lookup table?
  KIN1_ResetCycleCounter();
  KIN1_EnableCycleCounter();/* start counting */
  auto v5 = shiftrows_row2_row3(0x2F);
  auto cycles_inline_row2row3_again = KIN1_GetCycleCounter(); /* Stop counting */

  Serial.print("8-bit lookup again (cache?): ");
  Serial.print(v5);
  Serial.print(", cycles: ");
  Serial.println(cycles_inline_row2row3_again);
}

void setup() {
  KIN1_InitCycleCounter(); /* enable DWT hardware */
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  KIN1_ResetCycleCounter();
  Serial.println("######## Shifting 1 row: ########");
  shift_1_row();

  Serial.println("######## Shifting 2 rows: ########");
  shift_2_rows();

  Serial.println("##################################\n\n");
  delay(1000);
}
