#pragma once

#include "chip8.h"
#define BACKGROUND_R 0
#define BACKGROUND_G 0
#define BACKGROUND_B 0

#define COLOR_R 0
#define COLOR_G 0
#define COLOR_B 0

int init_display();
void render_frame(Chip8_state *chip8_state);
