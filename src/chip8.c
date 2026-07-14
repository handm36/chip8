#include "chip8.h"

void populate_chip8_state(Chip8_state *chip8_state) {
  chip8_state->PC = 0x200;
  chip8_state->wait_until_key_up = 0xFF;

  // sprite 0
  chip8_state->ram[0] = 0xF0;
  chip8_state->ram[1] = 0x90;
  chip8_state->ram[2] = 0x90;
  chip8_state->ram[3] = 0x90;
  chip8_state->ram[4] = 0xF0;

  // sprite 1
  chip8_state->ram[5] = 0x20;
  chip8_state->ram[6] = 0x60;
  chip8_state->ram[7] = 0x20;
  chip8_state->ram[8] = 0x20;
  chip8_state->ram[9] = 0x70;

  // sprite 2
  chip8_state->ram[10] = 0xF0;
  chip8_state->ram[11] = 0x10;
  chip8_state->ram[12] = 0xF0;
  chip8_state->ram[13] = 0x80;
  chip8_state->ram[14] = 0xF0;

  // sprite 3
  chip8_state->ram[15] = 0xF0;
  chip8_state->ram[16] = 0x10;
  chip8_state->ram[17] = 0xF0;
  chip8_state->ram[18] = 0x10;
  chip8_state->ram[19] = 0xF0;

  // sprite 4
  chip8_state->ram[20] = 0x90;
  chip8_state->ram[21] = 0x90;
  chip8_state->ram[22] = 0xF0;
  chip8_state->ram[23] = 0x10;
  chip8_state->ram[24] = 0x10;

  // sprite 5
  chip8_state->ram[25] = 0xF0;
  chip8_state->ram[26] = 0x80;
  chip8_state->ram[27] = 0xF0;
  chip8_state->ram[28] = 0x10;
  chip8_state->ram[29] = 0xF0;

  // sprite 6
  chip8_state->ram[30] = 0xF0;
  chip8_state->ram[31] = 0x80;
  chip8_state->ram[32] = 0xF0;
  chip8_state->ram[33] = 0x90;
  chip8_state->ram[34] = 0xF0;

  // sprite 7
  chip8_state->ram[35] = 0xF0;
  chip8_state->ram[36] = 0x10;
  chip8_state->ram[37] = 0x20;
  chip8_state->ram[38] = 0x40;
  chip8_state->ram[39] = 0x40;

  // sprite 8
  chip8_state->ram[40] = 0xF0;
  chip8_state->ram[41] = 0x90;
  chip8_state->ram[42] = 0xF0;
  chip8_state->ram[43] = 0x90;
  chip8_state->ram[44] = 0xF0;

  // sprite 9
  chip8_state->ram[45] = 0xF0;
  chip8_state->ram[46] = 0x90;
  chip8_state->ram[47] = 0xF0;
  chip8_state->ram[48] = 0x10;
  chip8_state->ram[49] = 0xF0;

  // sprite A
  chip8_state->ram[50] = 0xF0;
  chip8_state->ram[51] = 0x90;
  chip8_state->ram[52] = 0xF0;
  chip8_state->ram[53] = 0x90;
  chip8_state->ram[54] = 0x90;

  // sprite B
  chip8_state->ram[55] = 0xE0;
  chip8_state->ram[56] = 0x90;
  chip8_state->ram[57] = 0xE0;
  chip8_state->ram[58] = 0x90;
  chip8_state->ram[59] = 0xE0;

  // sprite C
  chip8_state->ram[60] = 0xF0;
  chip8_state->ram[61] = 0x80;
  chip8_state->ram[62] = 0x80;
  chip8_state->ram[63] = 0x80;
  chip8_state->ram[64] = 0xF0;

  // sprite D
  chip8_state->ram[65] = 0xE0;
  chip8_state->ram[66] = 0x90;
  chip8_state->ram[67] = 0x90;
  chip8_state->ram[68] = 0x90;
  chip8_state->ram[69] = 0xE0;

  // sprite E
  chip8_state->ram[70] = 0xF0;
  chip8_state->ram[71] = 0x80;
  chip8_state->ram[72] = 0xF0;
  chip8_state->ram[73] = 0x80;
  chip8_state->ram[74] = 0xF0;

  // sprite F
  chip8_state->ram[75] = 0xF0;
  chip8_state->ram[76] = 0x80;
  chip8_state->ram[77] = 0xF0;
  chip8_state->ram[78] = 0x80;
  chip8_state->ram[79] = 0x80;
}
