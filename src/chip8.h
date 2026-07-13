#pragma once
#include <stdint.h>

#define INSTRUCTIONS_PER_FRAME 15
#define KEYPAD_SIZE 16
#define RAM_SIZE 4096
#define STACK_SIZE 16
#define REGISTER_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define TOTAL_ROM_MEMORY                                                       \
  (0x1000 - 0x200) // 0x1000 - 0x200 is the total memory for the rom available

typedef struct {
  uint64_t display[DISPLAY_HEIGHT]; // 64x32 display for the normal chip8
  uint16_t I;
  uint16_t PC;
  uint16_t stack[STACK_SIZE];
  uint8_t ram[RAM_SIZE];
  uint8_t V[REGISTER_SIZE];
  uint8_t keypad[KEYPAD_SIZE];
  uint8_t SP;
  uint8_t delay;
  uint8_t sound;
} Chip8_state;

void populate_chip8_state(Chip8_state *chip8_state);
