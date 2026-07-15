#include "chip8.h"

void populate_chip8_state(Chip8_state *chip8) {
  chip8->PC = 0x200;
  chip8->wait_until_key_up = 0xFF;

  // sprite 0
  chip8->ram[0] = 0xF0;
  chip8->ram[1] = 0x90;
  chip8->ram[2] = 0x90;
  chip8->ram[3] = 0x90;
  chip8->ram[4] = 0xF0;

  // sprite 1
  chip8->ram[5] = 0x20;
  chip8->ram[6] = 0x60;
  chip8->ram[7] = 0x20;
  chip8->ram[8] = 0x20;
  chip8->ram[9] = 0x70;

  // sprite 2
  chip8->ram[10] = 0xF0;
  chip8->ram[11] = 0x10;
  chip8->ram[12] = 0xF0;
  chip8->ram[13] = 0x80;
  chip8->ram[14] = 0xF0;

  // sprite 3
  chip8->ram[15] = 0xF0;
  chip8->ram[16] = 0x10;
  chip8->ram[17] = 0xF0;
  chip8->ram[18] = 0x10;
  chip8->ram[19] = 0xF0;

  // sprite 4
  chip8->ram[20] = 0x90;
  chip8->ram[21] = 0x90;
  chip8->ram[22] = 0xF0;
  chip8->ram[23] = 0x10;
  chip8->ram[24] = 0x10;

  // sprite 5
  chip8->ram[25] = 0xF0;
  chip8->ram[26] = 0x80;
  chip8->ram[27] = 0xF0;
  chip8->ram[28] = 0x10;
  chip8->ram[29] = 0xF0;

  // sprite 6
  chip8->ram[30] = 0xF0;
  chip8->ram[31] = 0x80;
  chip8->ram[32] = 0xF0;
  chip8->ram[33] = 0x90;
  chip8->ram[34] = 0xF0;

  // sprite 7
  chip8->ram[35] = 0xF0;
  chip8->ram[36] = 0x10;
  chip8->ram[37] = 0x20;
  chip8->ram[38] = 0x40;
  chip8->ram[39] = 0x40;

  // sprite 8
  chip8->ram[40] = 0xF0;
  chip8->ram[41] = 0x90;
  chip8->ram[42] = 0xF0;
  chip8->ram[43] = 0x90;
  chip8->ram[44] = 0xF0;

  // sprite 9
  chip8->ram[45] = 0xF0;
  chip8->ram[46] = 0x90;
  chip8->ram[47] = 0xF0;
  chip8->ram[48] = 0x10;
  chip8->ram[49] = 0xF0;

  // sprite A
  chip8->ram[50] = 0xF0;
  chip8->ram[51] = 0x90;
  chip8->ram[52] = 0xF0;
  chip8->ram[53] = 0x90;
  chip8->ram[54] = 0x90;

  // sprite B
  chip8->ram[55] = 0xE0;
  chip8->ram[56] = 0x90;
  chip8->ram[57] = 0xE0;
  chip8->ram[58] = 0x90;
  chip8->ram[59] = 0xE0;

  // sprite C
  chip8->ram[60] = 0xF0;
  chip8->ram[61] = 0x80;
  chip8->ram[62] = 0x80;
  chip8->ram[63] = 0x80;
  chip8->ram[64] = 0xF0;

  // sprite D
  chip8->ram[65] = 0xE0;
  chip8->ram[66] = 0x90;
  chip8->ram[67] = 0x90;
  chip8->ram[68] = 0x90;
  chip8->ram[69] = 0xE0;

  // sprite E
  chip8->ram[70] = 0xF0;
  chip8->ram[71] = 0x80;
  chip8->ram[72] = 0xF0;
  chip8->ram[73] = 0x80;
  chip8->ram[74] = 0xF0;

  // sprite F
  chip8->ram[75] = 0xF0;
  chip8->ram[76] = 0x80;
  chip8->ram[77] = 0xF0;
  chip8->ram[78] = 0x80;
  chip8->ram[79] = 0x80;
}
