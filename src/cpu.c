#include "chip8.h"
#include <SDL3/SDL_init.h>

#ifdef DEBUG_MODE
#include <SDL3/SDL.h>
#endif

int run_cpu(Chip8_state *chip8, int display_wait_quirk, int vf_reset_quirk,
            int wrapping_quirk, int shift_quirk) {
  uint16_t temp;
  uint64_t translated_sprite;
  uint8_t sprite;
  uint8_t Vx;
  uint8_t Vy;
  uint8_t y;

  uint8_t first_byte = chip8->ram[chip8->PC];
  uint8_t second_byte = chip8->ram[chip8->PC + 1];

  uint8_t first_nibble = (first_byte & 0xF0) >> 4;
  uint8_t second_nibble = first_byte & 0x0F;
  uint8_t third_nibble = (second_byte & 0xF0) >> 4;
  uint8_t forth_nibble = second_byte & 0x0F;

#ifdef DEBUG_MODE
  SDL_Log("Curr instruction: %02x %02x\n\n", first_byte, second_byte);

  SDL_Log("PC: %04x\n\n", chip8->PC);

  SDL_Log("I: %04x\n\n", chip8->I);

  for (int i = 0; i < REGISTER_SIZE; i++)
    SDL_Log("V%d: %02x\n", i, chip8->V[i]);

  SDL_Log("\n");
#endif

  if (first_byte == 0x00) {
    if (second_byte == 0xE0) {
      // 00E0 cls instruction
      for (int i = 0; i < DISPLAY_HEIGHT; i++)
        chip8->display[i] = 0;

    } else if (second_byte == 0xEE) {
      // 00EE ret instruction
      chip8->PC = chip8->stack[chip8->SP];
      if (chip8->SP != 0) {
        chip8->SP = chip8->SP - 1;
      }

      return SDL_APP_CONTINUE;
    }
  } else {
    switch (first_nibble) {
    case 0x1:
      // 1nnn jump instruction
      chip8->PC = (second_nibble << 8) | second_byte;
      return SDL_APP_CONTINUE;
    case 0x2:
      // 2nnn call instruction
      if (chip8->SP != STACK_SIZE - 1)
        chip8->SP = chip8->SP + 1;

      chip8->stack[chip8->SP] = chip8->PC + 2;
      chip8->PC = (second_nibble << 8) | second_byte;
      return SDL_APP_CONTINUE;
    case 0x3:
      // 3xkk skip instruction if Vx == kk
      if (chip8->V[second_nibble] == second_byte)
        chip8->PC = chip8->PC + 2;
      break;
    case 0x4:
      // 4xkk skip instruction if Vx != kk
      if (chip8->V[second_nibble] != second_byte)
        chip8->PC = chip8->PC + 2;
      break;
    case 0x5:
      if (forth_nibble == 0) {
        // 5xy0 skip instruction if Vx == Vy
        if (chip8->V[second_nibble] == chip8->V[third_nibble])
          chip8->PC = chip8->PC + 2;
      }
      break;
    case 0x6:
      // 6xkk ld or load value into register
      chip8->V[second_nibble] = second_byte;
      break;
    case 0x7:
      // 7xkk add Vx = Vx + kk
      chip8->V[second_nibble] = chip8->V[second_nibble] + second_byte;
      break;
    case 0x8:
      switch (forth_nibble) {
      case 0:
        // 8xy0 load reg to reg Vx = Vy
        chip8->V[second_nibble] = chip8->V[third_nibble];
        break;
      case 1:
        // 8xy1 or Vx = Vx | Vy
        chip8->V[second_nibble] =
            chip8->V[second_nibble] | chip8->V[third_nibble];

        if (vf_reset_quirk == 1)
          chip8->V[0xF] = 0;
        break;
      case 2:
        // 8xy2 and Vx = Vx & Vy
        chip8->V[second_nibble] =
            chip8->V[second_nibble] & chip8->V[third_nibble];

        if (vf_reset_quirk == 1)
          chip8->V[0xF] = 0;
        break;
      case 3:
        // 8xy3 xor Vx = Vx ^ Vy
        chip8->V[second_nibble] =
            chip8->V[second_nibble] ^ chip8->V[third_nibble];

        if (vf_reset_quirk == 1)
          chip8->V[0xF] = 0;
        break;
      case 4:
        // 8xy4 + Vx = Vx + Vy
        temp = (uint16_t)chip8->V[second_nibble] +
               (uint16_t)chip8->V[third_nibble];

        chip8->V[second_nibble] = temp & 0xFF;

        if (temp > 0xFF)
          chip8->V[0xF] = 1;
        else
          chip8->V[0xF] = 0;
        break;
      case 5:
        // 8xy5 - Vx = Vx - Vy
        temp = chip8->V[second_nibble];

        chip8->V[second_nibble] = temp - chip8->V[third_nibble];

        if (temp >= chip8->V[third_nibble])
          chip8->V[0xF] = 1;
        else
          chip8->V[0xF] = 0;
        break;
      case 6:
        // 8xy6 shift right
        // VF is set to the last bit before the right shift
        if (shift_quirk == 1) {
          temp = chip8->V[second_nibble] & 0x1;
          chip8->V[second_nibble] = chip8->V[second_nibble] >> 1;
        } else {
          temp = chip8->V[third_nibble] & 0x1;
          chip8->V[second_nibble] = chip8->V[third_nibble] >> 1;
        }
        chip8->V[0xF] = temp;
        break;
      case 7:
        // 8xy7 - Vx = Vy - Vx
        temp = chip8->V[third_nibble];

        chip8->V[second_nibble] = temp - chip8->V[second_nibble];

        if (temp >= chip8->V[second_nibble])
          chip8->V[0xF] = 1;
        else
          chip8->V[0xF] = 0;
        break;
      case 0xE:
        // 8xyE shift left
        // VF is set to the first bit before the left shift
        if (shift_quirk == 1) {
          temp = !!(chip8->V[second_nibble] & 0x80);
          chip8->V[second_nibble] = chip8->V[second_nibble] << 1;
        } else {
          temp = !!(chip8->V[third_nibble] & 0x80);
          chip8->V[second_nibble] = chip8->V[third_nibble] << 1;
        }
        chip8->V[0xF] = temp;
        break;
      }
      break;
    case 9:
      if (forth_nibble == 0) {
        // 9xy0 skip instruction if the value of register Vx is not equal to the
        // value of register Vy
        if (chip8->V[second_nibble] != chip8->V[third_nibble])
          chip8->PC = chip8->PC + 2;
      }
      break;
    case 0xA:
      // Annn ld or load value into I
      chip8->I = (second_nibble << 8) | second_byte;
      break;
    case 0xB:
      // Bnnn jump NNN + v0
      chip8->PC = ((second_nibble << 8) | second_byte) + chip8->V[0];
      return SDL_APP_CONTINUE;
    case 0xC:
      // CxNN set Vx to a random number with NN as the mask
      chip8->V[second_nibble] = SDL_rand(256) & second_byte;
      break;
    case 0xD:
      // Dxyn draws sprite onto the display
      Vx = chip8->V[second_nibble] % DISPLAY_WIDTH;
      Vy = chip8->V[third_nibble] % DISPLAY_HEIGHT;
      chip8->V[0xF] = 0;

      // forth_nibble bc n is last
      for (int i = 0; i < forth_nibble; i++) {
        sprite = chip8->ram[chip8->I + i];

        y = Vy + i;
        if (y >= DISPLAY_HEIGHT) {
          if (wrapping_quirk == 1)
            y = y - DISPLAY_HEIGHT;
          else
            break;
        }

        // 64 - 8 = 56
        translated_sprite = ((uint64_t)sprite << 56) >> Vx;

        if (wrapping_quirk && Vx > 56)
          translated_sprite = translated_sprite | (uint64_t)sprite
                                                      << (64 - (Vx - 56));

        // collision check if Xor'd == Or'd then there isnt collision
        if ((chip8->display[y] ^ translated_sprite) !=
            (chip8->display[y] | translated_sprite))
          chip8->V[0xF] = 1;

        chip8->display[y] = chip8->display[y] ^ translated_sprite;
      }

      chip8->PC = chip8->PC + 2;

      if (display_wait_quirk == 1)
        return SDL_APP_SUCCESS;

      return SDL_APP_CONTINUE;
    case 0xE:
      switch (second_byte) {
      case 0x9E:
        // Ex9E skip the next instruction if the key in Vx is held
        if (chip8->keypad[chip8->V[second_nibble] & 0xF] == 1)
          chip8->PC = chip8->PC + 2;
        break;
      case 0xA1:
        // ExA1 skip the next instruction if the key in Vx is not held
        if (chip8->keypad[chip8->V[second_nibble] & 0xF] == 0)
          chip8->PC = chip8->PC + 2;
        break;
      }
      break;
    case 0xF:
      switch (second_byte) {
      case 0x07:
        // Fx07 Vx = delay
        chip8->V[second_nibble] = chip8->delay;
        break;
      case 0x0A:
        // Fx0A wait for a keypress and store result in Vx
        if (chip8->wait_until_key_up != 0xFF) {
          if (chip8->keypad[chip8->wait_until_key_up] == 0) {
            chip8->wait_until_key_up = 0xFF;
            chip8->PC = chip8->PC + 2;
            return SDL_APP_CONTINUE;
          } else {
            return SDL_APP_SUCCESS;
          }
        }

        for (int i = 0; i < KEYPAD_SIZE; i++) {
          if (chip8->keypad[i] == 1) {
            chip8->V[second_nibble] = i;
            chip8->wait_until_key_up = i;
          }
        }

        return SDL_APP_SUCCESS;
      case 0x15:
        // Fx15 delay = Vx
        chip8->delay = chip8->V[second_nibble];
        break;
      case 0x18:
        // Fx18 sound = Vx
        chip8->sound = chip8->V[second_nibble];
        break;
      case 0x1E:
        // Fx1E I = I + Vx
        chip8->I = chip8->I + chip8->V[second_nibble];
        break;
      case 0x29:
        // Fx29 set I to the memory address of the sprite data corresponding to
        // the hexadecimal digit stored in register Vx

        // * 5 cause the offset between sprites is 5
        chip8->I = (chip8->V[second_nibble] & 0xF) * 5;
        break;
      case 0x33:
        // Fx33 place the ones in I+2, the tens in I+1, and the hundreds in I of
        // the decimal value of Vx
        chip8->ram[chip8->I + 2] = chip8->V[second_nibble] % 10;
        chip8->ram[chip8->I + 1] = (chip8->V[second_nibble] / 10) % 10;
        chip8->ram[chip8->I] = (chip8->V[second_nibble] / 100) % 10;

        break;
      case 0x55:
        // Fx55 fill I with V0 to Vx recursively
        for (int i = 0; i <= second_nibble; i++)
          chip8->ram[chip8->I + i] = chip8->V[i];

        chip8->I = chip8->I + second_nibble + 1;
        break;
      case 0x65:
        // Fx65 fill V0 to Vx with I recursively
        for (int i = 0; i <= second_nibble; i++)
          chip8->V[i] = chip8->ram[chip8->I + i];

        chip8->I = chip8->I + second_nibble + 1;
        break;
      }
      break;
    }
  }

  chip8->PC = chip8->PC + 2;
  return SDL_APP_CONTINUE;
}
