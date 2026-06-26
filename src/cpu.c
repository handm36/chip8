#include "chip8.h"
#include <stdint.h>
#include <sys/types.h>

void run_cpu(Chip8_state *chip8_state) {
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
      // todo
    }
  }
  switch (first_4_bits_instruction) {
  case 0x1:
    // 1nnn jump instruction
    chip8_state->PC =
        (second_4_bits_instruction << 8) | second_byte_instruction;
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
      translated_sprite = ((uint64_t)sprite << (56 - Vx));

      if (Vx > (56 - 1)) {
        translated_sprite = translated_sprite | (uint64_t)sprite
                                                    << (63 - (Vx - 56));
      }
      // collision check if Xor'd == Or'd then there isnt collision
      if ((chip8_state->display[y] ^ translated_sprite) !=
          (chip8_state->display[y] | translated_sprite))
        chip8_state->V[0xF] = 1;

      chip8_state->display[y] = chip8_state->display[y] ^ translated_sprite;
    }
    break;
  }

  chip8_state->PC = chip8_state->PC + 2;
}
