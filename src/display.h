#pragma once

#include "chip8.h"
#define BACKGROUND_R 0
#define BACKGROUND_G 0
#define BACKGROUND_B 0

#define COLOR_HEX 0xFFFFFFFF

int init_display();
int render_frame(Chip8_state *chip8_state);
