#pragma once

#include "chip8.h"
#define BACKGROUND_R 36
#define BACKGROUND_G 41
#define BACKGROUND_B 51

// For the bitmap_to_RGBA
#define BACKGROUND_HEX 0x242933FF
#define COLOR_HEX 0x88C0D0FF

int init_display();
int render_frame(Chip8_state *chip8);
