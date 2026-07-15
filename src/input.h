#pragma once
#include "chip8.h"
#include <SDL3/SDL.h>

int handle_input(SDL_Event *event, Chip8_state *chip8);
