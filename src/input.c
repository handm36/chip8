#include "chip8.h"
#include <SDL3/SDL.h>

#ifdef DEBUG_MODE
#include "cpu.h"
#endif

int handle_input(SDL_Event *event, Chip8_state *chip8) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN:
    switch (event->key.key) {
    case SDLK_1:
      chip8->keypad[1] = 1;
      break;
    case SDLK_2:
      chip8->keypad[2] = 1;
      break;
    case SDLK_3:
      chip8->keypad[3] = 1;
      break;
    case SDLK_4:
      chip8->keypad[0xC] = 1;
      break;
    case SDLK_Q:
      chip8->keypad[4] = 1;
      break;
    case SDLK_W:
      chip8->keypad[5] = 1;
      break;
    case SDLK_E:
      chip8->keypad[6] = 1;
      break;
    case SDLK_R:
      chip8->keypad[0xD] = 1;
      break;
    case SDLK_A:
      chip8->keypad[7] = 1;
      break;
    case SDLK_S:
      chip8->keypad[8] = 1;
      break;
    case SDLK_D:
      chip8->keypad[9] = 1;
      break;
    case SDLK_F:
      chip8->keypad[0xE] = 1;
      break;
    case SDLK_Z:
      chip8->keypad[0xA] = 1;
      break;
    case SDLK_X:
      chip8->keypad[0] = 1;
      break;
    case SDLK_C:
      chip8->keypad[0xB] = 1;
      break;
    case SDLK_V:
      chip8->keypad[0xF] = 1;
      break;
    }
    break;
  case SDL_EVENT_KEY_UP:
    switch (event->key.key) {
    case SDLK_1:
      chip8->keypad[1] = 0;
      break;
    case SDLK_2:
      chip8->keypad[2] = 0;
      break;
    case SDLK_3:
      chip8->keypad[3] = 0;
      break;
    case SDLK_4:
      chip8->keypad[0xC] = 0;
      break;
    case SDLK_Q:
      chip8->keypad[4] = 0;
      break;
    case SDLK_W:
      chip8->keypad[5] = 0;
      break;
    case SDLK_E:
      chip8->keypad[6] = 0;
      break;
    case SDLK_R:
      chip8->keypad[0xD] = 0;
      break;
    case SDLK_A:
      chip8->keypad[7] = 0;
      break;
    case SDLK_S:
      chip8->keypad[8] = 0;
      break;
    case SDLK_D:
      chip8->keypad[9] = 0;
      break;
    case SDLK_F:
      chip8->keypad[0xE] = 0;
      break;
    case SDLK_Z:
      chip8->keypad[0xA] = 0;
      break;
    case SDLK_X:
      chip8->keypad[0] = 0;
      break;
    case SDLK_C:
      chip8->keypad[0xB] = 0;
      break;
    case SDLK_V:
      chip8->keypad[0xF] = 0;
      break;
#ifdef DEBUG_MODE
    case SDLK_RETURN:
      run_cpu(chip8, 0, 0, 0, 0);
      break;
#endif
    }
    break;
  }
  return SDL_APP_CONTINUE;
}
