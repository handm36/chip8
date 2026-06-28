#include <SDL3/SDL_timer.h>
#include <sys/types.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include "audio.h"
#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>
#include <stdio.h>

Chip8_state chip8_state = {0};
uint64_t last_time_timers;

static SDL_AppResult parse_file(char *path) {
  FILE *file_ptr = fopen(path, "rb");

  if (file_ptr == NULL) {
    SDL_Log("Failed to open file\n");
    return SDL_APP_FAILURE;
  }

  if (fseek(file_ptr, 0, SEEK_END) != 0) {
    fclose(file_ptr);
    SDL_Log("Failed to read file\n");
    return SDL_APP_FAILURE;
  }

  long size = ftell(file_ptr);
  if (size == -1) {
    fclose(file_ptr);
    SDL_Log("Failed to get file size\n");
    return SDL_APP_FAILURE;
  }

  // check if the file is correct and it fits into memory
  // removed the (size % 2 != 0) because of the corax rom
  if ((size == 0) || (size > TOTAL_ROM_MEMORY)) {
    fclose(file_ptr);
    SDL_Log("Invalid Chip8 rom file\n");
    return SDL_APP_FAILURE;
  }
  fseek(file_ptr, 0, SEEK_SET);

  size_t ret = fread(&(chip8_state.ram[0x200]), 1, size, file_ptr);
  if (ret != size) {
    fclose(file_ptr);
    SDL_Log("Failed to read file\n");
    return SDL_APP_FAILURE;
  }

  fclose(file_ptr);
  return SDL_APP_CONTINUE;
}

void update_timers() {
  uint64_t curr_time = SDL_GetTicks();
  // 1000ms per 60hz
  if (curr_time > last_time_timers + (1000 / 60)) {
    last_time_timers = curr_time;

    if (chip8_state.sound != 0)
      chip8_state.sound = chip8_state.sound - 1;

    if (chip8_state.delay != 0)
      chip8_state.delay = chip8_state.delay - 1;
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (argc != 2) {
    SDL_Log("Usage: %s <rom file>\n", argv[0]);
    return SDL_APP_FAILURE;
  }

  if (parse_file(argv[1]) == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  SDL_SetAppMetadata("Chip 8", "1.0", "com.chip8");

  if (init_display() == SDL_APP_FAILURE) {
    return SDL_APP_FAILURE;
  }

  populate_chip8_state(&chip8_state);
  last_time_timers = SDL_GetTicks();

  if (init_buzzer() == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  if (chip8_state.sound != 0)
    play_buzzer(&chip8_state);
  // Temporary until i fix the clockspeed
  SDL_Delay(10);

  if (render_frame(&chip8_state) == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  update_timers();
  run_cpu(&chip8_state);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
