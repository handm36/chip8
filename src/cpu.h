#pragma once

#include "chip8.h"
int run_cpu(Chip8_state *chip8, int display_wait_quirk, int vf_reset_quirk,
            int wrapping_quirk, int shift_quirk);
