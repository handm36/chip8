#include "chip8.h"
#include <stdint.h>
#include <sys/types.h>

void run_cpu(Chip8_state *chip8_state) {
  uint16_t temp;
  uint64_t translated_sprite;
  uint8_t sprite;
  uint8_t Vx;
  uint8_t Vy;
  uint8_t y;

  uint8_t first_byte_instruction = chip8_state->ram[chip8_state->PC];
  uint8_t second_byte_instruction = chip8_state->ram[chip8_state->PC + 1];

  uint8_t first_4_bits_instruction = (first_byte_instruction & 0xF0) >> 4;
  uint8_t second_4_bits_instruction = first_byte_instruction & 0x0F;
  uint8_t third_4_bits_instruction = (second_byte_instruction & 0xF0) >> 4;
  uint8_t forth_4_bits_instruction = second_byte_instruction & 0x0F;

  if (first_byte_instruction == 0x00) {
    if (second_byte_instruction == 0xE0) {
      // 00E0 cls instruction
      for (int i = 0; i < DISPLAY_HEIGHT; i++)
        chip8_state->display[i] = 0;

    } else {
      // 00EE ret instruction
      chip8_state->PC = chip8_state->stack[chip8_state->SP];
      if (chip8_state->SP != 0)
        chip8_state->SP = chip8_state->SP - 1;
    }
  }

  switch (first_4_bits_instruction) {
  case 0x1:
    // 1nnn jump instruction
    chip8_state->PC =
        (second_4_bits_instruction << 8) | second_byte_instruction;
    return;
  case 0x2:
    // 2nnn call instruction
    if (chip8_state->SP != 0)
      chip8_state->SP = chip8_state->SP + 1;

    chip8_state->stack[chip8_state->SP] = chip8_state->PC;
    chip8_state->PC =
        (second_4_bits_instruction << 8) | second_byte_instruction;
    return;
  case 0x3:
    // 3xkk skip instruction if Vx == kk
    if (chip8_state->V[second_4_bits_instruction] == second_byte_instruction)
      chip8_state->PC = chip8_state->PC + 2;
    break;
  case 0x4:
    // 4xkk skip instruction if Vx != kk
    if (chip8_state->V[second_4_bits_instruction] != second_byte_instruction)
      chip8_state->PC = chip8_state->PC + 2;
    break;
  case 0x5:
    // 5xy0 skip instruction if Vx == Vy
    if (chip8_state->V[second_4_bits_instruction] ==
        chip8_state->V[third_4_bits_instruction])
      chip8_state->PC = chip8_state->PC + 2;
    break;
  case 0x6:
    // 6xkk ld or load value into register
    chip8_state->V[second_4_bits_instruction] = second_byte_instruction;
    break;
  case 0x7:
    // 7xkk add Vx = Vx + kk
    chip8_state->V[second_4_bits_instruction] =
        chip8_state->V[second_4_bits_instruction] + second_byte_instruction;
    break;
  case 0x8:
    switch (forth_4_bits_instruction) {
    case 0:
      // 8xy0 load reg to reg Vx = Vy
      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[third_4_bits_instruction];
      break;
    case 1:
      // 8xy1 or Vx = Vx | Vy
      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[second_4_bits_instruction] |
          chip8_state->V[third_4_bits_instruction];
      break;
    case 2:
      // 8xy2 and Vx = Vx & Vy
      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[second_4_bits_instruction] &
          chip8_state->V[third_4_bits_instruction];
      break;
    case 3:
      // 8xy3 xor Vx = Vx ^ Vy
      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[second_4_bits_instruction] ^
          chip8_state->V[third_4_bits_instruction];
      break;
    case 4:
      // 8xy4 + Vx = Vx + Vy
      temp = (uint16_t)chip8_state->V[second_4_bits_instruction] +
             (uint16_t)chip8_state->V[third_4_bits_instruction];
      if (temp > 0xFF)
        chip8_state->V[0xF] = 1;
      else
        chip8_state->V[0xF] = 0;

      chip8_state->V[second_4_bits_instruction] = temp & 0xFF;
      break;
    case 5:
      // 8xy5 - Vx = Vx - Vy
      if (chip8_state->V[second_4_bits_instruction] >
          chip8_state->V[third_4_bits_instruction])
        chip8_state->V[0xF] = 1;
      else
        chip8_state->V[0xF] = 0;

      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[second_4_bits_instruction] -
          chip8_state->V[third_4_bits_instruction];
      break;
    case 6:
      // 8xy6 shift right
      // VF is set to the last bit before the right shift
      chip8_state->V[0xF] = chip8_state->V[third_4_bits_instruction] & 0x1;
      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[third_4_bits_instruction] >> 1;
      break;
    case 7:
      // 8xy7 - Vx = Vy - Vx
      if (chip8_state->V[second_4_bits_instruction] <
          chip8_state->V[third_4_bits_instruction])
        chip8_state->V[0xF] = 1;
      else
        chip8_state->V[0xF] = 0;

      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[third_4_bits_instruction] -
          chip8_state->V[second_4_bits_instruction];
      break;
    case 0xE:
      // 8xyE shift left
      // VF is set to the first bit before the left shift
      chip8_state->V[0xF] = chip8_state->V[third_4_bits_instruction] & 0x80;
      chip8_state->V[second_4_bits_instruction] =
          chip8_state->V[third_4_bits_instruction] << 1;
      break;
    }
    break;
  case 0xA:
    // Annn ld or load value into I
    chip8_state->I = (second_4_bits_instruction << 8) | second_byte_instruction;
    break;
  case 0xD:
    // Dxyn draws sprite onto the display
    Vx = chip8_state->V[second_4_bits_instruction];
    Vy = chip8_state->V[third_4_bits_instruction];
    chip8_state->V[0xF] = 0;

    // forth_4_bits_instruction bc n is last
    for (int i = 0; i < forth_4_bits_instruction; i++) {
      sprite = chip8_state->ram[chip8_state->I + i];

      y = Vy + i;
      if (y > DISPLAY_HEIGHT - 1)
        y = y - DISPLAY_HEIGHT;

      // 64 - 8 = 56
      translated_sprite = ((uint64_t)sprite << 56) >> Vx;

      if (Vx > (56 - 1)) {
        translated_sprite = translated_sprite | (uint64_t)sprite
                                                    << (64 - (Vx - 56));
      }
      // collision check if Xor'd == Or'd then there isnt collision
      if ((chip8_state->display[y] ^ translated_sprite) !=
          (chip8_state->display[y] | translated_sprite))
        chip8_state->V[0xF] = 1;

      chip8_state->display[y] = chip8_state->display[y] ^ translated_sprite;
    }
    break;
  case 0xF:
    switch (second_byte_instruction) {
    case 0x1E:
      // Fx1E I = I + Vx
      chip8_state->I =
          chip8_state->I + chip8_state->V[second_4_bits_instruction];
      break;
    case 0x33:
      // Fx33 place the ones in I+2, the tens in I+1, and the hundreds in I of
      // the decimal value of Vx
      chip8_state->ram[chip8_state->I + 2] =
          chip8_state->V[second_4_bits_instruction] % 10;
      chip8_state->ram[chip8_state->I + 1] =
          (chip8_state->V[second_4_bits_instruction] / 10) % 10;
      chip8_state->ram[chip8_state->I] =
          (chip8_state->V[second_4_bits_instruction] / 100) % 10;

      break;
    case 0x55:
      // Fx55 fill I with V0 to Vx recursively
      for (int i = 0; i <= second_4_bits_instruction; i++)
        chip8_state->ram[chip8_state->I + i] = chip8_state->V[i];

      chip8_state->I = chip8_state->I + second_4_bits_instruction + 1;
      break;
    case 0x65:
      // Fx65 fill V0 to Vx with I recursively
      for (int i = 0; i <= second_4_bits_instruction; i++)
        chip8_state->V[i] = chip8_state->ram[chip8_state->I + i];

      chip8_state->I = chip8_state->I + second_4_bits_instruction + 1;
      break;
    }
    break;
  }

  chip8_state->PC = chip8_state->PC + 2;
}
