#include "../forkskinny-plus/headers/forkskinny64-plus.h"
#include "../headers/full-state-slicing.h"
#include "../utils.h"

static inline void shift_rows_sliced_lookup_half_slices(State64Sliced2_16_t *state) {
	uint16_t row0_row1[] = {0x0, 0x8, 0x1, 0x9, 0x2, 0xa, 0x3, 0xb, 0x4, 0xc, 0x5, 0xd, 0x6,
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
	                        0xfe, 0xf7, 0xff};
	
	uint16_t row2_row3[] = {0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x1, 0x3, 0x5, 0x7, 0x9,
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
	                        0xfb, 0xfd, 0xff};
	
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

static inline void shift_rows_sliced_lookup(State64Sliced_16_t *state) {
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

static inline void shift_rows_sliced_packed_state(State64Sliced_16_t *state) {
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

static inline void shift_rows_sliced_packed_rows_fast_alignment(State64Sliced_16_t *state) {
	// @formatter:off
	auto slice0 = state->slices[0];
	auto slice1 = state->slices[1];
	auto slice2 = state->slices[2];
	auto slice3 = state->slices[3];
	
	// HIER ------------------ zie notities afmaken
	// rotate-right slice 0, 1, 2 and 3 by 0, 4, 8 and 12 bits respectively to align them s.t. the slice bits of every row fit next to each other
	uint16_t slice1_aligned = (slice1 >> 4) | (slice1 << 12);
	uint16_t slice2_aligned = (slice2 >> 8) | (slice1 << 8);
	uint16_t slice3_aligned = (slice1 >> 12) | (slice1 << 4);
	
	// Split up every slice into 4 rows each
	// So if slice0 contains 16 bits, then every bit of the first 4 bits of slice0 correspond to the most significant bit
	// of the first 4 cells (-> MSB because it's the first slice).
	// By this analogy, pack all the bits of the same row over every slice next to each other.
	uint16_t first_rows =   (slice0 & 0x0F00) | (slice1_aligned & 0x00F0) | (slice2_aligned & 0x000F) | (slice3_aligned & 0xF000);
	uint16_t second_rows =  (slice0 & 0x00F0) | (slice1_aligned & 0x000F) | (slice2_aligned & 0xF000) | (slice3_aligned & 0x0F00);
	uint16_t third_rows =   (slice0 & 0x000F) | (slice1_aligned & 0xF000) | (slice2_aligned & 0x0F00) | (slice3_aligned & 0x00F0);
	
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

static inline void shift_rows_sliced_packed_rows(State64Sliced_16_t *state) {
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

static inline void shift_rows_old(State64_t *state) {
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

int main(int argc, char **argv) {
	uint64_t state = 0x0123456789abcdef;
	
	auto vanilla = State64_t();
	vanilla.llrow = state;
	
	// slice the state
	auto sliced_state1 = State64Sliced_16_t();
	sliced_state1.state = slice(state);
	auto sliced_state2 = State64Sliced_16_t();
	sliced_state2.state = slice(state);
	auto sliced_state3 = State64Sliced_16_t();
	sliced_state3.state = slice(state);
	auto sliced_state4 = State64Sliced2_16_t();
	sliced_state4.state = slice(state);
	
	// shift rows
	shift_rows_old(&vanilla);
	shift_rows_sliced_packed_rows(&sliced_state1);
	shift_rows_sliced_packed_state(&sliced_state2);
	shift_rows_sliced_lookup(&sliced_state3);
	shift_rows_sliced_lookup_half_slices(&sliced_state4);
	
	auto unsliced1 = unslice(sliced_state1.state);
	auto unsliced2 = unslice(sliced_state2.state);
	auto unsliced3 = unslice(sliced_state3.state);
	auto unsliced4 = unslice(sliced_state4.state);
	
	int appel = 1;
}