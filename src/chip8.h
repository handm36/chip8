#pragma once
#include <stdint.h>

#define RAM_SIZE 4096
#define STACK_SIZE 16
#define REGISTER_SIZE 16
#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 64
#define TOTAL_ROM_MEMORY                                                       \
  0x1000 - 0x200 // 0x1000 - 0x200 is the total memory for the rom available

typedef struct {
  uint8_t ram[RAM_SIZE];
  uint16_t I;
  uint8_t V[REGISTER_SIZE];
  uint16_t PC;
  uint16_t stack[STACK_SIZE];
  uint8_t SP;
  uint8_t delay;
  uint8_t sound;
  uint64_t display[DISPLAY_WIDTH]; // 64x32 display for the normal chip8
} Chip8_state;
