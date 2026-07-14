#define SDL_MAIN_USE_CALLBACKS 1
#include "audio.h"
#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Chip8_state chip8_state = {0};
uint64_t last_time_timers;
uint64_t last_time_frame;

int display_wait_quirk = 0; // 1 if option is selected
int vf_reset_quirk = 0;
int wrapping_quirk = 0;
int mute = 0;
int shift_quirk = 0;
int cycles_per_frame = INSTRUCTIONS_PER_FRAME;
char *file_path;

static void print_usage(char *app_name) {
  SDL_Log("Usage: %s [options] <rom file>\n\n"
          "Options:\n   "
          "--cycles <cycles>, -c   Set the cycles per frame\n   "
          "--disp-wait,       -d   Enable the display wait quirk\n   "
          "--file <path>,     -f   Set the chip 8 rom file path\n   "
          "--mute,            -m   Mutes the buzzer\n   "
          "--shift,           -s   Enable the shift quirk\n   "
          "--vf-reset,        -r   Enable the Vf reset quirk\n   "
          "--wrap,            -w   Enable the wrapping quirk\n   ",
          app_name);
}

static int handle_args(int argc, char *argv[]) {
  int opt;
  int opt_i;
  static struct option options[] = {
      {"cycles", required_argument, 0, 'c'}, {"disp-wait", no_argument, 0, 'd'},
      {"file", required_argument, 0, 'f'},   {"mute", no_argument, 0, 'm'},
      {"shift", no_argument, 0, 's'},        {"vf-reset", no_argument, 0, 'r'},
      {"wrap", no_argument, 0, 'w'},         {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "rdwsc:f:m", options, &opt_i)) != -1) {
    switch (opt) {
    case 'r':
      vf_reset_quirk = 1;
      break;
    case 'd':
      display_wait_quirk = 1;
      break;
    case 'w':
      wrapping_quirk = 1;
      break;
    case 's':
      shift_quirk = 1;
      break;
    case 'c':
      cycles_per_frame = atoi(optarg);
      if (cycles_per_frame == 0) {
        SDL_Log("Invalid cycles per frame.\n");
        print_usage(argv[0]);
        return SDL_APP_FAILURE;
      }
      break;
    case 'f':
      file_path = optarg;
      break;
    case 'm':
      mute = 1;
      break;
    case '?':
      print_usage(argv[0]);
      return SDL_APP_FAILURE;
    }
  }

  if (!file_path && optind < argc)
    file_path = argv[optind];

  return SDL_APP_SUCCESS;
}

static SDL_AppResult parse_file(char *path) {
  FILE *file_ptr = fopen(path, "rb");

  if (file_ptr == NULL) {
    SDL_Log("No file specified\n");
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

static void update_timers() {
  // 1s per 60hz
  if (SDL_GetTicksNS() > last_time_timers + (uint64_t)(1.0e9 / 60)) {
    last_time_timers = SDL_GetTicksNS();

    if (chip8_state.sound != 0)
      chip8_state.sound = chip8_state.sound - 1;

    if (chip8_state.delay != 0)
      chip8_state.delay = chip8_state.delay - 1;
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (handle_args(argc, argv) == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  if (parse_file(file_path) == SDL_APP_FAILURE) {
    print_usage(argv[0]);
    return SDL_APP_FAILURE;
  }

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetAppMetadata("Chip 8", "1.0", "com.chip8");

  if (init_display() == SDL_APP_FAILURE) {
    return SDL_APP_FAILURE;
  }

  populate_chip8_state(&chip8_state);
  last_time_timers = SDL_GetTicksNS();
  last_time_frame = SDL_GetTicksNS();

  if (init_buzzer() == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  return handle_input(event, &chip8_state);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
#ifdef DEBUG_MODE
  if (render_frame(&chip8_state) == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  if (chip8_state.sound != 0)
    chip8_state.sound = chip8_state.sound - 1;

  if (chip8_state.delay != 0)
    chip8_state.delay = chip8_state.delay - 1;
#else
  for (int i = 0; i < cycles_per_frame; i++) {
    if (run_cpu(&chip8_state, display_wait_quirk, vf_reset_quirk,
                wrapping_quirk, shift_quirk) ==
        SDL_APP_SUCCESS) // SDL_APP_SUCCESS signals early exit for instructions
                         // like Dxyn and Fx0A
      break;
  }

  if (render_frame(&chip8_state) == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  update_timers();

  if (mute == 0) {
    if (chip8_state.sound == 0)
      stop_buzzer();
    else
      play_buzzer();
  }

  // 1.0e9/60 is the nanoseconds it takes to do 60 fps
  uint64_t elapsed = SDL_GetTicksNS() - last_time_frame;
  uint64_t ns_per_fps = (uint64_t)(1.0e9 / 60);
  if (elapsed < ns_per_fps)
    SDL_DelayNS(ns_per_fps - elapsed);

  last_time_frame = SDL_GetTicksNS();
#endif
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
