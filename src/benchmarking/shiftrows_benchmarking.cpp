#include "../forkskinny-plus/headers/forkskinny64-plus.h"
#include "../headers/full-state-slicing.h"

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
	
	// shift rows
	shift_rows_old(&vanilla);
	shift_rows_sliced_packed_rows(&sliced_state1);
	shift_rows_sliced_packed_state(&sliced_state2);
	shift_rows_sliced_lookup(&sliced_state3);
	
	auto unsliced1 = unslice(sliced_state1.state);
	auto unsliced2 = unslice(sliced_state2.state);
	auto unsliced3 = unslice(sliced_state3.state);
	
	int appel = 1;
}