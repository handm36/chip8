#include "chip8.h"
#include <SDL3/SDL.h>

#ifdef DEBUG_MODE
#include "cpu.h"
#endif

int handle_input(SDL_Event *event, Chip8_state *chip8_state) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN:
    switch (event->key.key) {
    case SDLK_1:
      chip8_state->keypad[1] = 1;
      break;
    case SDLK_2:
      chip8_state->keypad[2] = 1;
      break;
    case SDLK_3:
      chip8_state->keypad[3] = 1;
      break;
    case SDLK_4:
      chip8_state->keypad[0xC] = 1;
      break;
    case SDLK_Q:
      chip8_state->keypad[4] = 1;
      break;
    case SDLK_W:
      chip8_state->keypad[5] = 1;
      break;
    case SDLK_E:
      chip8_state->keypad[6] = 1;
      break;
    case SDLK_R:
      chip8_state->keypad[0xD] = 1;
      break;
    case SDLK_A:
      chip8_state->keypad[7] = 1;
      break;
    case SDLK_S:
      chip8_state->keypad[8] = 1;
      break;
    case SDLK_D:
      chip8_state->keypad[9] = 1;
      break;
    case SDLK_F:
      chip8_state->keypad[0xE] = 1;
      break;
    case SDLK_Z:
      chip8_state->keypad[0xA] = 1;
      break;
    case SDLK_X:
      chip8_state->keypad[0] = 1;
      break;
    case SDLK_C:
      chip8_state->keypad[0xB] = 1;
      break;
    case SDLK_V:
      chip8_state->keypad[0xF] = 1;
      break;
    }
    break;
  case SDL_EVENT_KEY_UP:
    switch (event->key.key) {
    case SDLK_1:
      chip8_state->keypad[1] = 0;
      break;
    case SDLK_2:
      chip8_state->keypad[2] = 0;
      break;
    case SDLK_3:
      chip8_state->keypad[3] = 0;
      break;
    case SDLK_4:
      chip8_state->keypad[0xC] = 0;
      break;
    case SDLK_Q:
      chip8_state->keypad[4] = 0;
      break;
    case SDLK_W:
      chip8_state->keypad[5] = 0;
      break;
    case SDLK_E:
      chip8_state->keypad[6] = 0;
      break;
    case SDLK_R:
      chip8_state->keypad[0xD] = 0;
      break;
    case SDLK_A:
      chip8_state->keypad[7] = 0;
      break;
    case SDLK_S:
      chip8_state->keypad[8] = 0;
      break;
    case SDLK_D:
      chip8_state->keypad[9] = 0;
      break;
    case SDLK_F:
      chip8_state->keypad[0xE] = 0;
      break;
    case SDLK_Z:
      chip8_state->keypad[0xA] = 0;
      break;
    case SDLK_X:
      chip8_state->keypad[0] = 0;
      break;
    case SDLK_C:
      chip8_state->keypad[0xB] = 0;
      break;
    case SDLK_V:
      chip8_state->keypad[0xF] = 0;
      break;
#ifdef DEBUG_MODE
    case SDLK_RETURN:
      run_cpu(chip8_state, 0, 0, 0);
      break;
#endif
    }
    break;
  }
  return SDL_APP_CONTINUE;
}
